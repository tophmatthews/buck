#include "Knockout.h"

#include "BuckUtils.h"

template<>
InputParameters validParams<Knockout>()
{
  InputParameters params = validParams<Kernel>();

  params.addRequiredCoupledVar("coupled_conc", "List of coupled concentration variables.");
  params.addRequiredParam<std::vector<Real> >("coupled_conc_atoms", "List of coupled concentration variables.");
  params.addCoupledVar("fission_rate", 1, "Fission rate");
  params.addParam<Real>("parameter", 1, "Knockout rate.");
  params.addRequiredParam<int>("g", "Group number");

  return params;
}

Knockout::Knockout(const std::string & name, InputParameters parameters) :
    Kernel(name, parameters),
    _atoms(getParam<std::vector<Real> >("coupled_conc_atoms")),
    _fsn_rate(coupledValue("fission_rate")),
    _parameter(getParam<Real>("parameter")),
    _g(getParam<int>("g"))
{
  _G = coupledComponents("coupled_conc");

  for ( int g=0; g<_G; ++g)
    _vals.push_back( &coupledValue("coupled_conc", g) );
}

Real
Knockout::computeQpResidual()
{
  if ( _g == 1 )
  {
    Real gains(0);
    for ( int g=1; g<_G; ++g )
      gains += _parameter * _fsn_rate[_qp] * (*_vals[g])[_qp] * _atoms[g];

    return - _test[_i][_qp] * gains;
  }

  else
    return _test[_i][_qp] * _parameter * _fsn_rate[_qp] * _u[_qp];
}

Real
Knockout::computeQpJacobian()
{
  if ( _g != 1 )
    return _test[_i][_qp] * _parameter * _fsn_rate[_qp] * _phi[_j][_qp];
  else
    return 0;
}
