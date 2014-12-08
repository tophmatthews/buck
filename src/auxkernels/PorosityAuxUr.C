#include "PorosityAuxUr.h"
#include "Material.h"

template<>
InputParameters validParams<PorosityAuxUr>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addParam<Real>("initial_porosity", 0.05, "Initial porosity");
  
  return params;
}

PorosityAuxUr::PorosityAuxUr(const std::string & name, InputParameters parameters) : 
    AuxKernel(name, parameters),

    _initial_porosity(getParam<Real>("initial_porosity")),

    _gas_swelling(getMaterialProperty<Real>("gas_swelling")),
    _solid_swelling(getMaterialProperty<Real>("solid_swelling")),
    _densification(getMaterialProperty<Real>("densification"))
{}

Real
PorosityAuxUr::computeValue()
{
  // Solid swelling component is subtracted due to assumption that bubble porosity is above atoms that stay in solution
  Real total_porosity = _initial_porosity + _gas_swelling[_qp] + _densification[_qp] - _solid_swelling[_qp];
  if ( total_porosity < 0. ) total_porosity = 0.;

  return total_porosity;
}
