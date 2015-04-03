#ifndef SWELLINGPOSTPROCESSOR_H
#define SWELLINGPOSTPROCESSOR_H

#include "ElementAverageValue.h"

class SwellingPostprocessor: public ElementAverageValue
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
	std::vector<Real> _atoms;
	std::vector<Real> _width;

};

template<>
InputParameters validParams<SwellingPostprocessor>();

#endif // SWELLINGPOSTPROCESSOR_H
