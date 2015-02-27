#ifndef OldSinkGrowth_H
#define OldSinkGrowth_H

#include "Kernel.h"

class OldSinkGrowth;

template<>
InputParameters validParams<OldSinkGrowth>();

class OldSinkGrowth : public Kernel
{
public:
  OldSinkGrowth(const std::string & name, InputParameters parameters);

protected:

  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  Real calcLosses();
  Real calcGains();

  std::vector<VariableValue *> _c;
  std::vector<VariableName> _names;
  NonlinearVariableName _this_var;
  std::vector<Real> _minsize;
  std::vector<Real> _avgsize;
  std::vector<Real> _maxsize;
  std::vector<Real> _jumpsize;
  VariableValue & _temp;

  int _g;
  int _G;
  int _N_nuc;

  MaterialProperty<Real> & _atomic_diffusivity;

};
#endif //OldSinkGrowth_H
