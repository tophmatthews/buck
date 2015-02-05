#include "AtomicDiffusion.h"

template<>
InputParameters validParams<AtomicDiffusion>()
{
  InputParameters params = validParams<Diffusion>();

  return params;
}

AtomicDiffusion::AtomicDiffusion( const std::string & name, InputParameters parameters ) :
    Diffusion( name, parameters ),
    _atomic_diffusivity(getMaterialProperty<Real>("atomic_diffusivity"))
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
