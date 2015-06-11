#ifndef KNOCKOUTRATEPOSTPROCESSOR_H
#define KNOCKOUTRATEPOSTPROCESSOR_H

#include "ElementAverageValue.h"

class KnockoutRatePostprocessor: public ElementAverageValue
{

public:
  KnockoutRatePostprocessor(const std::string & name, InputParameters parameters);

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
InputParameters validParams<KnockoutRatePostprocessor>();

#endif
