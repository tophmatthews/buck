#ifndef GAINRATEPOSTPROCESSOR_H
#define GAINRATEPOSTPROCESSOR_H

#include "ElementAverageValue.h"

class GainRatePostprocessor: public ElementAverageValue
{

public:
  GainRatePostprocessor(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpIntegral();

private:
	VariableValue & _r;
	VariableValue & _c1;
	MaterialProperty<Real> & _Dg;
	const Real _width;

};

template<>
InputParameters validParams<GainRatePostprocessor>();

#endif
