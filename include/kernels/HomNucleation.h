#ifndef HOMNUCLEATION_H
#define HOMNUCLEATION_H

#include "Kernel.h"

//Forward Declarations
class HomNucleation;

template<>
InputParameters validParams<HomNucleation>();

class HomNucleation : public Kernel
{
public:
  HomNucleation(const std::string & name, InputParameters parameters);

protected:

  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

private:
  std::vector<unsigned int> _vars;
  std::vector<VariableValue *> _vals;
  MaterialProperty<std::vector<std::vector<Real> > > & _rx_rates;

  int _N;
  int _m;
};
#endif //HOMNUCLEATION_H
