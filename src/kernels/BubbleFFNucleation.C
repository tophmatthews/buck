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

  return params;
}


BubbleFFNucleation::BubbleFFNucleation(const std::string & name, InputParameters parameters)
  :BubbleBase(name,parameters),
  _frd(coupledValue("fission_rate")),
  _factor(getParam<Real>("factor")),
  _num(getParam<int>("number")),
  _size(getParam<int>("size"))
{
}


void
BubbleFFNucleation::calcLosses(Real & losses, bool jac)
{
  if (_g != 0)
    return;
  if (jac)
    return;
  if ( (*_c[0])[_qp] < 1e7 )
  {
    if ( (*_c[_size-1])[_qp] == 0 )
      return;
    else if ( (*_c[0])[_qp] < 1e6 )
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
  if ( (*_c[0])[_qp] < 1e7 )
  {
    if ( (*_c[_size-1])[_qp] == 0 )
      return;
    else if ( (*_c[0])[_qp] < 1e6 )
      return;
  }

  gains += _factor * _num * _frd[_qp];
}
