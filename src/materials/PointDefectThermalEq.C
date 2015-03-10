#include "PointDefectThermalEq.h"

#include "MooseEnum.h"
#include "RPoly.h"
template<>
InputParameters validParams<PointDefectThermalEq>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredCoupledVar("temp", "Coupled Temperature");
  params.addRequiredParam<int>("fueltype", "Fuel type, (1=uc or 2=uo2)");
  params.addParam<Real>("x", 0, "Stoichiometry deviation");

  return params;
}

PointDefectThermalEq::PointDefectThermalEq(const std::string & name, InputParameters parameters) :
  Material(name, parameters),
  _temp(coupledValue("temp")),
  _fueltype(getParam<int>("fueltype")),
  _x(getParam<Real>("x")),

  _kB(8.6173324e-5), // Boltzmann constant, [ev/K]

  // Fractional concentrations
  _X_vo(declareProperty<Real>("X_vo")),
  _X_io(declareProperty<Real>("X_io")),
  _X_vu(declareProperty<Real>("X_vu")),
  _X_iu(declareProperty<Real>("X_iu"))
{
}

void
PointDefectThermalEq::calcUO2Conc()
{
  Real G_S  = 6.4; // Shottky defect formation energy, [eV]
  Real G_Fo = 3.1; // Oxygen Frenkel defect formation energy, [eV]
  Real G_Fu = 9.5; // Uranium Frenkel defect formation energy, [ev]

  Real K_S  = std::exp(-G_S  / _kB / _temp[_qp]); // Shottky defect creation constant
  Real K_Fo = std::exp(-G_Fo / _kB / _temp[_qp]); // Oxygen Frenekl defect creation constant
  Real K_Fu = std::exp(-G_Fu / _kB / _temp[_qp]); // Uranim Frenkel defect creation constant

  double coeffs[5];     // Coefficients by decreasing order
  double real_roots[4]; // Real roots
  double img_roots[4];  // Imaginary roots (should just be 0's)
  int info[4];          // Info list (unused)

  coeffs[0] = K_Fu / K_S;
  coeffs[1] = 1;
  coeffs[2] = 0.5 * _x;
  coeffs[3] = -0.5 * K_Fo;
  coeffs[4] = -K_S;

  RPoly::rpoly(coeffs, 4, real_roots, img_roots, info);

  _X_vo[_qp] = -1;
  for ( int i=0; i<4; ++i)
  {
    if ( real_roots[i] > 0 )
    {
      if (_X_vo[_qp] == -1)
        _X_vo[_qp] = real_roots[i];
      else
        mooseError("In PointDefect: more than one positive root");
    }
  }
  if (_X_vo[_qp] == -1)
    mooseError("In PointDefect: no positive roots");

  _X_io[_qp] = K_Fo / _X_vo[_qp];
  _X_vu[_qp] = K_S / std::pow(_X_vo[_qp], 2.0);
  _X_iu[_qp] = K_Fu / _X_vu[_qp];
}

void
PointDefectThermalEq::calcUCConc()
{
  mooseError("In PointDefectThermalEq: Fueltype 1 is not implmented yet.");
  //TODO: implement UC thermal concentrations
}

void
PointDefectThermalEq::computeQpProperties()
{
  if (_fueltype == 1)
    calcUCConc();
  else if (_fueltype == 2)
    calcUO2Conc();
  else
    mooseError("in PointDefectThermalEq: Wrong fueltype.");

  // std::cout << "X_vo: " << _X_vo[0] << " X_io: " << _X_io[0] << " X_vu: " << _X_vu[0] << " X_iu: " << _X_iu[0] << std::endl;
}
