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

  std::vector<VariableValue *> _c;
  std::vector<Real> _avgsize;
  std::vector<Real> _maxsize;
  std::vector<Real> _jumpsize;
  VariableValue & _temp;

  int _g;
  int _G;
  int _N_nuc;

  MaterialProperty<Real> & _atomic_diffusivity;

};
#endif //SINKGROWTH_H
