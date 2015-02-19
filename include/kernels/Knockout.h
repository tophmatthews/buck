#ifndef KNOCKOUT_H
#define KNOCKOUT_H

#include "Kernel.h"

//Forward Declarations
class Knockout;

template<>
InputParameters validParams<Knockout>();

class Knockout : public Kernel
{
public:

  Knockout(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  std::vector<Real> _atoms;
  std::vector<VariableValue *> _vals;
  VariableValue & _fsn_rate;

  Real _parameter;
  int _g;
  int _G;
};

#endif // KNOCKOUT_H
