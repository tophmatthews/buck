#include "CreepUC.h"

#include "BurnupFunction.h"
#include "MaterialUC.h"
#include "SymmIsotropicElasticityTensor.h"

template<>
InputParameters validParams<CreepUC>()
{
  InputParameters params = validParams<SolidModel>();

  //  Material parameters

  //  Sub-Newton Iteration control parameters
  params.addParam<Real>("relative_tolerance", 1e-4, "Relative convergence tolerance for sub-newtion iteration");
  params.addParam<Real>("absolute_tolerance", 1e-20, "Absolute convergence tolerance for sub-newtion iteration");
  params.addParam<unsigned int>("max_its", 10, "Maximum number of sub-newton iterations");
  params.addParam<bool>("output_iteration_info", false, "Set true to output sub-newton iteration information");
  params.addParam<PostprocessorName>("output", "", "The reporting postprocessor to use for the max_iterations value.");

  // params.addParam<FunctionName>("burnup_function", "Burnup function");

  params.addParam<bool>("calc_elastic_modulus", false, "Flag for using MaterialUC to compute Young's modulus and Poisson's ratio. If false, they will be calculated using the values given in the input file.");
  params.addParam<bool>("model_creep", true, "Flag for modeling creep");

  params.addCoupledVar("fission_rate", 0, "Coupled fission rate");
  params.addCoupledVar("porosity", 0, "Coupled porosity");

  return params;
}


CreepUC::CreepUC( const std::string & name,
                    InputParameters parameters )
  :SolidModel( name, parameters ),

   _relative_tolerance(parameters.get<Real>("relative_tolerance")),
   _absolute_tolerance(parameters.get<Real>("absolute_tolerance")),
   _max_its(parameters.get<unsigned int>("max_its")),
   _output_iteration_info(getParam<bool>("output_iteration_info")),
   _output( getParam<PostprocessorName>("output") != "" ? &getPostprocessorValue("output") : NULL ),

   // _burnup_function( isParamValid("burnup_function") ?
   //                   dynamic_cast<BurnupFunction*>(&getFunction("burnup_function")) : NULL ),

   _creep_strain(declareProperty<SymmTensor>("creep_strain")),
   _creep_strain_old(declarePropertyOld<SymmTensor>("creep_strain")),

   _a1(1.0e-37), // Irradiation creep constant
   _a2(4.14), // Thermal creep constant
   _q(63200),    // Thermal activation constant, [K]


   _calc_elastic_modulus(parameters.get<bool>("calc_elastic_modulus")),
   _model_creep(parameters.get<bool>("model_creep")),

   // _has_fission_rate(isCoupled("fission_rate")),
   _fission_rate(coupledValue("fission_rate")),
   _porosity(coupledValue("porosity"))
{

  // if (_has_fission_rate && _burnup_function)
  //   mooseError("CreepUC: Cannot specify burnup_function with either fission_rate or burnup");
}

