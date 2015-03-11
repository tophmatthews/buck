#ifndef GASATOMDIFFUSIVITY_H
#define GASATOMDIFFUSIVITY_H

#include "Material.h"

class GasAtomDiffusivity;

template<>
InputParameters validParams<GasAtomDiffusivity>();

class GasAtomDiffusivity : public Material
{
public:
  GasAtomDiffusivity(const std::string & name,
                         InputParameters parameters);

protected:
  virtual void computeQpProperties();

  VariableValue & _temp;
  VariableValue & _fission_rate;

  Real _D0;
  Real _Q;
  Real _D0f;
  Real _Qf;
  const Real _R;
  const Real _factor;
  const int _model;
  int _G;

  MaterialProperty<Real> & _gas_diffusivity;
};

#endif // GASATOMDIFFUSIVITY_H
