#include "GrainBoundaryGasFlux.h"

template<>
InputParameters validParams<GrainBoundaryGasFlux>()
{
  InputParameters params = validParams<SideIntegralVariablePostprocessor>();
  return params;
}

GrainBoundaryGasFlux::GrainBoundaryGasFlux(const std::string & name, InputParameters parameters) :
    SideIntegralVariablePostprocessor(name, parameters),
    _diffusivities(getMaterialProperty<std::vector<Real> >("diffusivities"))
{
}

Real
GrainBoundaryGasFlux::computeQpIntegral()
{
	return -_diffusivities[_qp][0]*_grad_u[_qp]*_normals[_qp];
}
