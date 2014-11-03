#ifndef VSWELLINGUR_H
#define VSWELLINGUR_H

#include "VolumetricModel.h"

// Forward declarations
class VSwellingUr;

template<>
InputParameters validParams<VSwellingUr>();

class VSwellingUr : public VolumetricModel
{
public:
  VSwellingUr( const std::string & name, InputParameters parameters);

  void initStatefulProperties(unsigned n_points);

  virtual void modifyStrain(const unsigned int qp,
                            const Real sclae_factor,
                            SymmTensor & strain_increment,
                            SymmTensor & dstrain_increment_dT);

  virtual void solidSwelling(const Real solid_swelling_factor,
                             const Real burnup,
                             Real & fract_volumetric,
                             Real & dfract_volumetric_dtemp);

  virtual void calcGasSwelling( const Real burnup,
                                const Real burup_old,
                                const Real temp,
                                const Real por,
                                const Real cont_pres,
                                Real & fract_volumetric,
                                Real & dfract_volumetric_dtemp );

  static void densification(const Real total_densification,
                            const Real complete_densification_burnup,
                            const Real burnup,
                            Real & fract_volumetric_increment,
                            Real & dfract_volumetric_increment_dtemp);

private:
  VariableValue & _burnup;
  VariableValue & _burnup_old;

  VariableValue & _temperature;
  VariableValue & _contact_pressure;
  VariableValue & _porosity;
  
  const Real _total_densification;
  const Real _burnup_constant;
  const Real _solid_factor;
  const Real _calc_gas_swell;

  MaterialProperty<Real> * _solid_swelling;
  MaterialProperty<Real> * _gas_swelling;
  MaterialProperty<Real> * _densification;

  MaterialProperty<Real> * _solid_swelling_old;
  MaterialProperty<Real> * _gas_swelling_old;
  MaterialProperty<Real> * _densification_old;
};

#endif //VSWELLINGUR_H
