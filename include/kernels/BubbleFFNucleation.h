#ifndef BUBBLEFFNUCLEATION_H
#define BUBBLEFFNUCLEATION_H

#include "BubbleBase.h"

class BubbleFFNucleation;

template<>
InputParameters validParams<BubbleFFNucleation>();

class BubbleFFNucleation : public BubbleBase
{
public:
  BubbleFFNucleation(const InputParameters & parameters);

protected:
  virtual void calcLosses(Real & losses, const bool jac);
  virtual void calcGains(Real & gains, const bool jac);

private:
	VariableValue & _frd;
	const Real _factor;
	const int _num;
	const int _size;
	const Real _upper;
	const Real _lower;
};

#endif
