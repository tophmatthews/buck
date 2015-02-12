#include "Knockout.h"

template<>
InputParameters validParams<Knockout>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription( "Body source scaled by variable and factor." );

  params.addParam<Real>("factor", 1, "Number multiplied scaling variable.");
  params.addCoupledVar("scaling_variable", 1, "Variable for Scaling");

  return params;
}

Knockout::Knockout(const std::string & name, InputParameters parameters) :
    Kernel(name, parameters),
    _factor(getParam<Real>("factor")),
    _var(coupledValue("scaling_variable"))
{
}

Real
Knockout::computeQpResidual()
{
  return -_test[_i][_qp] * _factor * _var[_qp];
}

Real
Knockout::computeQpJacobian()
{
  return 0.0;
}
