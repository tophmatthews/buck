#include "PointDefect.h"

#include "MaterialXeBubble.h"

template<>
InputParameters validParams<PointDefect>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredParam<bool>("is_vac", "Flag for vacancies=true or intersitial=false point defect");
  params.addRequiredParam<int>("fueltype", "Fuel type, (1=uc or 2=uo2)");
  params.addRequiredCoupledVar("coupled_defect", "The other coupled defect varible");
  params.addRequiredCoupledVar("temp", "Temperature");
  params.addRequiredCoupledVar("dislocation_density", "Dislocation density [cm^-2]");
  params.addRequiredCoupledVar("bubble_concentration", "Bubble concentration [cm^-3]");
  params.addRequiredCoupledVar("bubble_radius", "Bubble radius [cm]");
  params.addRequiredCoupledVar("bubble_atoms", "Number of atoms in bubble [cm]");
  params.addRequiredParam<Real>("grain_diameter", "Grain diameter [cm]");
  params.addParam<Real>("gamma", 0.626, "Surface tension [J/m^2]");
  params.addParam<Real>("omega", 4.09e-29, " Atomic volume [m^3]");

  return params;
}

PointDefect::PointDefect(const std::string & name, InputParameters parameters) :
  Kernel(name, parameters),
  _is_vac(getParam<bool>("is_vac")),
  _fueltype(getParam<int>("fueltype")),
  _o(coupledValue("coupled_defect")),
  _temp(coupledValue("temp")),
  _rho_d(coupledValue("dislocation_density")),
  _Nc(coupledValue("bubble_concentration")),
  _Rc(coupledValue("bubble_radius")),
  _Mc(coupledValue("bubble_atoms")),
  _d(getParam<Real>("grain_diameter")),
  _gamma(getParam<Real>("gamma")),
  _omega(getParam<Real>("omega")),

  _Dv(getMaterialProperty<Real>("vacancy_diffusivity")),
  _Di(getMaterialProperty<Real>("interstitial_diffusivity")),

  // _X_vo(getMaterialProperty<Real>("X_vo")),
  // _X_io(getMaterialProperty<Real>("X_io")),
  _X_vu(getMaterialProperty<Real>("X_vu")),
  _X_iu(getMaterialProperty<Real>("X_iu")),

  _kB(1.3806488e-23), // J/K

  _Zi(1.02), // interstitial dislocation bias factor
  _Zv(1.00), // vacancy dislocation bias factor

  _g(1.0e8) // [um^2] recombination coefficient ratio to Di
{
}


// void
// PointDefect::initialSetup()
// {
//   _u[_qp] =
// }

void
PointDefect::calcRates()
{
  // First set parameters for this variable type (vac or int)
  Real D; // diffusivity
  Real Z; // dislocation bias factor
  Real X_th; // thermal equilibrium concentration
  Real sign; // Sign flip for emission from bubbles

  if (_is_vac)
  {
    D = _Dv[_qp];
    Z = _Zv;
    X_th = _X_vu[_qp];
    sign = -1.0;
  }
  else
  {
    D = _Di[_qp];
    Z = _Zi;
    X_th = _X_iu[_qp];
    sign = 1.0;
  }

  // Calculate time constants
  Real l_d   = _rho_d[_qp] * D * Z;                     // Time constant for dislocaitons
  Real l_c   = 4.0 * M_PI * _Nc[_qp] * _Rc[_qp] * D;    // Time constant for cavities
  Real l_gb  = 6.0 / _d * std::sqrt( D * (l_d + l_c) ); // Time constant for grain boundaries
  Real l_tot = l_d + l_c + l_gb;                        // Total time constant

  // Calculate removal rate
  Real alpha = _Di[_qp] * _g;             // recombination coefficient
  Real R_r   = alpha * _u[_qp] * _o[_qp]; // removal rate from recombination
  Real R_s   = l_tot * _u[_qp];           // removal rate from sinks

  _R_tot  = R_s + R_r;                // total removal rate
  _dR_tot = alpha * _o[_qp] + l_tot;  // total removal rate jacobian

  // Calculate Emission rate from sinks
  Real sigma(0); // TODO: change to actual hydrostatic stress

  Real rad_in_meters = _Rc[_qp] * 1.0e-6;

  Real bub_p = MaterialXeBubble::VDW_RtoP(rad_in_meters, _temp[_qp], _Mc[_qp]);
  Real delP  = bub_p - 2.0 * _gamma / rad_in_meters  - sigma;

  Real E_c  = l_c * X_th * std::exp( sign * delP * _omega / _kB / _temp[_qp] ); // Emission from cavities
  Real E_d  = l_d * X_th;                                                       // Emission from dislocations
  Real E_gb = l_d * X_th;                                                       // Emission from grain boundaries
  Real E_fr = alpha * _X_vu[_qp] * _X_iu[_qp];                                  // Emission from thermal Frenekel pair production

  // std::cout << "rad_in_meters: " << rad_in_meters << " bub_p: " << bub_p << " delP: " << delP << " exp: " << std::exp( sign * delP * _omega / _kB / _temp[_qp] ) << " E_c: " << E_c << std::endl;
  // std::cout << "isvac? " << _is_vac
  //           << " R_d: " << l_d * _u[_qp]
  //           << " R_c: " << l_c * _u[_qp]
  //           << " R_gb: " << l_gb * _u[_qp]
  //           << " R_r: " << alpha * _u[_qp] * _o[_qp]
  //           << " E_d: " << l_d * X_th
  //           << " E_c: " << l_c * X_th
  //           << " E_gb: " << l_gb * X_th
  //           << " E_r: " << alpha * _X_vu[_qp] * _X_iu[_qp] << std::endl;

  _E_tot  = E_c + E_d + E_gb + E_fr; // total emission rate
  _dE_tot = 0;                       // total emission rate jacobian
}

Real
PointDefect::computeQpResidual()
{
  calcRates();

  // std::cout << "is vac?: " << _is_vac << " E_tot: " << _E_tot << " R_tot: " << _R_tot << std::endl;

  return -(_E_tot - _R_tot) * _test[_i][_qp];
}

Real
PointDefect::computeQpJacobian()
{
  calcRates();

  return -(_dE_tot - _dR_tot) * _phi[_j][_qp] * _test[_i][_qp];
}
