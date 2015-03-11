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

  virtual Real calcLosses(bool jac);
  virtual Real calcGains(bool jac);

  std::vector<VariableName> _names;
  const NonlinearVariableName _this_var;
  const unsigned int _s;
  const unsigned int _M;

  std::vector<VariableValue *> _c;
  std::vector<VariableValue *> _r;
  std::vector<Real> _width;
  std::vector<Real> _minsize;
  std::vector<Real> _maxsize;
  std::vector<Real> _avgsize;
  
  unsigned int _G;
  int _g;  
};

#endif //BUBBLEBASE_H
