#include "FGSource.h"

// MOOSE
#include "Function.h"
#include "MooseException.h"

template<>
InputParameters validParams<FGSource>()
{
  InputParameters params = validParams<Kernel>();

  params.addParam<Real>("yield", 0.25, "Number of fission gas atoms per fission");

  params.addCoupledVar("fission_rate", "Coupled Fission Rate");

  return params;
}

FGSource::FGSource(const std::string & name, InputParameters parameters) :
    Kernel(name, parameters),
    _yield(getParam<Real>("yield")),
    _has_fission_rate(isCoupled("fission_rate")),
    _fission_rate(_has_fission_rate ? coupledValue("fission_rate") : _zero)
{
  if ( !_has_fission_rate  )
    mooseError("From FGSource: fission_rate needed");
}

Real
FGSource::computeQpResidual()
{
  return _test[_i][_qp] * -_yield * _fission_rate[_qp];
}

Real
FGSource::computeQpJacobian()
{
  return 0.0;
}