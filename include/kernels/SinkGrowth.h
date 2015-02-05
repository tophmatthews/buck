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
  std::vector<Real> parseAtomsFromVars();

private:
  Real calcLosses();
  Real calcGains();

  VariableValue & _temp;
  int _m;
  MaterialProperty<Real> & _atomic_diffusivity;

  int _N;
  std::vector<VariableValue *> _c;
  std::vector<Real> _atoms;
};
#endif //SINKGROWTH_H
