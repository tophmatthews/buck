#ifndef ATOMICDIFFUSIONCOEF_H
#define ATOMICDIFFUSIONCOEF_H

#include "Material.h"

class AtomicDiffusionCoef;

template<>
InputParameters validParams<AtomicDiffusionCoef>();

class AtomicDiffusionCoef : public Material
{
public:
  AtomicDiffusionCoef(const std::string & name,
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

  MaterialProperty<Real> & _atomic_diffusivity;
};

#endif // ATOMUCDIFFUSIONCOEF_H
