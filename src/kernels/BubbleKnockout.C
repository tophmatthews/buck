#include "BubbleKnockout.h"

template<>
InputParameters validParams<BubbleKnockout>()
{
  InputParameters params = validParams<BubbleBase>();

  params.addParam<Real>("factor", 1.0, "User supplied multiplier.");
  params.addCoupledVar("fission_rate", 0, "Variable for fission rate density.");
  params.addParam<Real>("b", -1, "Value to set constant knockout parameter. B is automatically calculated if not given");

  return params;
}


BubbleKnockout::BubbleKnockout(const std::string & name, InputParameters parameters)
  :BubbleBase(name,parameters),
  _factor(getParam<Real>("factor")),
  _fsn_rate_den(coupledValue("fission_rate")),
  _b(getParam<Real>("b"))
{
}


void
BubbleKnockout::calcLosses(Real & losses, bool jac)
{
  if (_g == 0) return;

  Real k = calcKnockoutRate(_g);

  if (!jac)
    losses += k * _u[_qp];
  else
    losses += k;
}


void
BubbleKnockout::calcGains(Real & gains, bool jac)
{
  if (_g == _G-1) return;
  if (jac) return;

  if (_g==0)
  {
    for (int i=1; i<_G; ++i)
    {
      Real k = calcKnockoutRate(i);
      if (i==1)
        k *= 2.0; // needed since split dimer results in two single atoms
      gains += k * (*_c[i])[_qp] * _widths[i-1];
    }
  }
  else
  {
    Real k = calcKnockoutRate(_g+1);
    gains += k * (*_c[_g+1])[_qp];
  }
}


Real
BubbleKnockout::calcKnockoutRate(int i)
{
  Real b = calcB( (*_r[i])[_qp] );
  Real frd = _fsn_rate_den[_qp] * 1.0e18;

  return _factor * b * frd * _atoms[i];
}


Real
BubbleKnockout::calcB(const Real r)
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
