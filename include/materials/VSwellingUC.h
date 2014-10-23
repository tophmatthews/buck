#ifndef VSWELLINGUC_H
#define VSWELLINGUC_H

#include "VolumetricModel.h"

// Forward declarations
class VSwellingUC;

template<>
InputParameters validParams<VSwellingUC>();

class VSwellingUC : public VolumetricModel
{
public:
  VSwellingUC( const std::string & name, InputParameters parameters);

  void initStatefulProperties(unsigned n_points);

  virtual void modifyStrain(const unsigned int qp,
                            const Real sclae_factor,
                            SymmTensor & strain_increment,
                            SymmTensor & dstrain_increment_dT);

  static void solidSwelling(const Real solid_swelling_factor,
                            const Real burnup,
                            Real & fract_volumetric,
                            Real & dfract_volumetric_dtemp);

 static void gaseousSwelling(const Real zone,
                             const Real T2,
                             const Real fuel_density,
                             const Real burnup,
                             const Real temperature,
                             Real & fract_volumetric_increment,
                             Real & dfract_volumetric_increment_dtemp);
//
//  static void densification(const Real total_densification,
//                            const Real complete_densification_burnup,
//                            const bool constant_c_d,
//                            const Real temperature,
//                            const Real burnup,
//                            Real & fract_volumetric_increment,
//                            Real & dfract_volumetric_increment_dtemp);

private:
  VariableValue & _burnup;
  VariableValue & _burnup_old;

  VariableValue & _temperature;
  VariableValue & _temperature_old;

  const Real _solid_factor;
  const Real _calc_gas_swell;

  MaterialProperty<Real> & _density;
  MaterialProperty<Real> & _density_old;
  MaterialProperty<Real> & _zone;
  MaterialProperty<Real> & _zone_old;
  MaterialProperty<Real> & _T2;
  MaterialProperty<Real> & _T2_old;

  MaterialProperty<Real> * _solid_swell;
  MaterialProperty<Real> * _solid_swell_old;
  MaterialProperty<Real> * _gas_swell;
  MaterialProperty<Real> * _gas_swell_old;
  MaterialProperty<Real> * _densification;
  MaterialProperty<Real> * _densification_old;

};

#endif //VSWELLINGUC_H
