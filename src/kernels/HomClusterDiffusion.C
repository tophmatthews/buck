#include "HomClusterDiffusion.h"

template<>
InputParameters validParams<HomClusterDiffusion>()
{
  InputParameters params = validParams<Diffusion>();

  params.addRequiredParam<int>("m", "Number of atoms in the current concentration variable");
  return params;
}

HomClusterDiffusion::HomClusterDiffusion( const std::string & name, InputParameters parameters ) :
    Diffusion( name, parameters ),
    _diffusivities(getMaterialProperty<std::vector<Real> >("cluster_diffusivities")),
    _m(getParam<int>("m"))
{
}

Real
HomClusterDiffusion::computeQpResidual()
{
  // std::cout << _diffusivities[_qp][_m-1] << " " << std::endl;
  return _diffusivities[_qp][_m-1] * Diffusion::computeQpResidual();
}

Real
HomClusterDiffusion::computeQpJacobian()
{
  return _diffusivities[_qp][_m-1] * Diffusion::computeQpJacobian();
}
