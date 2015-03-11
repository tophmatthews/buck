#ifndef BUBBLEBASE_H
#define BUBBLEBASE_H

#include "Kernel.h"

class BubbleBase;

template<>
InputParameters validParams<BubbleBase>();

class BubbleBase : public Kernel
{
public:
  BubbleBase(const std::string & name, InputParameters parameters);

protected:

  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  virtual void calcLosses(Real & losses, const bool jac){}
  virtual void calcGains(Real & gains, const bool jac){}

  std::vector<VariableName> _names;
  const NonlinearVariableName _this_var;

  std::vector<VariableValue *> _c;
  std::vector<VariableValue *> _r;
  std::vector<Real> _avgsize;

  unsigned int _G;
  int _g;
};

#endif //BUBBLEBASE_H
