#include "PositiveDamper.h"

template<>
InputParameters validParams<PositiveDamper>()
{
  InputParameters params = validParams<Damper>();
  params.addRequiredParam<Real>("damping", "Damping coefficient to use if increment results in a negative value");
  return params;
}

PositiveDamper::PositiveDamper(std::string name, InputParameters parameters) :
    Damper(name, parameters),
    _damping(parameters.get<Real>("damping"))
{
}

Real
PositiveDamper::computeQpDamping()
{

  Real delu = _u[_qp] - _u_increment[_qp];
  if ( delu < 0)
    return _damping;

  return 1.0;
}
