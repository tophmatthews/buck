#ifndef SINGLECAVITY_H
#define SINGLECAVITY_H

#include "Kernel.h"

class SingleCavity;

template<>
InputParameters validParams<SingleCavity>();

class SingleCavity : public Kernel
{
public:

  SingleCavity(const std::string & name, InputParameters parameters);

protected:

  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:

  VariableValue & _temp;
  VariableValue & _Cv;
  VariableValue & _Ci;

  const Real _N;
  const Real _gamma;
  const Real _omega;

  MaterialProperty<Real> & _Dv;
  MaterialProperty<Real> & _Di;

  // MaterialProperty<Real> & _X_vo;
  // MaterialProperty<Real> & _X_io;
  MaterialProperty<Real> & _X_vu;
  MaterialProperty<Real> & _X_iu;

  const Real _kB;
};

#endif //SINGLECAVITY_H
