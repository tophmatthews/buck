#ifndef BUBBLEGROWTH_H
#define BUBBLEGROWTH_H

#include "BubbleBase.h"

class BubbleGrowth;

template<>
InputParameters validParams<BubbleGrowth>();

class BubbleGrowth : public BubbleBase
{

public:
  BubbleGrowth(const std::string & name, InputParameters parameters);

protected:
  virtual void calcLosses(Real & losses, const bool jac);
  virtual void calcGains(Real & gains, const bool jac);

private:
	void calcLossesExperimental(Real & losses, const bool jac);
  void calcGainsExperimental(Real & gains, const bool jac);

	bool _allow_loss;
	MaterialProperty<Real> & _Dg;

  const bool _exp;
};

#endif //BUBBLEGROWTH_H
