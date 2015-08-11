#ifndef BUBBLEKNOCKOUT_H
#define BUBBLEKNOCKOUT_H

#include "BubbleBase.h"

class BubbleKnockout;

template<>
InputParameters validParams<BubbleKnockout>();

class BubbleKnockout : public BubbleBase
{
public:
  BubbleKnockout(const InputParameters & parameters);

protected:
  virtual void calcLosses(Real & losses, const bool jac);
  virtual void calcGains(Real & gains, const bool jac);

private:
	Real calcKnockoutRate(const int i);
	Real calcB(const Real r);

	const Real _factor;
  VariableValue  & _fsn_rate_den;
  Real _b;
};

#endif