void
CreepUC::computeStress()
{
  if ( _model_creep )
  {
  // Define coefficients
    // const Real fission_rate = _has_fission_rate ? _fission_rate[_qp] : _burnup_function->fissionRate(_q_point[_qp]);
    const Real fission_rate = _fission_rate[_qp];

    const Real _c1 = _a1 * fission_rate; // irradiation term
    const Real _c2 = _a2;                    // thermal term
    const Real _activation_term = std::exp( -_q / _temperature[_qp] );


    // Given the stretching, compute the stress increment and add it to the old stress. Also update the creep strain
    // stress = stressOld + stressIncrement
    // creep_strain = creep_strainOld + creep_strainIncrement

    if(_t_step == 0) return;

  // compute trial stress
    SymmTensor stress_new( *elasticityTensor() *_strain_increment );
    stress_new += _stress_old;

  // compute deviatoric trial stress
    SymmTensor dev_trial_stress(stress_new);
    dev_trial_stress.addDiag( -stress_new.trace()/3.0 );

  // compute effective trial stress
    Real dts_squared = dev_trial_stress.doubleContraction(dev_trial_stress);
    Real effective_trial_stress = std::sqrt(1.5 * dts_squared);

  // Use Newton sub-iteration to determine effective creep strain increment
    unsigned int it = 0;
    Real creep_residual = 10.;


    Real del_p = 0.;
    Real norm_residual = 10.;
    Real first_norm_residual = 10.;

    Real phi(0);
    Real dphi_ddelp(0);

    while(it < _max_its && norm_residual > _absolute_tolerance && (norm_residual/first_norm_residual) > _relative_tolerance)
    {

      const Real stress_delta = effective_trial_stress - 3.*_shear_modulus*del_p;

      // report info and abort if effective stress becomes negative
      if(stress_delta < 0)
      {
        Moose::err
          << " it = " << it << std::endl
          << " dt = " << _dt << std::endl
          << " temperature = " << _temperature[_qp] << std::endl
          << " trial stress = " << effective_trial_stress << std::endl
          << " del_p = " << del_p
          << std::endl;
          mooseError("Effective stress in CreepUC is negative!");
      }
      // edot = A1*Fdot*sigma + A2*sigma^2.44*exp(-Q/T)
      phi = stress_delta * _c1 + _c2 * std::pow(stress_delta, 2.44) * _activation_term;
      dphi_ddelp = -3. * _shear_modulus * (_c1 + 2.44 * _c2 * std::pow(stress_delta, 1.44) * _activation_term);

      creep_residual = phi -  del_p/_dt;
      norm_residual = std::abs(creep_residual);
      if(it==0) first_norm_residual = norm_residual;

      del_p = del_p + (creep_residual / (1/_dt - dphi_ddelp));


      // iteration output
      if (_output_iteration_info == true)
      {
        Moose::out
          <<" it=" <<it
          <<" dt=" <<_dt
          <<" temperature=" << _temperature[_qp]
          <<" trial stress=" <<effective_trial_stress
          <<" phi=" <<phi
          <<" dphi=" <<dphi_ddelp
          <<" creep_residual=" <<norm_residual
          <<" del_p=" <<del_p
          <<" relative tolerance=" << _relative_tolerance
          <<" absolute tolerance=" << _absolute_tolerance
          <<"\n";
      }

      it++;
    }

    // report info if iteration fails
    if(it == _max_its && (norm_residual/first_norm_residual) > _relative_tolerance && norm_residual > _absolute_tolerance)
    {
      Moose::err
        << " it = " << it << std::endl
        << " dt = " << _dt << std::endl
        << " temperature = " << _temperature[_qp] << std::endl
        << " trial stress = " << effective_trial_stress << std::endl
        << " phi = " << phi << std::endl
        << " dphi = " << dphi_ddelp << std::endl
        << " creep_residual = " << norm_residual << std::endl
        << " del_p = " << del_p << std::endl
        <<" relative tolerance=" << _relative_tolerance
        <<" absolute tolerance=" << _absolute_tolerance
        << std::endl;
      mooseError("Max sub-newton iteration hit during creep solve!");
    }

  // compute creep and elastic strain increments (avoid potential divide by zero - how should this be done)?
    if (effective_trial_stress < 0.01) effective_trial_stress = 0.01;
    SymmTensor creep_strain_increment(dev_trial_stress);
    creep_strain_increment *= (1.5*del_p/effective_trial_stress);

    _strain_increment -= creep_strain_increment;

  // update stress and creep strain
  // compute stress increment
    _stress[_qp] =  *elasticityTensor() * _strain_increment;

    _stress[_qp] += _stress_old;
    _creep_strain[_qp] = creep_strain_increment;
    _creep_strain[_qp] += _creep_strain_old[_qp];
  }
  else
  {
    if(_t_step == 0)
    {
      return;
    }
    SymmTensor stress_new( (*elasticityTensor()) * _strain_increment );
    _stress[_qp] =  stress_new;
    _stress[_qp] += _stress_old;
  }
}


bool
CreepUC::updateElasticityTensor(SymmElasticityTensor & tensor)
{
  bool changed(false);
  if ( _calc_elastic_modulus )
  {
    SymmIsotropicElasticityTensor * t = dynamic_cast<SymmIsotropicElasticityTensor*>(&tensor);
    if (!t)
      mooseError("Cannot use Youngs modulus or Poissons ratio functions");
    t->unsetConstants();
    // Point p;

    Real porosity = _porosity[_qp];
    Real YM(0.0);
    Real PR(0.0);

    YM = UCMechanical::YOUNGS(_temperature[_qp], porosity );
    PR = UCMechanical::POISSONS(porosity);
    
    // std::cout << "temp: " << _temperature[_qp] << " por: " << porosity << " YM: " << YM << " PR: " << PR << std::endl;

    t->constant(false);
    t->setYoungsModulus(YM);
    t->setPoissonsRatio(PR);

    changed = true;
  }
  return changed;
}

// void
// CreepUC::applyThermalStrain()
// {
//   if (_calc_alpha)
//   {
//     if( _has_temp && _t_step != 0)
//     {
//       Real alpha(0.0), alpha0(0.0), alpha_bar(0.0);
//       // Real tStrain(0.0);
//       const Real temp(_temperature[_qp]);
//       Real temp0(_temperature_old[_qp]);
//       if (_t_step == 1 && _has_stress_free_temp)
//       {
//         temp0 = _stress_free_temp;
//       }

//       if (temp < 0)
//         mooseError("MechUC: Negative temperature on block.");

//       alpha     = UCMechanical::FALPHA(temp);
//       alpha0    = UCMechanical::FALPHA(temp0);
//       alpha_bar = ( alpha + alpha0 )/2.0;

//       _strain_increment.addDiag( -alpha_bar * ( temp - temp0 ) );

//       _d_strain_dT.zero();
//       _d_strain_dT.addDiag(-alpha_bar);
//     }
//   }
//   else
//   {
//     SolidModel::applyThermalStrain();
//   }
// }

////////////////////////////////////////////////////////////////////////

void
CreepUC::createElasticityTensor()
{

  if ( _calc_elastic_modulus ) // need this because youngs modulus is not constant
  {
    SymmIsotropicElasticityTensor * iso = new SymmIsotropicElasticityTensor(false);
    mooseAssert(_youngs_modulus_set, "Internal error:  Youngs modulus not set");
    mooseAssert(_poissons_ratio_set, "Internal error:  Poissons ratio not set");
    iso->setYoungsModulus( _youngs_modulus );
    iso->setPoissonsRatio( _poissons_ratio );
    iso->calculate(0);
    elasticityTensor( iso );
  }
  else
  {
    SolidModel::createElasticityTensor();
  }
}
