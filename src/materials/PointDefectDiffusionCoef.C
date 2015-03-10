#include "PointDefectDiffusionCoef.h"
#include "MooseEnum.h"

template<>
InputParameters validParams<PointDefectDiffusionCoef>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredCoupledVar("temp", "Coupled Temperature");

  params.addRequiredParam<bool>("is_vac", "Flag to define vacancy (true) or interstitial (false) diffusion coef.");
  params.addParam<Real>("D0", "Diffusion coefficient [um^2/s]");
  params.addParam<Real>("Q", "Activation energy [eV/mol]");
  params.addParam<Real>("factor", 1, "Scaling factor to multiply by diffusivity.");
  params.addParam<int>("model", 1, "Switch for diffusion coefficient model (0=user input, 1=uo2)");

  return params;
}

PointDefectDiffusionCoef::PointDefectDiffusionCoef(const std::string & name, InputParameters parameters) :
  Material(name, parameters),
  _temp(coupledValue("temp")),
  _is_vac(getParam<bool>("is_vac")),
  _factor(getParam<Real>("factor")),
  _model(getParam<int>("model")),

  _kB(8.6173324e-5) // Boltzmann constant, [ev/K]

{
  if (_is_vac)
    _diffusivity = &declareProperty<Real>("vacancy_diffusivity");
  else
    _diffusivity = &declareProperty<Real>("interstitial_diffusivity");
  if ( _model == 0 )
  {
    if ( !isParamValid("D0") || !isParamValid("Q") )
      mooseError("In PointDefectDiffusionCoef: if model = 0 (user supplied), D0 and Q must also be supplied");
    _D0 = getParam<Real>("D0");
    _Q = getParam<Real>("Q");
  }
  else
  {
    if ( isParamValid("D0") || isParamValid("Q") )
      mooseError("In PointDefectDiffusionCoef: D0 and Q are supplied, model must = 0");

    if ( _model == 1 )
    {
      if (_is_vac)
      {
        _D0 = 1.0e5; // um2/s
        _Q = 2.4; // eV
      }
      else
      {
        _D0 = 7.12e11; //um2/s
        _Q = 1.9; // eV
      }
    }
    // else if ( _model == 2 )
    // {
    //   _D0 = 4.6e11;
    //   _Q = 326360.0;
    // }
    // else if ( _model == 3 )
    // {
    //   _D0 = 1.66e5;
    //   _Q = 221154.0;
    // }
    else
      mooseError("In PointDefectDiffusionCoef: Invalid model value given.");
  }
}

void
PointDefectDiffusionCoef::computeQpProperties()
{
  (*_diffusivity)[_qp] = _D0 * std::exp( -_Q / _kB / _temp[_qp] ) * _factor;
  // std::cout << "is vac?: " << _is_vac << " temp: " << _temp[_qp] << " diff: " << (*_diffusivity)[_qp] << std::endl;
}
