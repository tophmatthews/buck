#include "FGSource.h"

#include "MooseException.h"

template<>
InputParameters validParams<FGSource>()
{
  InputParameters params = validParams<Kernel>();

  params.addParam<Real>("yield", 0.25, "Number of fission gas atoms per fission");

  params.addCoupledVar("fission_rate", 0, "Coupled Fission Rate");

  return params;
}

FGSource::FGSource(const std::string & name, InputParameters parameters) :
    Kernel(name, parameters),
    _yield(getParam<Real>("yield")),
    _fission_rate(coupledValue("fission_rate"))
{
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