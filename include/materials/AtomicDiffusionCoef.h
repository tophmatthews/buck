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
  virtual void computeProperties();

  VariableValue & _temp;

  const Real _D0;
  const Real _Q;
  const Real _k;
  const Real _factor;

  MaterialProperty<Real> & _atomic_diffusivity;
};

#endif // ATOMUCDIFFUSIONCOEF_H
