#ifndef C1LOSSPOSTPROCESSOR_H
#define C1LOSSPOSTPROCESSOR_H

#include "ElementAverageValue.h"

class C1LossPostprocessor: public ElementAverageValue
{

public:
  C1LossPostprocessor(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral();

private:
	Real calcKnockoutRate();
	Real calcB(const Real r);
	VariableValue & _r;
	VariableValue & _c1;
	VariableValue & _frd;
	const MaterialProperty<Real> & _Dg;
	const Real _width;
	const Real _atoms;
	Real _b;
	const Real _factor;
};

template<>
InputParameters validParams<C1LossPostprocessor>();

#endif
