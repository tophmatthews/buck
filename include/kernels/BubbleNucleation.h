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

  virtual void calcLosses(Real & losses, const bool jac);
  virtual void calcGains(Real & gains, const bool jac);

private:

	VariableValue & _temp;
	const Real _a;
	const Real _omega;
	const Real _factor;

	MaterialProperty<Real> & _Dg;

	const Real _Z11;
};

#endif //BUBBLENUCLEATION_H
