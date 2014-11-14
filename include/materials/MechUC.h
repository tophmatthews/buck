#ifndef MECHUC_H
#define MECHUC_H

#include "SolidModel.h"
#include "CreepUC.h"
#include "VSwellingUC.h"

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

  // bool _model_thermal_expansion;
  bool _model_swelling;
  bool _calc_alpha;
  // bool _model_gas_swelling;
  // bool _model_relocation;

  const std::string _name_swelling_model;
  // const std::string _name_gas_swelling_model;
  // const std::string _name_relocation_model;

  /// Compute the stress (sigma += deltaSigma)

  virtual void computeStress();
  virtual void initQpStatefulProperties();
  virtual void computeThermalStrain();
  virtual void computeSwellingStrain();
  // virtual void computeGasSwellingStrain();
  // virtual void computeRelocationStrain();
  virtual void modifyStrainIncrement();


};

#endif //MECHUC_H_
