#ifndef POINTDEFECTTHERMALEQ_H
#define POINTDEFECTTHERMALEQ_H

#include "Material.h"

class PointDefectThermalEq;

template<>
InputParameters validParams<PointDefectThermalEq>();

class PointDefectThermalEq : public Material
{
public:
  PointDefectThermalEq(const std::string & name,
                         InputParameters parameters);

protected:
  virtual void computeQpProperties();

  void calcUCConc();
  void calcUO2Conc();

  VariableValue & _temp;
  const int _fueltype;
  const Real _x;

  const Real _kB;

  MaterialProperty<Real> & _X_vo_eq;
  MaterialProperty<Real> & _X_io_eq;
  MaterialProperty<Real> & _X_vu_eq;
  MaterialProperty<Real> & _X_iu_eq;
};

#endif // POINTDEFECTTHERMALEQ_H
