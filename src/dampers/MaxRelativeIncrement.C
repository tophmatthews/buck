#include "MaxRelativeIncrement.h"

template<>
InputParameters validParams<MaxRelativeIncrement>()
{
  InputParameters params = validParams<Damper>();
  params.addRequiredParam<Real>("max_relative", "The maximum newton increment for the variable.");
  return params;
}

MaxRelativeIncrement::MaxRelativeIncrement(std::string name, InputParameters parameters) :
    Damper(name, parameters),
    _max_relative(parameters.get<Real>("max_relative"))
{
}

MaxRelativeIncrement::~MaxRelativeIncrement()
{
}

Real
MaxRelativeIncrement::computeQpDamping()
{

  Real relative = _u_increment[_qp] / _u[_qp];
  if (std::abs(relative) > _max_relative)
  {
    return std::abs( _max_relative / relative );
  }

  return 1.0;
}
