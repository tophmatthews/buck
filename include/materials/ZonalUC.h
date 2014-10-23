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
  virtual void calcT2(unsigned int);

  virtual void initQpStatefulProperties();
  virtual void computeProperties();

  const Real _frac_yield;
  const Real _nitrogen_fraction;
  const bool _oxygen_level;
  const Real _zone3_width;
  const Real _frac_rel_zone1;
  const Real _frac_rel_zone3;
  const Real _frac_rel_zone4;
  const Real _burnup_threshold;

  bool _has_fission_rate;
  VariableValue & _fission_rate;
  VariableValue & _fission_rate_old;

  bool _has_temp;
  VariableValue & _temp;
  VariableValue & _temp_old;

  bool _has_burnup;
  VariableValue & _burnup;
  VariableValue & _burnup_old;

  MaterialProperty<Real> & _T2;

  MaterialProperty<Real> & _zone;
  MaterialProperty<Real> & _gas_gen;
  MaterialProperty<Real> & _gas_rel;

  MaterialProperty<Real> & _zone_old;
  MaterialProperty<Real> & _gas_gen_old;
  MaterialProperty<Real> & _gas_rel_old;


  const Real _R;
  const Real _avogadros_num;

  bool _testing;
};

#endif //ZONALUC_H
