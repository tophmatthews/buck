#ifndef HOMNUCLEATION_H
#define HOMNUCLEATION_H

#include "Kernel.h"

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
	Real calcLosses( bool jacobian );
	Real calcGains();

  std::vector<VariableValue *> _vals;
  MaterialProperty<std::vector<std::vector<Real> > > & _rx_rates;

  int _N_nuc;
  int _g;
  std::vector<Real> _atoms;
};
#endif //HOMNUCLEATION_H
