#include "BubbleCoalescence.h"

#include "MaterialXeBubble.h"
#include "BuckUtils.h"

template<>
InputParameters validParams<BubbleCoalescence>()
{
  InputParameters params = validParams<BubbleBase>();

  return params;
}

BubbleCoalescence::BubbleCoalescence(const std::string & name, InputParameters parameters)
  :BubbleBase(name,parameters),
  _Dg(getMaterialProperty<std::vector<Real> >("bubble_diffusivity"))
{
}


/// OLDER


// void
// BubbleCoalescence::calcLosses(Real & losses, bool jac)
// {
//   // if (_g==_G-1) // Don't allow losses if largest bubble size
//   //   return;

//   Real K_gi(0);
// 	for ( unsigned int i=0; i<_G; ++i )
// 	{
//     if ( i!=_g )
//     {
//       Real r_gi = (*_r[_g])[_qp] + (*_r[i])[_qp];
//       Real D_gi = _Dg[_qp][_g] + _Dg[_qp][i];

//       Real factor(1.0);
//       if ( _g == 0)
//         factor = _width[i];

//       if (!jac)
//     		losses += 4.0 * M_PI * r_gi * D_gi * (*_c[i])[_qp] * _u[_qp] * factor;
//       else
//         losses += 4.0 * M_PI * r_gi * D_gi * (*_c[i])[_qp] * factor;

//       // if (!jac)
//       //   losses += 4.0 * M_PI * r_gi * D_gi * (*_c[i])[_qp] * Buck::pow(_u[_qp], factor);
//       // else
//       //   losses += 4.0 * M_PI * r_gi * D_gi * (*_c[i])[_qp] * factor * Buck::pow(_u[_qp], factor-1);
//     }
//     else if ( _g!=0 )
//     {
//       if (!jac)
//         losses +=  8.0 * M_PI * (*_r[_g])[_qp] * _Dg[_qp][_g] * _u[_qp];
//       else
//         losses += 16.0 * M_PI * (*_r[_g])[_qp] * _Dg[_qp][_g] * _u[_qp];
//     }
// 	}
// }


// void
// BubbleCoalescence::calcGains(Real & gains, bool jac)
// {
//    // Don't count gains if single atom bubble or dimer
//   if ( _g==0 || _g==1 )
//     return;
//   if (jac)
//     return;

//   for ( unsigned int i=1; i<_g; ++i )
//   {
//     for ( unsigned int j=0; j<=i; ++j )
//     {
//       Real Ng = _atoms[i] + _atoms[j]; // Determine new bubble atom size
//       // std::cout << "\ncombining i: " << _atoms[i] << " j: " << _atoms[j] << " into Ng: " << Ng << std::endl;

//       if ( Ng >= _atoms[_g-1] )
//       {
//         Real K_ji;
//         Real fk1;
//         Real fk2;

//         Real r_ji = ( (*_r[i])[_qp] + (*_r[j])[_qp] );
//         Real D_ji = _Dg[_qp][i] + _Dg[_qp][j];
//         K_ji = 4.0 * M_PI * r_ji * D_ji * (*_c[i])[_qp] * (*_c[j])[_qp];

//         if (i==j)
//         {
//           K_ji /= 2.0;
//         }

//         if ( Ng < _atoms[_g])
//         {
//           Buck::getPartition(fk1, fk2, Ng, _atoms[_g-1], _atoms[_g]);
//           gains += K_ji * fk2;
//           // std::cout << "big: g: " << _g << " fk1: " << fk1 << " fk2: " << fk2 << std::endl;
//           // std::cout << "\tNg: " << Ng << " min[g-1]: " << _atoms[_g-1] << " min[g]: " << _atoms[_g] << std::endl;
//         }

//         else if ( _g == _G-1 )
//         {
//           gains += K_ji;
//           // std::cout << "all in big: g: " << _g << std::endl;
//         }

//         else if ( Ng < _atoms[_g+1] )
//         {
//           Buck::getPartition(fk1, fk2, Ng, _atoms[_g], _atoms[_g+1]);
//           gains += K_ji * fk1;
//           // std::cout << "small: g: " << _g << " fk1: " << fk1 << " fk2: " << fk2 << std::endl;
//           // std::cout << "\tNg: " << Ng << " min[g]: " << _atoms[_g] << " min[g+1]: " << _atoms[_g+1] << std::endl;
//         }
//       }
//     }
//   }
// }



// by average

void
BubbleCoalescence::calcLosses(Real & losses, bool jac)
{
  // if (_g==_G-1) // Don't allow losses if largest bubble size
  //   return;

  if (_g == 0)
  {
    for ( unsigned int i=0; i<_G; ++i )
    {
      if (!jac)
        losses += 4.0 * M_PI * _Dg[_qp][0] * (*_r[i])[_qp] * (*_c[i])[_qp] * _u[_qp] * _width[i];
      else
        losses += 4.0 * M_PI * _Dg[_qp][0] * (*_r[i])[_qp] * (*_c[i])[_qp] * _width[i];
    }
  }
  else
  {
    if (!jac)
      losses += 4.0 * M_PI * _Dg[_qp][0] * (*_r[_g])[_qp] * (*_c[0])[_qp] * _u[_qp];
    else
      losses += 4.0 * M_PI * _Dg[_qp][0] * (*_r[_g])[_qp] * (*_c[0])[_qp];
  } 
}


void
BubbleCoalescence::calcGains(Real & gains, bool jac)
{
   // Don't count gains if single atom bubble or dimer
  if ( _g==0 || _g==1 )
    return;
  if (jac)
    return;

  gains += 4.0 * M_PI * _Dg[_qp][0] * (*_r[_g-1])[_qp] * (*_c[_g-1])[_qp] * (*_c[0])[_qp];
}