#include "SpeciesDiffusion.h"

namespace {
const std::string diffusivity_name = "diffusivity_property";
}

template<>
InputParameters validParams<SpeciesDiffusion>()
{
  InputParameters params = validParams<Diffusion>();

  params.addRequiredParam<int>("m", "Name of the diffusivity material property");
  return params;
}

SpeciesDiffusion::SpeciesDiffusion( const std::string & name, InputParameters parameters ) :
    Diffusion( name, parameters ),
    _diffusivities(getMaterialProperty<std::vector<Real> >("diffusivities")),
    _m(getParam<int>("m"))
{
}

Real
SpeciesDiffusion::computeQpResidual()
{
  // std::cout << _diffusivities[_qp][_m-1] << " " << std::endl;
  return _diffusivities[_qp][_m-1] * Diffusion::computeQpResidual();
}

Real
SpeciesDiffusion::computeQpJacobian()
{
  return _diffusivities[_qp][_m-1] * Diffusion::computeQpJacobian();
}
