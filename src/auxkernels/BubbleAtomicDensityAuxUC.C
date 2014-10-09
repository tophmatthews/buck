#include "BubbleAtomicDensityAuxUC.h"

#include "Material.h"

template<>
InputParameters validParams<BubbleAtomicDensityAuxUC>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addParam<Real>("radius", 1, "Set bubble radius [nm]");
  params.addRequiredParam<UserObjectName>("arho_UO", "Specify BubbleAtomicDensityUO");

  params.addCoupledVar("temp", "Coupled Temperature");

  return params;
}

BubbleAtomicDensityAuxUC::BubbleAtomicDensityAuxUC(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),

  _radius(getParam<Real>("radius")),
  _arho_UO(getUserObject<BubbleAtomicDensityUO>("arho_UO")),

  _has_temp(isCoupled("temp")),
  _temp(_has_temp ? coupledValue("temp") : _zero)

{
  if ( !_has_temp  )
    mooseError("From BubbleAtomicDensityAuxUC: temp needed");
}

Real
BubbleAtomicDensityAuxUC::computeValue()
{
  return _arho_UO.calcAtomicDensity( _temp[_qp], _radius );
}
