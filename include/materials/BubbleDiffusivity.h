#ifndef BUBBLEDIFFUSIVITY_H
#define BUBBLEDIFFUSIVITY_H

#include "Material.h"

class BubbleDiffusivity;

template<>
InputParameters validParams<BubbleDiffusivity>();

class BubbleDiffusivity : public Material
{
public:
  BubbleDiffusivity(const std::string & name,
                         InputParameters parameters);

protected:
  // virtual void initialSetup();
  virtual void initialize();
  virtual void computeQpProperties();

  std::vector<VariableValue *> _r;
  VariableValue & _temp;

  unsigned int _G;
  Real _D0;
  Real _Q;
  const Real _R;
  const Real _factor;
  const int _model;
  MaterialProperty<Real> & _gas_diffusivity;
  bool _init;

  MaterialProperty<std::vector<Real> > & _bubble_diffusivity;
};

#endif // BUBBLEDIFFUSIVITY_H
