#include "BubbleFFNucleation.h"

#include "BuckUtils.h"

template<>
InputParameters validParams<BubbleFFNucleation>()
{
  InputParameters params = validParams<BubbleBase>();

  params.addRequiredCoupledVar("fission_rate", "Fission Rate Density");
  params.addParam<Real>("factor", 1.0, "User supplied multiplier.");
  params.addParam<int>("number", 5, "Number of bubbles created per fission");
  params.addParam<int>("size", 4, "Size of bubbles created");
  params.addParam<Real>("upper", 1e7, "Upper deadband limit");
  params.addParam<Real>("lower", 1e6, "Lower deadband limit");

  return params;
}


BubbleFFNucleation::BubbleFFNucleation(const InputParameters & parameters)
  :BubbleBase(parameters),
  _frd(coupledValue("fission_rate")),
  _factor(getParam<Real>("factor")),
  _num(getParam<int>("number")),
  _size(getParam<int>("size")),
  _upper(getParam<Real>("upper")),
  _lower(getParam<Real>("lower"))
{
}


void
BubbleFFNucleation::calcLosses(Real & losses, bool jac)
{
  if (_g != 0)
    return;
  if (jac)
    return;
  if ( (*_c[0])[_qp] < _upper )
  {
    if ( (*_c[_size-1])[_qp] == 0 )
      return;
    else if ( (*_c[0])[_qp] < _lower )
      return;
  }

  losses += _factor * _num * _size * _frd[_qp];
}


void
BubbleFFNucleation::calcGains(Real & gains, bool jac)
{
  if ( _g != _size - 1)
    return;
  if (jac)
    return;
  if ( (*_c[0])[_qp] < _upper )
  {
    if ( (*_c[_size-1])[_qp] == 0 )
      return;
    else if ( (*_c[0])[_qp] < _lower )
      return;
  }

  gains += _factor * _num * _frd[_qp];
}
