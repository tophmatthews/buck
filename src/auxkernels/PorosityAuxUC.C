#include "PorosityAuxUC.h"
#include "Material.h"

template<>
InputParameters validParams<PorosityAuxUC>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addParam<Real>("initial_porosity", 0.05, "Initial porosity");
  
  return params;
}

PorosityAuxUC::PorosityAuxUC(const std::string & name, InputParameters parameters) : 
    AuxKernel(name, parameters),

    _initial_porosity(getParam<Real>("initial_porosity")),

    _P1_swelling(getMaterialProperty<Real>("P1_swelling")),
    _P2_swelling(getMaterialProperty<Real>("P2_swelling")),
    _P3_swelling(getMaterialProperty<Real>("P3_swelling")),
    _densification(getMaterialProperty<Real>("densification"))
{}

Real
PorosityAuxUC::computeValue()
{
  Real total_swelling = _P1_swelling[_qp] + _P2_swelling[_qp] + _P3_swelling[_qp];
  // std::cout << "P1: " << _P1_swelling[_qp] << " P2: " << _P2_swelling[_qp] << " P3: " << _P3_swelling[_qp] << " initial: " << _initial_porosity << std::endl;
  Real total_porosity = _initial_porosity + total_swelling + _densification[_qp];
  if ( total_porosity < 0. ) total_porosity = 0.;

  return total_porosity;
}
