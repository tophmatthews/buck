#ifndef HOMCLUSTERLOSS_H
#define HOMCLUSTERLOSS_H

#include "Kernel.h"

//Forward Declarations
class HomClusterLoss;

template<>
InputParameters validParams<HomClusterLoss>();

class HomClusterLoss : public Kernel
{
public:
  HomClusterLoss(const std::string & name, InputParameters parameters);

protected:

  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  // virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  const int _var_size;

  VariableValue & _c1;
  MaterialProperty<std::vector<Real> > & _coeffs;
};
#endif //HOMCLUSTERLOSS_H
