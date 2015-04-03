#ifndef VACANCYDIFFUSIVITY_H
#define VACANCYDIFFUSIVITY_H

#include "Material.h"

class VacancyDiffusivity;

template<>
InputParameters validParams<VacancyDiffusivity>();

class VacancyDiffusivity : public Material
{
public:
  VacancyDiffusivity(const std::string & name,
                         InputParameters parameters);

protected:
  virtual void computeQpProperties();

  VariableValue & _temp;

  Real _D0;
  Real _Q;
  const Real _factor;
  const int _model;
  const Real _kB;

  MaterialProperty<Real> & _vacancy_diffusivity;
};

#endif // VACANCYDIFFUSIVITY_H
