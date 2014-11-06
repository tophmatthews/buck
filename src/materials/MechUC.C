#include "MechUC.h"
#include "MaterialUC.h"
#include "VSwellingUC.h"
#include <ctype.h>
#include <stdlib.h>

#include "SymmIsotropicElasticityTensor.h"
#include "Element.h"
// using namespace UCMechanical;

template<>
InputParameters validParams<MechUC>()
{
   InputParameters params = validParams<CreepUC>();

   params.addCoupledVar("porosity", 0, "Coupled Porosity");
   
   params.addParam<bool>("model_thermal_expansion", true, "Set true to turn on thermal expansion model and calculate alpha from MechanicalUC. If false, thermal expansion will be calculated using alpha or alpha function given in input file.");
   params.addParam<bool>("model_swelling", true, "Set true to turn on swelling model");
   // params.addParam<bool>("model_gas_swelling", false, "Set true to turn on swelling model");

   params.addParam<bool>("model_creep", true, "Set true to turn on creep model");
   params.addParam<std::string>("name_swelling_model", "VSwellingUC", "name of swelling model");
   // params.addParam<std::string>("name_gas_swelling_model", "VSwellingMX", "name of gaseous swelling model");
   
  params.addParam<bool>("calc_elastic_modulus", false, "Flag for using MaterialUC to compute Young's modulus and Poisson's ratio. If false, they will be calculated using the values given in the input file.");

  return params;
}

MechUC::MechUC( const std::string & name, InputParameters parameters ) :
  CreepUC( name, parameters ),
  _porosity(coupledValue("porosity")),
        
  _model_thermal_expansion(getParam<bool>("model_thermal_expansion")),
  _model_swelling(getParam<bool>("model_swelling")),
  // _model_gas_swelling(getParam<bool>("model_gas_swelling")),
  _model_creep(getParam<bool>("model_creep")),
        
  _name_swelling_model(getParam<std::string>("name_swelling_model")),
  // _name_gas_swelling_model(getParam<std::string>("name_gas_swelling_model")),
        
  _calc_elastic_modulus(getParam<bool>("calc_elastic_modulus"))
{
}

MechUC::~MechUC()
{
}

void
MechUC::computeStress()
{

  if( _model_creep)
  {
    CreepUC::computeStress();
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

void
MechUC::computeThermalStrain()
{
  if( _model_thermal_expansion )
  {
    if( _has_temp && _t_step != 0)
    {
      Real alpha(0.0), alpha0(0.0), alpha_bar(0.0);
      // Real tStrain(0.0);
      const Real temp(_temperature[_qp]);
      Real temp0(_temperature_old[_qp]);
      if (_t_step == 1 && _has_stress_free_temp)
      {
        temp0 = _stress_free_temp;
      }

      if (temp < 0)
        mooseError("MechUC: Negative temperature on block.");

      alpha     = UCMechanical::FALPHA(temp);
      alpha0    = UCMechanical::FALPHA(temp0);
      alpha_bar = ( alpha + alpha0 )/2.0;

      _strain_increment.addDiag( -alpha_bar * ( temp - temp0 ) );

      _d_strain_dT.zero();
      _d_strain_dT.addDiag(-alpha_bar);

    }
  }
  else
  {
    applyThermalStrain();
  }

}

void
MechUC::computeSwellingStrain()
{
  if( _model_swelling )
  {
    const SubdomainID current_block = _current_elem->subdomain_id();
    const std::vector<VolumetricModel*> & vm( _volumetric_models[current_block] );
    bool bFoundSwellingModel(false);

    const Real VoldV0 = element()->volumeRatioOld(_qp);
    for (unsigned int i(0); i < vm.size(); ++i)
    {
      if( vm[i]->name() == _name_swelling_model)
      {
        vm[i]->modifyStrain(_qp, 1/VoldV0, _strain_increment, _d_strain_dT);
        bFoundSwellingModel = true;
      }
    }

    if(!bFoundSwellingModel)
      mooseWarning( "No swelling model block defined in the input file");
  }

}

// void
// MechUC::computeGasSwellingStrain()
// {
//   if( _model_gas_swelling)
//   {
//     const SubdomainID current_block = _current_elem->subdomain_id();
//     const std::vector<VolumetricModel*> & vm( _volumetric_models[current_block] );
//     bool bFoundGasSwellingModel(false);

//     const Real VoldV0 = element()->volumeRatioOld(_qp);
//     for (unsigned int i(0); i < vm.size(); ++i)
//     {
//       if( vm[i]->name() == _name_gas_swelling_model)
//       {
//         vm[i]->modifyStrain(_qp, 1/VoldV0, _strain_increment, _d_strain_dT);
//         bFoundGasSwellingModel = true;
//       }
//     }

//     if(!bFoundGasSwellingModel)
//     {
//       mooseWarning( "No gas swelling model block defined in the input file");
//     }
//   }

// }

void
MechUC::modifyStrainIncrement()
{

  computeThermalStrain();

  computeSwellingStrain();

  // computeGasSwellingStrain();

}

void
MechUC::initQpStatefulProperties()
{
  CreepUC::initQpStatefulProperties();
}

bool
MechUC::updateElasticityTensor(SymmElasticityTensor & tensor)
{ 
  if( _calc_elastic_modulus )
  {
    Real porosity = _porosity[_qp];
    Real YM(0.0);
    Real PR(0.0);
    SymmIsotropicElasticityTensor * t = dynamic_cast<SymmIsotropicElasticityTensor*>(&tensor);
    if (!t)
    {
      mooseError("Cannot use Youngs modulus or Poissons ratio functions");
    }
    t->unsetConstants();

    YM = UCMechanical::YOUNGS(_temperature[_qp], porosity );
    PR = UCMechanical::POISSONS(porosity);
    
    //std::cout << "temp: " << _temperature[_qp] << " por: " << porosity << " YM: " << YM << " PR: " << PR << std::endl;

    t->constant(false);
    t->setYoungsModulus(YM);
    t->setPoissonsRatio(PR);

    bool changed = true;
    return changed;
  }
  else
  {
    return SolidModel::updateElasticityTensor( tensor );
  }

}