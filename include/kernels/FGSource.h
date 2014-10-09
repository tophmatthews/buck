#ifndef FGSOURCE_H
#define FGSOURCE_H

#include "Kernel.h"

//Forward Declarations
class FGSource;
class Function;

template<>
InputParameters validParams<FGSource>();

class FGSource : public Kernel
{
public:

  FGSource(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  Real _yield;

  bool _has_fission_rate;
  VariableValue  & _fission_rate;
};

#endif
