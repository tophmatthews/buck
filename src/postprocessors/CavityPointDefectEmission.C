#include "CavityPointDefectEmission.h"

#include "MaterialXeBubble.h"

template<>
InputParameters validParams<CavityPointDefectEmission>()
{
  InputParameters params = validParams<CavityPointDefectFlux>();

  params.addRequiredCoupledVar("temp", "Temperature");
  params.addRequiredCoupledVar("bubble_radius", "Bubble radius [cm]");
  params.addRequiredParam<Real>("N", "Number of atoms in bubble");
  params.addRequiredParam<bool>("is_vac", "Flag whether or not is vacancy");
  params.addParam<Real>("gamma", 0.626, "Surface tension [J/m^2]");
  params.addParam<Real>("omega", 4.09e-29, " Atomic volume [m^3]");


  return params;
}

CavityPointDefectEmission::CavityPointDefectEmission(const std::string & name, InputParameters parameters) :
  CavityPointDefectFlux(name, parameters),
  _temp(coupledValue("temp")),
  _rad(coupledValue("bubble_radius")),
  _N(getParam<Real>("N")),
  _is_vac(getParam<bool>("is_vac")),
  _gamma(getParam<Real>("gamma")),
  _omega(getParam<Real>("omega")),

  _kB(1.3806488e-23) // J/K
{
  if (_is_vac)
  {
    _X = &getMaterialProperty<Real>("X_vu");
    _sign = -1.0;
  }
  else
  {
    _X = &getMaterialProperty<Real>("X_iu");
    _sign = 1.0;
  }

}

Real
CavityPointDefectEmission::computeQpIntegral()
{
  Real sigma(0); // TODO: add real sigma
  Real rad_in_meters = _rad[_qp] * 1.0e-6;
  Real bub_p = MaterialXeBubble::VDW_RtoP(rad_in_meters, _temp[_qp], _N);
  Real delP = bub_p - 2.0 * _gamma / rad_in_meters - sigma;

  return _diffusivity[_qp] * (*_X)[_qp] * std::exp(_sign * delP * _omega / _kB / _temp[_qp]);
}
