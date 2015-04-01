#include "VectorBoundsAux.h"
#include "SystemBase.h"
#include "Conversion.h"

template<>
InputParameters validParams<VectorBoundsAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addParam<Real>("upper", "The upper bound for the variable");
  params.addParam<Real>("lower", "The lower bound for the variable");
  params.addRequiredCoupledVar("bounded_variable", "Variables to be bounded");
  return params;
}

VectorBoundsAux::VectorBoundsAux(const std::string & name, InputParameters parameters) :
  AuxKernel(name, parameters),
  _upper_vector(_nl_sys.getVector("upper_bound")),
  _lower_vector(_nl_sys.getVector("lower_bound")),
  _bounded_variable_id(coupled("bounded_variable"))
{
  _N = coupledComponents("bounded_variable");
  _vars.resize(_N);
  for ( int i=0; i<_N; ++i )
    _vars[i] = (coupled("bounded_variable", i));

  if (!isNodal())
    mooseError("VectorBoundsAux must be used on a nodal auxiliary variable!");
  _upper_valid = parameters.isParamValid("upper");
  if (_upper_valid) _upper = getParam<Real>("upper");
  _lower_valid = parameters.isParamValid("lower");
  if (_lower_valid) _lower = getParam<Real>("lower");
}

Real
VectorBoundsAux::computeValue()
{
  for ( int i=0; i<_N; ++i)
  {
    if ( _current_node->n_dofs(_nl_sys.number(), _vars[i] ) > 0)
    {
      // The zero is for the component, this will only work for Lagrange variables!
      dof_id_type dof = _current_node->dof_number( _nl_sys.number(), _vars[i], 0 );
      if (_upper_valid) _upper_vector.set(dof, _upper);
      if (_lower_valid) _lower_vector.set(dof, _lower);
    }
  }

  return 0.0;
}
