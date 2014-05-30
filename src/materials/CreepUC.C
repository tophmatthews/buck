#include "CreepUC.h"

#include "SymmIsotropicElasticityTensor.h"

template<>
InputParameters validParams<CreepUC>()
{
  InputParameters params = validParams<SolidModel>();
  
  //  Sub-Newton Iteration control parameters
  params.addParam<Real>("relative_tolerance", 1e-4, "Relative convergence tolerance for sub-newtion iteration");
  params.addParam<Real>("absolute_tolerance", 1e-20, "Absolute convergence tolerance for sub-newtion iteration");
  params.addParam<unsigned int>("max_its", 10, "Maximum number of sub-newton iterations");
  params.addParam<bool>("output_iteration_info", false, "Set true to output sub-newton iteration information");
  params.addParam<PostprocessorName>("output", "", "The reporting postprocessor to use for the max_iterations value.");

  params.addCoupledVar("fission_rate", "Coupled fission rate");

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

   _has_fission_rate(isCoupled("fission_rate")),
   _fission_rate(_has_fission_rate ? coupledValue("fission_rate") : _zero),

   _creep_strain(declareProperty<SymmTensor>("creep_strain")),
   _creep_strain_old(declarePropertyOld<SymmTensor>("creep_strain")),

   _a1(1.0e-37), // Irradiation creep constant
   _a2(9.48e-9), // Thermal creep constant
   _q(63200)     // Thermal activation constant, [K]
{
}

void
CreepUC::computeStress()
{

// Define coefficients
  const Real _c1 = _a1*_fission_rate[_qp]; // irradiation term
  const Real _c2 = _a2;                    // thermal term
  const Real _activation_term = std::exp(-_q/_temperature[_qp]);

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
    {std::cerr
        << " it = " << it << std::endl
        << " dt = " << _dt << std::endl
        << " temperature = " << _temperature[_qp] << std::endl
        << " trial stress = " << effective_trial_stress << std::endl
        << " del_p = " << del_p
        << std::endl;
        mooseError("Effective stress in CreepUO2 is negative!");
    }
    
    phi = stress_delta * _c1 + _c2 * std::pow(stress_delta, 2.44) * _activation_term;
    dphi_ddelp = -3. * _shear_modulus * (_c1 + 2.44 * _c2 * std::pow(stress_delta, 1.44) * _activation_term);
    
    creep_residual = phi -  del_p/_dt;
    norm_residual = std::abs(creep_residual);
    if(it==0) first_norm_residual = norm_residual;

    del_p = del_p + (creep_residual / (1/_dt - dphi_ddelp));


    // iteration output
    if (_output_iteration_info == true)
    {
      std::cout
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
    std::cerr
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

//bool
//CreepUC::updateElasticityTensor(SymmElasticityTensor & tensor)
//{
//  if (_matpro_ym || _matpro_pr || _matpro_alpha)
//  {
//    FORTRAN_CALL(Matpro::phypro).comp = 0; // Plutonium content
//    FORTRAN_CALL(Matpro::phypro).deloxy = 0; // oxygen concentration of cladding
//    FORTRAN_CALL(Matpro::phypro).bu = 9.5e5*_burnup[_qp];
//    FORTRAN_CALL(Matpro::phyprp)();
//  }
//
//  bool changed(false);
//  if (_matpro_ym || _matpro_pr)
//  {
//    SymmIsotropicElasticityTensor * t = dynamic_cast<SymmIsotropicElasticityTensor*>(&tensor);
//    if (!t)
//    {
//      mooseError("Cannot use Youngs modulus or Poissons ratio functions");
//    }
//    t->unsetConstants();
//    Point p;
//    if (_matpro_ym)
//    {
//      Real dens_frac( _density_percent * 0.01 );
//      Real oxy( _oxy_to_metal_ratio );
//      Real po2( _po2_fraction );
//      t->setYoungsModulus( FORTRAN_CALL(Matpro::felmod)(_temperature[_qp], dens_frac, oxy, po2) );
//    }
//    else
//    {
//      t->setYoungsModulus( _youngs_modulus );
//    }
//    if (_matpro_pr)
//    {
//      Real po2(_po2_fraction);
//      t->setPoissonsRatio( FORTRAN_CALL(Matpro::fpoir)(_temperature[_qp], po2));
//    }
//    else
//    {
//      t->setPoissonsRatio( _poissons_ratio );
//    }
//    changed = true;
//  }
//  return changed;
//}

//void
//CreepUC::applyThermalStrain()
//{
//  if (_matpro_alpha)
//  {
//    Real molten_fraction(0);
//    // WARNING: The MATPRO function is based on a reference temperature of 300K.
//    Real tStrain( FORTRAN_CALL(Matpro::fthexp)(_temperature[_qp], molten_fraction) );
//    Real tStrainOld( FORTRAN_CALL(Matpro::fthexp)(_temperature_old[_qp], molten_fraction) );
//    _strain_increment.addDiag( -(tStrain-tStrainOld) );
//    _d_strain_dT.zero();
//    Real delT(_temperature[_qp]-_temperature_old[_qp]);
//    if (delT != 0)
//    {
//      _d_strain_dT.addDiag( -(tStrain-tStrainOld)/delT );
//    }
//  }
//  else
//  {
//    SolidModel::applyThermalStrain();
//  }
//}

////////////////////////////////////////////////////////////////////////

//void
//CreepUC::createElasticityTensor()
//{
//
//  if (_matpro_ym || _matpro_pr)
//  {
//    SymmIsotropicElasticityTensor * iso = new SymmIsotropicElasticityTensor(false);
//    mooseAssert(_youngs_modulus_set, "Internal error:  Youngs modulus not set");
//    mooseAssert(_poissons_ratio_set, "Internal error:  Poissons ratio not set");
//    iso->setYoungsModulus( _youngs_modulus );
//    iso->setPoissonsRatio( _poissons_ratio );
//    iso->calculate(0);
//    elasticityTensor( iso );
//  }
//  else
//  {
//    SolidModel::createElasticityTensor();
//  }
//}
