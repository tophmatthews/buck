#include "HomNucleation.h"

#include "MooseException.h"

template<>
InputParameters validParams<HomNucleation>()
{
  InputParameters params = validParams<Kernel>();

  params.addRequiredCoupledVar("nucleation_conc_vars", "List of concentration variables for nucleation model");
  params.addRequiredParam<int>("m", "Number of atoms in cluster");

  return params;
}

HomNucleation::HomNucleation(const std::string & name, InputParameters parameters)
  :Kernel(name,parameters),
  _rx_rates(getMaterialProperty<std::vector<std::vector<Real> > >("rx_rates")),
  _m(getParam<int>("m"))

{
  _N = coupledComponents("nucleation_conc_vars");

  _vars.resize(_N);
  _vals.resize(_N);

  for ( int i=0; i<_N; ++i)
  {
    _vars[i] = coupled("nucleation_conc_vars", i);
    _vals[i] = &coupledValue("nucleation_conc_vars", i);
  }
}

Real HomNucleation::computeQpResidual()
{
  Real losses(0);
  Real gains(0);

  // losses due to this cluster type moving
  for ( int i=0; i<_N; ++i )
  {
    if ( _m-1 != i ) // protect against self interaction
      losses -= _rx_rates[_qp][_m-1][i] * (*_vals[i])[_qp];
  }

  // Losses due to other cluster types moving
  for ( int i=0; i<_N; ++i )
  {
    if ( _m-1 != i ) // protect against self interaction
      losses -= _rx_rates[_qp][i][_m-1] * (*_vals[i])[_qp];
  }

  // Self combination loss. Factor of 2 needed since two points are lost.
  losses -= 2 * _rx_rates[_qp][_m-1][_m-1] * _u[_qp];

  losses *= _u[_qp]; 

  // Gains from smaller clusters joining
  for ( int i=0; i<_m-1; ++i ) // will not iterate if _m=1
  {
    for ( int j=0; j<_m-1; ++j)
    {
      if ( (i+j+2) == _m ) // If the combination of two bubble sizes results in current bubble size
      {
        gains += _rx_rates[_qp][i][j] * (*_vals[i])[_qp] * (*_vals[j])[_qp];
      }
    }
  }

  return -(losses + gains) * _test[_i][_qp];
}

Real HomNucleation::computeQpJacobian()
{
  Real losses(0);
  // Real gains(0); // gains are independent of u

  // losses due to this cluster type moving
  for ( int i=0; i<_N; ++i )
  {
    if ( _m-1 != i ) // protect against self interaction
      losses -= _rx_rates[_qp][_m-1][i] * (*_vals[i])[_qp];
  }

  // Losses due to other cluster types moving
  for ( int i=0; i<_N; ++i )
  {
    if ( _m-1 != i ) // protect against self interaction
      losses -= _rx_rates[_qp][i][_m-1] * (*_vals[i])[_qp];
  }

  // Self combination loss. 2 is to signify two points are lost. *2 for derivative if u**2
  losses -= 4 * _rx_rates[_qp][_m-1][_m-1] * _u[_qp];

  losses *= _phi[_j][_qp]; 

  return -losses * _test[_i][_qp];
}
