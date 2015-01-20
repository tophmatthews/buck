#ifndef HOMPOINTLOSS_H
#define HOMPOINTLOSS_H

#include "Kernel.h"

//Forward Declarations
class HomPointLoss;

template<>
InputParameters validParams<HomPointLoss>();

class HomPointLoss : public Kernel
{
public:
  HomPointLoss(const std::string & name, InputParameters parameters);

protected:

  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

private:
  std::vector<unsigned int> _vars;
  std::vector<VariableValue *> _vals;
  MaterialProperty<std::vector<Real> > & _coeffs;
  int _n;
};
#endif //HOMPOINTLOSS_H
