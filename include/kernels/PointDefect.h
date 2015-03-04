#ifndef POINTDEFECT_H
#define POINTDEFECT_H

#include "Kernel.h"

class PointDefect;

template<>
InputParameters validParams<PointDefect>();

class PointDefect : public Kernel
{
public:

  PointDefect(const std::string & name, InputParameters parameters);

protected:

  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:

  void calcRates();

  const bool _is_vac;
  const int _fueltype;
  VariableValue & _o;
  VariableValue & _temp;
  VariableValue & _rho_d;
  VariableValue & _Nc;
  VariableValue & _Rc;
  VariableValue & _Mc;
  const Real _d;
  const Real _gamma;
  const Real _omega;

  MaterialProperty<Real> & _Dv;
  MaterialProperty<Real> & _Di;

  // MaterialProperty<Real> & _X_vo;
  // MaterialProperty<Real> & _X_io;
  MaterialProperty<Real> & _X_vu;
  MaterialProperty<Real> & _X_iu;

  const Real _kB;
  const Real _Zi;
  const Real _Zv;
  const Real _g;

  Real _G_S;
  Real _G_Fo;
  Real _G_Fu;
  
  Real _E_tot;
  Real _dE_tot;
  Real _R_tot;
  Real _dR_tot;
};

#endif //POINTDEFECT_H
