#ifndef SinkGrowth_H
#define SinkGrowth_H

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
  Real calcLossesForAtoms(bool jac);
  Real calcLossesForBubbles(bool jac);
  Real calcGainsForBubbles(bool jac);

  std::vector<VariableValue *> _c;
  std::vector<VariableName> _names;
  NonlinearVariableName _this_var;
  std::vector<Real> _maxsize;
  std::vector<Real> _minsize;
  std::vector<Real> _jumpsize;
  VariableValue & _temp;

  int _g;
  unsigned int _G;
  unsigned int _N_nuc;

  MaterialProperty<Real> & _atomic_diffusivity;

};
#endif //SinkGrowth_H
