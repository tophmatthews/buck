#include "SwellingPostprocessor.h"

template<>
InputParameters validParams<SwellingPostprocessor>()
{
  InputParameters params = validParams<ElementAverageValue>();

  params.addRequiredCoupledVar("r", "radius variable.");
  params.addRequiredParam<Real>("width", "width");

  return params;
}


SwellingPostprocessor::SwellingPostprocessor(const std::string & name, InputParameters parameters) :
  ElementAverageValue(name, parameters),
  _r(coupledValue("r")),
  _width(getParam<Real>("width"))
{
}


Real
SwellingPostprocessor::computeQpIntegral()
{
	Real swell(0);

	swell += _u[_qp] * 4.0/3.0 * M_PI * std::pow( _r[_qp], 3.0 );

  return swell;
}
