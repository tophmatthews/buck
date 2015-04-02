#ifndef SWELLINGPOSTPROCESSOR_H
#define SWELLINGPOSTPROCESSOR_H

#include "ElementIntegralVariablePostprocessor.h"

class SwellingPostprocessor: public ElementIntegralVariablePostprocessor
{

public:
  SwellingPostprocessor(const std::string & name, InputParameters parameters);
  virtual ~SwellingPostprocessor() {}

protected:
  virtual Real computeQpIntegral();

private:
	Real _G;
	std::vector<VariableValue *> _c;
	std::vector<VariableValue *> _r;

};

template<>
InputParameters validParams<SwellingPostprocessor>();

#endif // SWELLINGPOSTPROCESSOR_H
