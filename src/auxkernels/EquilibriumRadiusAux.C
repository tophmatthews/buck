#include "EquilibriumRadiusAux.h"

#include "Material.h"
#include "MaterialXeBubble.h"

template<>
InputParameters validParams<EquilibriumRadiusAux>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addRequiredCoupledVar("temp", "Coupled temperature");
  params.addCoupledVar("sigma", 0, "Coupled hydrostatic stress");
  params.addRequiredParam<Real>("m", "Number of atoms");
  params.addParam<Real>("gamma", 0.626e-12, "Surface tension [J/um^2]");
  params.addParam<Real>("B", 8.469e-11, " Atomic volume [um^3]");
  return params;
}

EquilibriumRadiusAux::EquilibriumRadiusAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),
  _temp(coupledValue("temp")),
  _sigma(coupledValue("sigma")),
  _m(getParam<Real>("m")),
  _gamma(getParam<Real>("gamma")),
  _B(getParam<Real>("B"))
{
}

Real
EquilibriumRadiusAux::computeValue()
{
  Real gamma_in_m = _gamma * 1.0e12;
  Real B_in_m = _B * 1.0e-18;
  Real rad_in_m = MaterialXeBubble::VDW_MtoR(_m, _temp[_qp], _sigma[_qp], gamma_in_m, B_in_m, false);
  return rad_in_m * 1.0e6;
}
