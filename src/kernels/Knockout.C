#include "Knockout.h"

#include "BuckUtils.h"

template<>
InputParameters validParams<Knockout>()
{
  InputParameters params = validParams<Kernel>();

  params.addRequiredCoupledVar("coupled_vars", "List of bubble variables for nucleation model, including c1. Must be consecutive!");
  params.addCoupledVar("fission_rate", 1, "Fission rate");
  params.addParam<Real>("parameter", 1, "Knockout rate.");
  params.addRequiredParam<int>("m", "Number of atoms in cluster");

  return params;
}

Knockout::Knockout(const std::string & name, InputParameters parameters) :
    Kernel(name, parameters),
    _fsn_rate(coupledValue("fission_rate")),
    _parameter(getParam<Real>("parameter")),
    _m(getParam<int>("m"))
{
  _N = coupledComponents("coupled_vars");
  Buck::atomsFromN(_atoms, _N);

  for ( int i=0; i<_N; ++i)
    _vals.push_back( &coupledValue("coupled_vars", i) );  
}

Real
Knockout::computeQpResidual()
{
  if ( _m == 1 )
  {
    Real gains(0);
    for ( int i=1; i<_N; ++i )
      gains += _parameter * _fsn_rate[_qp] * (*_vals[i])[_qp] * _atoms[i];

    return - _test[_i][_qp] * gains;
  }

  else
    return _test[_i][_qp] * _parameter * _fsn_rate[_qp] * _u[_qp];
}

Real
Knockout::computeQpJacobian()
{
  if ( _m != 1 )
    return _test[_i][_qp] * _parameter * _fsn_rate[_qp] * _phi[_j][_qp];
  else
    return 0;
}
