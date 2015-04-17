#include "C1LossPostprocessor.h"

template<>
InputParameters validParams<C1LossPostprocessor>()
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


C1LossPostprocessor::C1LossPostprocessor(const std::string & name, InputParameters parameters) :
  ElementAverageValue(name, parameters),
  _r(coupledValue("r")),
  _c1(coupledValue("c1")),
  _frd(coupledValue("fission_rate")),
  _Dg(getMaterialProperty<Real>("gas_diffusivity")),
  _width(getParam<Real>("width")),
  _atoms(getParam<Real>("atoms")),
  _b(getParam<Real>("b")),
  _factor(getParam<Real>("factor"))
{}


Real
C1LossPostprocessor::computeQpIntegral()
{
  Real growth = 4.0 * M_PI * _Dg[_qp] * _r[_qp] * _u[_qp] * _width * _c1[_qp];

  Real k = calcKnockoutRate();
  Real knockout = k * _u[_qp] * _width;

  return std::abs(growth) + std::abs(knockout);
}

Real
C1LossPostprocessor::calcKnockoutRate()
{
  Real b = calcB( _r[_qp] );
  Real frd = _frd[_qp] * 1.0e18;

  return _factor * b * frd * _atoms;
}


Real
C1LossPostprocessor::calcB(const Real r)
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
