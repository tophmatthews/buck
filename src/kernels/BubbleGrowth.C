#include "BubbleGrowth.h"

template<>
InputParameters validParams<BubbleGrowth>()
{
  InputParameters params = validParams<BubbleBase>();

  params.addParam<bool>("allow_loss", false, "Flag to allow losses from the largest bubble group.");
  params.addParam<bool>("experimental", false, "Flag to use experimental formulations.");

  return params;
}

BubbleGrowth::BubbleGrowth(const std::string & name, InputParameters parameters)
  :BubbleBase(name,parameters),
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
  if ( !_allow_loss && _g==_G-1 ) return;// Don't allow losses if largest bubble size

  if (_g == 0)
  {
    // for ( unsigned int i=1; i<_G-1; ++i )
    // {
    //   Real term = 4.0 * M_PI * _Dg[_qp] * (*_r[i])[_qp] * Buck::linEst(_atoms[i], _atoms[i+1], (*_c[i])[_qp], (*_c[i+1])[_qp], _atoms[i+1] - 1.0 ) * _widths[i];
    //   else if (!jac)
    //     losses += term * _u[_qp];
    //   else
    //     losses += term;
    // }
    // if ( _allow_loss )
    // {
    //   else if (!jac)
    //     losses += 4.0 * M_PI * _Dg[_qp] * (*_r[i])[_qp] * (*_c[i])[_qp] * _widths[i] * _u[_qp];
    //   else
    //     losses += 4.0 * M_PI * _Dg[_qp] * (*_r[i])[_qp] * (*_c[i])[_qp] * _widths[i];
    // }
  }
  else
  {
    Real base = 4.0 * M_PI * _Dg[_qp] * (*_r[_g])[_qp] * (*_c[0])[_qp];
    if ( _g!=_G-1 )
    {
      if (!jac)
      {
        // std::cout << _atoms[_g] << " " << _atoms[_g+1] << " " << _u[_qp] << " " <<  (*_c[_g+1])[_qp] << " " << Buck::linEst( _atoms[_g], _atoms[_g+1], _u[_qp], (*_c[_g+1])[_qp], _atoms[_g+1] - 1.0 ) << std::endl;
        losses += base * Buck::linEst( _atoms[_g], _atoms[_g+1], _u[_qp], (*_c[_g+1])[_qp], _atoms[_g+1] - 1.0 );
      }
      else
        losses += base * Buck::dlinEstdLeft( _atoms[_g], _atoms[_g+1], _u[_qp], (*_c[_g+1])[_qp], _atoms[_g+1] - 1.0 );
    }
    else
    {
      if (!jac)
        losses += 4.0 * M_PI * _Dg[_qp] * (*_r[_g])[_qp] * (*_c[0])[_qp] * _u[_qp];
      else
        losses += 4.0 * M_PI * _Dg[_qp] * (*_r[_g])[_qp] * (*_c[0])[_qp];
    }
  }
}


void
BubbleGrowth::calcGainsExperimental(Real & gains, bool jac)
{
  if ( _g==0 || _g==1 ) return; // Don't count gains if single atom bubble or dimer

  Real base = 4.0 * M_PI * _Dg[_qp] * (*_r[_g-1])[_qp] * (*_c[0])[_qp];
  if (!jac)
  {
    // std::cout << _atoms[_g-1] << " " << _atoms[_g] << " " << (*_c[_g-1])[_qp] << " " <<  _u[_qp] << " " << Buck::linEst( _atoms[_g-1], _atoms[_g], (*_c[_g-1])[_qp], _u[_qp], _atoms[_g] - 1.0 ) << std::endl;
    gains += base * Buck::linEst( _atoms[_g-1], _atoms[_g], (*_c[_g-1])[_qp], _u[_qp], _atoms[_g] - 1.0 );
  }
  else
    gains += base * Buck::dlinEstdRight( _atoms[_g-1], _atoms[_g], (*_c[_g-1])[_qp], _u[_qp], _atoms[_g] - 1.0 );
}
