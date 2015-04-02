#include "BasicDiffusion.h"

template<>
InputParameters validParams<BasicDiffusion>()
{
  InputParameters params = validParams<Diffusion>();

  params.addParam<std::string>("diffusivity", "diffusivity", "Diffusion coefficient (Default: diffusivity");

  return params;
}

BasicDiffusion::BasicDiffusion( const std::string & name, InputParameters parameters ) :
    Diffusion( name, parameters ),
    _diffusivity(getMaterialProperty<Real>(getParam<std::string>("diffusivity")))
{
}

Real
BasicDiffusion::computeQpResidual()
{
  return _diffusivity[_qp] * Diffusion::computeQpResidual();
}

Real
BasicDiffusion::computeQpJacobian()
{
  return _diffusivity[_qp] * Diffusion::computeQpJacobian();
}
