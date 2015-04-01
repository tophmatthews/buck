#include "BubbleGrowth.h"

#include "MaterialXeBubble.h"
#include "BuckUtils.h"

template<>
InputParameters validParams<BubbleGrowth>()
{
  InputParameters params = validParams<BubbleBase>();

  params.addParam<bool>("allow_loss", false, "Flag to allow losses from the largest bubble group.");

  return params;
}

BubbleGrowth::BubbleGrowth(const std::string & name, InputParameters parameters)
  :BubbleBase(name,parameters),
  _allow_loss(getParam<bool>("allow_loss")),
  _Dg(getMaterialProperty<Real>("gas_diffusivity"))
{
}


void
BubbleGrowth::calcLosses(Real & losses, bool jac)
{
  if ( !_allow_loss && _g==_G-1 ) // Don't allow losses if largest bubble size
    return;

  if (_g == 0)
  {
    for ( unsigned int i=1; i<_G; ++i )
    {
      if (!jac)
        losses += 4.0 * M_PI * _Dg[_qp] * (*_r[i])[_qp] * (*_c[i])[_qp] * _u[_qp] * _width[i];
      else
        losses += 4.0 * M_PI * _Dg[_qp] * (*_r[i])[_qp] * (*_c[i])[_qp] * _width[i];
    }
  }
  else
  {
    if (!jac)
      losses += 4.0 * M_PI * _Dg[_qp] * (*_r[_g])[_qp] * (*_c[0])[_qp] * _u[_qp];
    else
      losses += 4.0 * M_PI * _Dg[_qp] * (*_r[_g])[_qp] * (*_c[0])[_qp];
  } 
}


void
BubbleGrowth::calcGains(Real & gains, bool jac)
{
  if ( _g==0 || _g==1 ) // Don't count gains if single atom bubble or dimer
    return;
  if (jac)
    return;

  gains += 4.0 * M_PI * _Dg[_qp] * (*_r[_g-1])[_qp] * (*_c[_g-1])[_qp] * (*_c[0])[_qp];
}