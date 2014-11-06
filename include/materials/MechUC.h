#ifndef MECHUC
#define MECHUC

#include "SolidModel.h"
#include "CreepUC.h"

// Forward declarations
class MechUC;

template<>
InputParameters validParams<MechUC>();

class MechUC : public CreepUC
{
public:
  MechUC( const std::string & name,
                 InputParameters parameters );

  virtual ~MechUC();

protected:

  VariableValue & _porosity;
  
  bool _model_thermal_expansion;
  bool _model_swelling;
  // bool _model_gas_swelling;
  bool _model_creep;

  const std::string _name_swelling_model;
  // const std::string _name_gas_swelling_model;
  
  const bool _calc_elastic_modulus;
    
  /// Compute the stress (sigma += deltaSigma)

  virtual void computeStress();
  virtual void initQpStatefulProperties();
  virtual void computeThermalStrain();
  virtual void computeSwellingStrain();
  // virtual void computeGasSwellingStrain();
  virtual void modifyStrainIncrement();
  virtual bool updateElasticityTensor(SymmElasticityTensor & tensor );

};

#endif //MECHUC
