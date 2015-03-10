#include "SingleCavity.h"

#include "MaterialXeBubble.h"

template<>
InputParameters validParams<SingleCavity>()
{
  InputParameters params = validParams<Kernel>();

  params.addRequiredCoupledVar("temp", "Temperature");
  params.addRequiredCoupledVar("Cv", "Vacancy concentration");
  params.addRequiredCoupledVar("Ci", "Interstitial concentration");
  params.addRequiredParam<Real>("N", "Number of atoms in bubble");
  params.addParam<Real>("gamma", 0.626, "Surface tension [J/m^2]");
  params.addParam<Real>("omega", 4.09e-29, " Atomic volume [m^3]");

  return params;
}

SingleCavity::SingleCavity(const std::string & name, InputParameters parameters) :
  Kernel(name, parameters),
  _temp(coupledValue("temp")),
  _Cv(coupledValue("Cv")),
  _Ci(coupledValue("Ci")),
  _N(getParam<Real>("N")),
  _gamma(getParam<Real>("gamma")),
  _omega(getParam<Real>("omega")),

  _Dv(getMaterialProperty<Real>("vacancy_diffusivity")),
  _Di(getMaterialProperty<Real>("interstitial_diffusivity")),

  // _X_vo(getMaterialProperty<Real>("X_vo")),
  // _X_io(getMaterialProperty<Real>("X_io")),
  _X_vu(getMaterialProperty<Real>("X_vu")),
  _X_iu(getMaterialProperty<Real>("X_iu")),

  _kB(1.3806488e-23) // J/K
{
}

Real
SingleCavity::computeQpResidual()
{
  Real sigma(0); // TODO: change to actual hydrostatic stress

  Real rad_in_meters = _u[_qp] * 1.0e-6;

  Real bub_p = MaterialXeBubble::VDW_RtoP(rad_in_meters, _temp[_qp], _N);
  Real delP = bub_p - 2.0 * _gamma / rad_in_meters - sigma;

  Real vacan = _Dv[_qp] * ( _Cv[_qp] - _X_vu[_qp] * std::exp(-delP * _omega / _kB / _temp[_qp]) );
  Real inter = _Di[_qp] * ( _Ci[_qp] - _X_iu[_qp] * std::exp(+delP * _omega / _kB / _temp[_qp]) );

  // std::cout << "\trad: " << _u[_qp] << " bub_p: " << bub_p << " delP: " << delP << " exp: " << delP * _omega / _kB / _temp[_qp] << " X_vu: " << _X_vu[_qp] << " phi_ve: " << _X_vu[_qp] * std::exp(-delP * _omega / _kB / _temp[_qp]) << std::endl;
  // std::cout << "\trad_in_meters: " << rad_in_meters << " bub_p: " << bub_p << " delP: " << delP << " E_c: " << std::exp(-delP * _omega / _kB / _temp[_qp]) << std::endl;

  // std::cout << "phi_v: "    << _Dv[_qp] * _Cv[_qp] 
  //           << " phi_i: "   << _Di[_qp] * _Ci[_qp] 
  //           << " phi_ve: "  << _Dv[_qp] * _X_vu[_qp] * std::exp(-delP * _omega / _kB / _temp[_qp]) 
  //           << " phi_ie: "  << _Di[_qp] * _X_iu[_qp] * std::exp(+delP * _omega / _kB / _temp[_qp])
  //           << std::endl;

  return -(vacan - inter) / _u[_qp] * _test[_i][_qp];
}

Real
SingleCavity::computeQpJacobian()
{
  return -computeQpResidual() / _u[_qp] * _phi[_j][_qp];
}
