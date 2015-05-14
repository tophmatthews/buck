#include "GasAtomDiffusivity.h"
#include "MooseEnum.h"

template<>
InputParameters validParams<GasAtomDiffusivity>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredCoupledVar("temp", "Coupled Temperature");
  params.addCoupledVar("fission_rate", 0, "Coupled fission rate");
  params.addParam<Real>("D0", "Diffusion coefficient [um^2/s]");
  params.addParam<Real>("Q", "Activation energy [J/mol]");
  params.addParam<Real>("D0f", 0, "Fission enhanced diffusion coefficient [um^2/s]");
  params.addParam<Real>("Qf", 0, "Fission enhanced activation energy [J/mol]");
  params.addParam<Real>("R", 8.31446, "Ideal gas constant [J/(K*mo)]");
  params.addParam<Real>("factor", 1, "Scaling factor to multiply by diffusivity.");
  params.addParam<int>("model", 1, "Switch for diffusion coefficient model (0=user input, 1=UC Matzke, 2=UC Madrid, 3=UC Eyre, 4=UC Ronchi, 5=UO2 Griesmeyer");

  return params;
}


GasAtomDiffusivity::GasAtomDiffusivity(const std::string & name, InputParameters parameters) :
  Material(name, parameters),
  _temp(coupledValue("temp")),
  _fission_rate(coupledValue("fission_rate")),
  _R(getParam<Real>("R")),
  _factor(getParam<Real>("factor")),
  _model(getParam<int>("model")),
  _gas_diffusivity(declareProperty<Real>("gas_diffusivity"))
{
  if ( _model == 0 )
  {
    if ( !isParamValid("D0") || !isParamValid("Q") )
      mooseError("In GasAtomDiffusivity: if model = 0 (user supplied), D0 and Q must also be supplied");
    _D0 = getParam<Real>("D0");
    _Q = getParam<Real>("Q");
    _D0f = getParam<Real>("D0f");
    _Qf = getParam<Real>("Qf");
  }
  else
  {
    if ( isParamValid("D0") || isParamValid("Q") )
      mooseError("In GasAtomDiffusivity: D0 and Q are supplied, model must = 0");

    if ( _model == 1 ) // UC Matzke
    {
      _D0 = 0.3e8;
      _Q = 355000.0;
    }
    else if ( _model == 2 ) // UC Madrid
    {
      _D0 = 4.6e5;
      _Q = 326360.0;
    }
    else if ( _model == 3 ) // UC Eyre
    {
      _D0 = 1.66e-1;
      _Q = 221154.0;
    }
    else if ( _model == 4 ) // UC Ronchi
    {
      _D0 = 4.6e7;
      _Q = 328421.0;
      _D0f = 1.3e-9;
    }
    else if ( _model == 5 ) // UO2 Griesmeyer
    {
      _D0 = 2.1e4;
      _Q = 381000.0;
      _D0f = 1.e-4;
      _Qf = 26.36;
    }
    else
      mooseError("In GasAtomDiffusivity: Invalid model value given.");
  }
}


void
GasAtomDiffusivity::computeQpProperties()
{
  Real diff_thermal = _D0 *  std::exp( -_Q  / _R / _temp[_qp] );
  Real diff_fission = _D0f * std::exp( -_Qf / _R / _temp[_qp] ) * _fission_rate[_qp];

  _gas_diffusivity[_qp] = (diff_thermal + diff_fission) * _factor;
}
