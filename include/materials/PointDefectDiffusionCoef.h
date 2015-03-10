#ifndef POINTDEFECTDIFFUSIONCOEF_H
#define POINTDEFECTDIFFUSIONCOEF_H

#include "Material.h"

class PointDefectDiffusionCoef;

template<>
InputParameters validParams<PointDefectDiffusionCoef>();

class PointDefectDiffusionCoef : public Material
{
public:
  PointDefectDiffusionCoef(const std::string & name,
                         InputParameters parameters);

protected:
  virtual void computeQpProperties();

  VariableValue & _temp;

  const bool _is_vac;
  Real _D0;
  Real _Q;
  const Real _factor;
  const int _model;
  const Real _kB;

  MaterialProperty<Real> * _diffusivity;
};

#endif // POINTDEFECTDIFFUSIONCOEF_H
