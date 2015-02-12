#include "HomNucleation.h"

#include "MooseException.h"

template<>
InputParameters validParams<HomNucleation>()
{
  InputParameters params = validParams<Kernel>();

  params.addRequiredCoupledVar("nucleation_conc_vars", "List of concentration variables for nucleation model, including c1.");
  params.addRequiredParam<int>("m", "Number of atoms in cluster.");

  return params;
}

HomNucleation::HomNucleation(const std::string & name, InputParameters parameters)
  :Kernel(name,parameters),
  _rx_rates(getMaterialProperty<std::vector<std::vector<Real> > >("rx_rates")),
  _m(getParam<int>("m"))
{
  _N = coupledComponents("nucleation_conc_vars");

  _vals.resize(_N);

  for ( int i=0; i<_N; ++i)
    _vals[i] = &coupledValue("nucleation_conc_vars", i);
}


Real
HomNucleation::computeQpResidual()
{
  Real losses = calcLosses(false);
  Real gains = calcGains();

  return -( gains - losses * _u[_qp] ) * _test[_i][_qp];
}


Real
HomNucleation::computeQpJacobian()
{
  Real losses = calcLosses(true);


  return losses * _phi[_j][_qp] * _test[_i][_qp];
}


Real
HomNucleation::calcLosses( bool jacobian )
{
  Real losses(0);

  // losses due to this cluster type moving
  for ( int i=0; i<_N; ++i )
  {
    if ( _m-1 != i ) // protect against self interaction
      losses += _rx_rates[_qp][_m-1][i] * (*_vals[i])[_qp]; // * _u[_qp] later
  }

  // Losses due to other cluster types moving
  for ( int i=0; i<_N; ++i )
  {
    if ( _m-1 != i ) // protect against self interaction
      losses += _rx_rates[_qp][i][_m-1] * (*_vals[i])[_qp]; // * _u[_qp] later
  }

  Real factor(2.0); // Factor of 2 needed since two points are lost.
  if ( jacobian )
    factor *= 2.0; // Extra factor of 2 needed since derivative of u**2

  // Self combination loss. 
  losses += factor * _rx_rates[_qp][_m-1][_m-1] * _u[_qp]; // * _u[_qp] below

  return losses;
}


Real
HomNucleation::calcGains()
{
  Real gains(0);

  // Gains from smaller clusters joining
  for ( int i=0; i<_m-1; ++i )
  {
    for ( int j=0; j<_m-1; ++j)
    {
      if ( (i+j+2) == _m ) // If the combination of two bubble sizes results in current bubble size
        gains += _rx_rates[_qp][i][j] * (*_vals[i])[_qp] * (*_vals[j])[_qp];
    }
  }
  return gains;
}