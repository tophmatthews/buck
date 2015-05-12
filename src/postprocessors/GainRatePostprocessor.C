#include "GainRatePostprocessor.h"

template<>
InputParameters validParams<GainRatePostprocessor>()
{
  InputParameters params = validParams<ElementAverageValue>();

  params.addRequiredCoupledVar("r", "radius variable.");
  params.addRequiredCoupledVar("c1", "c1");
  params.addRequiredParam<Real>("width", "width");

  return params;
}


GainRatePostprocessor::GainRatePostprocessor(const std::string & name, InputParameters parameters) :
  ElementAverageValue(name, parameters),
  _r(coupledValue("r")),
  _c1(coupledValue("c1")),
  _Dg(getMaterialProperty<Real>("gas_diffusivity")),
  _width(getParam<Real>("width"))
{
}


Real
GainRatePostprocessor::computeQpIntegral()
{
  Real growth = 4.0 * M_PI * _Dg[_qp] * _r[_qp] * _u[_qp] * _width * _c1[_qp];
  return growth;
}
