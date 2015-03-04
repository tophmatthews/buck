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

  
  std::vector<VariableName> _names;
  const NonlinearVariableName _this_var;
  VariableValue & _temp;
  const unsigned int _M;
  const Real _a;
  const Real _omega;

  std::vector<VariableValue *> _c;
  std::vector<Real> _width;
  std::vector<Real> _minsize;
  std::vector<Real> _maxsize;
  std::vector<Real> _avgsize;
  
  unsigned int _N;
  int _g;  
  

  MaterialProperty<Real> & _atomic_diffusivity;

};
#endif //SinkGrowth_H
