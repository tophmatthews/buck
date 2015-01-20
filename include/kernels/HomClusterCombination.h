#ifndef HOMCLUSTERCOMBINATION_H
#define HOMCLUSTERCOMBINATION_H

#include "Kernel.h"

//Forward Declarations
class HomClusterCombination;

template<>
InputParameters validParams<HomClusterCombination>();

class HomClusterCombination : public Kernel
{
public:
  HomClusterCombination(const std::string & name, InputParameters parameters);

protected:

  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

private:
  const int _var_size;
  VariableValue & _c1;
  VariableValue & _smaller;
  MaterialProperty<std::vector<Real> > & _coeffs;
};
#endif //HOMCLUSTERCOMBINATION_H
