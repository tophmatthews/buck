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

  Real _factor;
  VariableValue  & _var;
};

#endif // KNOCKOUT_H
