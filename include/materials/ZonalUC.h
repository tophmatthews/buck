#ifndef ZONALUC_H
#define ZONALUC_H

#include "Material.h"

//Forward Declarations
class ZonalUC;

template<>
InputParameters validParams<ZonalUC>();

/**
 * Simple material with constant properties.
 */
class ZonalUC : public Material
{
public:
  ZonalUC(const std::string & name,
                         InputParameters parameters);

protected:
  virtual Real calcTi(unsigned int qp, const int i, const Real burnup);

  virtual void initQpStatefulProperties();
  virtual void computeProperties();

  const Real _frac_yield;
  // const Real _nitrogen_fraction;
  // const bool _oxygen_level;
  const Real _zone3_width;
  const Real _frac_rel_zone1;
  const Real _frac_rel_zone3;
  const Real _frac_rel_zone4;
  const Real _burnup_threshold_rel;
  const Real _burnup_threshold_zone;

  VariableValue & _fission_rate;
  VariableValue & _temp;
  VariableValue & _burnup;

  VariableValue & _fission_rate_old;  
  VariableValue & _temp_old;
  VariableValue & _burnup_old;

  MaterialProperty<Real> & _T2;
  MaterialProperty<Real> & _T3;
  MaterialProperty<Real> & _zone;
  MaterialProperty<Real> & _gas_gen;
  MaterialProperty<Real> & _gas_rel;

  MaterialProperty<Real> & _T2_old;
  MaterialProperty<Real> & _T3_old;
  MaterialProperty<Real> & _zone_old;
  MaterialProperty<Real> & _gas_gen_old;
  MaterialProperty<Real> & _gas_rel_old;


  const Real _R;
  const Real _avogadros_num;

  bool _testing;
};

#endif //ZONALUC_H
