#include "AtomicDiffusion.h"

template<>
InputParameters validParams<AtomicDiffusion>()
{
  InputParameters params = validParams<Diffusion>();

  params.addParam<std::string>("diffusion_coef", "atomic_diffusivity", "Diffusion coefficient (Default: atomic_diffusivity");

  return params;
}

AtomicDiffusion::AtomicDiffusion( const std::string & name, InputParameters parameters ) :
    Diffusion( name, parameters ),
    _atomic_diffusivity(getMaterialProperty<Real>(getParam<std::string>("diffusion_coef")))
{
}

Real
AtomicDiffusion::computeQpResidual()
{
  return _atomic_diffusivity[_qp] * Diffusion::computeQpResidual();
}

Real
AtomicDiffusion::computeQpJacobian()
{
  return _atomic_diffusivity[_qp] * Diffusion::computeQpJacobian();
}
