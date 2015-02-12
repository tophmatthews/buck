#include "BoundedElementAverage.h"

template<>
InputParameters validParams<BoundedElementAverage>()
{
  InputParameters params = validParams<ElementAverageValue>();

	params.addParam<Real>("upper", "The upper bound for the variable");
  params.addParam<Real>("lower", "The lower bound for the variable");

  return params;
}

BoundedElementAverage::BoundedElementAverage(const std::string & name, InputParameters parameters) :
	ElementAverageValue(name, parameters)
{}

void
BoundedElementAverage::execute()
{
  for (_qp=0; _qp<_qrule->n_points(); _qp++)
  {
  	if ( isParamValid("upper") )
  	{
  		if ( _u[_qp] > getParam<Real>("upper") )
  			mooseError("From Postprocessor "<< _name << ": value is greater than upper limit of " << getParam<Real>("upper") );
  	}
    if ( isParamValid("lower") )
  	{
  		if ( _u[_qp] < getParam<Real>("lower") )
  			mooseError("From Postprocessor "<< _name << ": value is lower than lower limit of " << getParam<Real>("lower") );
  	}
  }
  ElementAverageValue::execute();
}
