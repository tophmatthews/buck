#ifndef BUBBLENUCLEATION_H
#define BUBBLENUCLEATION_H

#include "BubbleBase.h"

class BubbleNucleation;

template<>
InputParameters validParams<BubbleNucleation>();

class BubbleNucleation : public BubbleBase
{
public:
  BubbleNucleation(const std::string & name, InputParameters parameters);

protected:

  virtual Real calcLosses(bool jac);
  virtual Real calcGains(bool jac);

private:

	VariableValue & _temp;
	const Real _a;
	const Real _omega;

	MaterialProperty<Real> & _atomic_diffusivity;

	const Real _Z11;
};

#endif //BUBBLENUCLEATION_H
