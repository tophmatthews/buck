#include "BubbleGrowth.h"

template<>
InputParameters validParams<BubbleGrowth>()
{
  InputParameters params = validParams<BubbleBase>();

  params.addParam<bool>("allow_loss", false, "Flag to allow losses from the largest bubble group.");
  params.addParam<bool>("experimental", false, "Flag to use experimental formulations.");

  return params;
}

BubbleGrowth::BubbleGrowth(const InputParameters & parameters)
  :BubbleBase(parameters),
  _allow_loss(getParam<bool>("allow_loss")),
  _Dg(getMaterialProperty<Real>("gas_diffusivity")),
  _exp(getParam<bool>("experimental"))
{
}

void
BubbleGrowth::calcLosses(Real & losses, bool jac)
{
  if (_exp)
  {
    calcLossesExperimental(losses, jac);
    return;
  }

  if ( !_allow_loss && _g==_G-1 ) return;// Don't allow losses if largest bubble size

  if (_g == 0)
  {
    for ( unsigned int i=1; i<_G; ++i )
    {
      if ( !_allow_loss && i==_G-1 ) break;

      Real k = 4.0 * M_PI * _Dg[_qp] * (*_r[i])[_qp] * (*_c[i])[_qp] * _widths[i];
      if (!jac)
        losses += k * _u[_qp];
      else
        losses += k;
    }
  }
  else
  {
    Real k = 4.0 * M_PI * _Dg[_qp] * (*_r[_g])[_qp] * (*_c[0])[_qp];
    if (!jac)
      losses += k * _u[_qp];
    else
      losses += k;
  }
}

void
BubbleGrowth::calcGains(Real & gains, bool jac)
{
  if (_exp)
  {
    calcGainsExperimental(gains, jac);
    return;
  }

  if ( _g==0 || _g==1 ) return; // Don't count gains if single atom bubble or dimer
  if (jac) return;

  gains += 4.0 * M_PI * _Dg[_qp] * (*_r[_g-1])[_qp] * (*_c[_g-1])[_qp] * (*_c[0])[_qp];
}

void
BubbleGrowth::calcLossesExperimental(Real & losses, bool jac)
{
}


void
BubbleGrowth::calcGainsExperimental(Real & gains, bool jac)
{
}
