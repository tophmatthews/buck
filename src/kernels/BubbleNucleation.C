#include "BubbleNucleation.h"

#include "BuckUtils.h"

template<>
InputParameters validParams<BubbleNucleation>()
{
  InputParameters params = validParams<BubbleBase>();

  params.addRequiredCoupledVar("temp", "Temperature");
  params.addParam<Real>("a", 5.0e-4, "Lattice Parameter [um]");
  params.addParam<Real>("omega", 4.09e-11, " Atomic volume [um^3]");

  return params;
}

BubbleNucleation::BubbleNucleation(const std::string & name, InputParameters parameters)
  :BubbleBase(name,parameters),
  _temp(coupledValue("temp")),
  _a(getParam<Real>("a")),
  _omega(getParam<Real>("omega")),

  _Dg(getMaterialProperty<Real>("gas_diffusivity")),

  _Z11(84.0)
{
  mooseDoOnce(Buck::iterateAndDisplay("max", _maxsize));
  mooseDoOnce(Buck::iterateAndDisplay("min", _minsize));
  mooseDoOnce(Buck::iterateAndDisplay("width", _width));
  mooseDoOnce(Buck::iterateAndDisplay("avg", _avgsize));
}

Real
BubbleNucleation::calcLosses(bool jac)
{
  if (_g != 0)
    return 0;

  Real P11 = _Z11 * _omega * _Dg[_qp] * _u[_qp] / std::pow(_a, 2.0);

  if (!jac)
    return 2.0 * P11 * _u[_qp];
  else
    return 4.0 * P11;
}


Real
BubbleNucleation::calcGains(bool jac)
{
  if (_g != 1)
    return 0;

  Real P11 = _Z11 * _omega * _Dg[_qp] * std::pow( (*_c[0])[_qp]/_a, 2.0 );

  if (!jac)
    return P11;
  else
    return 0;
}
