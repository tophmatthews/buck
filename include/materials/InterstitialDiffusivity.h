#ifndef INTERSTITIALDIFFUSIVITY_H
#define INTERSTITIALDIFFUSIVITY_H

#include "Material.h"

class InterstitialDiffusivity;

template<>
InputParameters validParams<InterstitialDiffusivity>();

class InterstitialDiffusivity : public Material
{
public:
  InterstitialDiffusivity(const std::string & name,
                         InputParameters parameters);

protected:
  virtual void computeQpProperties();

  VariableValue & _temp;

  Real _D0;
  Real _Q;
  const Real _factor;
  const int _model;
  const Real _kB;

  MaterialProperty<Real> & _interstitial_diffusivity;
};

#endif // INTERSTITIALDIFFUSIVITY_H
