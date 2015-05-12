#include "KnockoutRatePostprocessor.h"

template<>
InputParameters validParams<KnockoutRatePostprocessor>()
{
  InputParameters params = validParams<ElementAverageValue>();

  params.addRequiredCoupledVar("r", "radius variable.");
  params.addRequiredCoupledVar("c1", "c1");
  params.addRequiredCoupledVar("fission_rate", "fission rate density");
  params.addRequiredParam<Real>("width", "width");
  params.addRequiredParam<Real>("atoms", "atoms");
  params.addParam<Real>("b", -1, "Value to set constant knockout parameter. B is automatically calculated if not given");
  params.addParam<Real>("factor", 1.0, "Number multiplied scaling variable.");

  return params;
}


KnockoutRatePostprocessor::KnockoutRatePostprocessor(const std::string & name, InputParameters parameters) :
  ElementAverageValue(name, parameters),
  _r(coupledValue("r")),
  _c1(coupledValue("c1")),
  _frd(coupledValue("fission_rate")),
  _Dg(getMaterialProperty<Real>("gas_diffusivity")),
  _width(getParam<Real>("width")),
  _atoms(getParam<Real>("atoms")),
  _b(getParam<Real>("b")),
  _factor(getParam<Real>("factor"))
{
}


Real
KnockoutRatePostprocessor::computeQpIntegral()
{
  Real k = calcKnockoutRate();
  Real knockout = k * _u[_qp] * _width;

  return knockout;
}


Real
KnockoutRatePostprocessor::calcKnockoutRate()
{
  Real b = calcB( _r[_qp] );
  Real frd = _frd[_qp] * 1.0e18;

  return _factor * b * frd * _atoms;
}


Real
KnockoutRatePostprocessor::calcB(const Real r)
{
  if ( _b >= 0 )
    return _b;

  Real a = 0.02831;
  Real b =-0.0803;
  Real c =-0.149;

  Real logr = std::log10(r);
  Real right = a * std::pow( logr, 2.0 ) + b * logr + c;
  return std::pow(10.0, right) * 1e-25;
}
