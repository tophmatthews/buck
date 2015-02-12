#include "AtomicDiffusionCoef.h"
#include "MooseEnum.h"

template<>
InputParameters validParams<AtomicDiffusionCoef>()
{
  InputParameters params = validParams<Material>();

  params.addCoupledVar("temp", 0, "Coupled Temperature");

  params.addParam<Real>("D0", 1.7e5, "Diffusion coefficient [nm^2/s]");
  params.addParam<Real>("Q", 2.3, "Activation energy [eV]");
  params.addParam<Real>("k", 8.617e-5, "Boltzmann's constant [eV/K]");
  params.addParam<Real>("factor", 1, "Scaling factor to multiply by diffusivity.");

  return params;
}

AtomicDiffusionCoef::AtomicDiffusionCoef(const std::string & name, InputParameters parameters) :
  Material(name, parameters),

  _temp(coupledValue("temp")),

  _D0(getParam<Real>("D0")),
  _Q(getParam<Real>("Q")),
  _k(getParam<Real>("k")),
  _factor(getParam<Real>("factor")),

  _atomic_diffusivity(declareProperty<Real>("atomic_diffusivity"))
{
}

void
AtomicDiffusionCoef::computeProperties()
{
  for( unsigned int qp(0); qp < _qrule->n_points(); ++qp)
    _atomic_diffusivity[qp] = _D0 * std::exp( -_Q / _k / _temp[qp] ) * _factor;
}
