#ifndef SWELLINGPOSTPROCESSOR_H
#define SWELLINGPOSTPROCESSOR_H

#include "ElementAverageValue.h"

class SwellingPostprocessor: public ElementAverageValue
{
public:
  SwellingPostprocessor(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral();

private:
	VariableValue & _r;
	const Real _width;
};

template<>
InputParameters validParams<SwellingPostprocessor>();

#endif // SWELLINGPOSTPROCESSOR_H
