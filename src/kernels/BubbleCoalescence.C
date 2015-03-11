#include "BubbleCoalescence.h"

#include "MaterialXeBubble.h"
#include "BuckUtils.h"

template<>
InputParameters validParams<BubbleCoalescence>()
{
  InputParameters params = validParams<BubbleNucleation>();

  return params;
}

BubbleCoalescence::BubbleCoalescence(const std::string & name, InputParameters parameters)
  :BubbleNucleation(name,parameters),
  _Dg(getMaterialProperty<std::vector<Real> >("bubble_diffusivity"))
{
}

Real
BubbleCoalescence::calcLosses(bool jac)
{
  Real K_gi(0);

	for (int i=0; i<_G; ++i)
	{
    if ( i!=_g )
    {
      Real r_gi = ( (*_r[_g])[_qp] + (*_r[i])[_qp] );
      Real D_gi = _Dg[_qp][_g] + _Dg[_qp][i];

  		K_gi += r_gi * D_gi * (*_c[i])[_qp];
    }
    else if (_g!=0)
    {
      if (!jac)
        K_gi += 2.0 * (*_r[_g])[_qp] * _Dg[_qp][_g] * _u[_qp];
      else
        K_gi += 4.0 * (*_r[_g])[_qp] * _Dg[_qp][_g] * _u[_qp];
    }
	}

  if (!jac)
    return 4.0 * M_PI * K_gi * _u[_qp];
  else
    return 4.0 * M_PI * K_gi;
}


Real
BubbleCoalescence::calcGains(bool jac)
{
   // Don't count gains if single atom bubble
  if ( _g==0 )
    return 0;
  if (jac)
    return 0;

  Real gains(0);

  for (int i=1; i<_g; ++i)
  {
    for ( int j=0; j<=i; ++j)
    {
      Real Ng = _minsize[i] + _minsize[j]; // Determine new bubble atom size

      if ( Ng >= _minsize[_g-1] )
      {
        Real K_ji;
        Real fk1;
        Real fk2;

        Real r_ji = ( (*_r[i])[_qp] + (*_r[j])[_qp] );
        Real D_ji = _Dg[_qp][j] + _Dg[_qp][i];
        K_ji = r_ji * D_ji * (*_c[i])[_qp] * (*_c[j])[_qp];

        if (i==j)
          K_ji /= 2.0;

        if ( Ng < _minsize[_g])
        {
          Buck::getPartition(fk1, fk2, Ng, _minsize[_g-1], _minsize[_g]);
          gains += K_ji * fk2;
        }

        else if ( _g == _G-1 )
          gains += K_ji;

        else if ( Ng < _minsize[_g+1] )
        {
          Buck::getPartition(fk1, fk2, Ng, _minsize[_g], _minsize[_g+1]);
          gains += K_ji * fk1;
        }
      }
    }
  }

  return 4.0 * M_PI * gains;
}
