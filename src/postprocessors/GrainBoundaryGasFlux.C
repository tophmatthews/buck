#include "GrainBoundaryGasFlux.h"

template<>
InputParameters validParams<GrainBoundaryGasFlux>()
{
  InputParameters params = validParams<SideIntegralVariablePostprocessor>();
  return params;
}

GrainBoundaryGasFlux::GrainBoundaryGasFlux(const std::string & name, InputParameters parameters) :
    SideIntegralVariablePostprocessor(name, parameters),
    _diffusivity(getMaterialProperty<Real>("atomic_diffusivity"))
{
}

Real
GrainBoundaryGasFlux::computeQpIntegral()
{
	return -_diffusivity[_qp]*_grad_u[_qp]*_normals[_qp];
}
