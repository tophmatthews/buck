#ifndef SINKGROWTH_H
#define SINKGROWTH_H

#include "Kernel.h"

class SinkGrowth;

template<>
InputParameters validParams<SinkGrowth>();

class SinkGrowth : public Kernel
{
public:
  SinkGrowth(const std::string & name, InputParameters parameters);

protected:

  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  Real calcLosses();
  Real calcGains();

  std::vector<Real> _atoms;
  VariableValue & _temp;
  int _g;
  int _N_nuc;

  MaterialProperty<Real> & _atomic_diffusivity;

  int _G;
  std::vector<VariableValue *> _c;

};
#endif //SINKGROWTH_H
