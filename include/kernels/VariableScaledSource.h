#ifndef VARIABLESCALEDSOURCE_H
#define VARIABLESCALEDSOURCE_H

#include "Kernel.h"

//Forward Declarations
class VariableScaledSource;

template<>
InputParameters validParams<VariableScaledSource>();

class VariableScaledSource : public Kernel
{
public:

  VariableScaledSource(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  Real _factor;
  VariableValue  & _var;
};

#endif
