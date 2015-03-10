#include "VacancyDiffusivity.h"
#include "MooseEnum.h"

template<>
InputParameters validParams<VacancyDiffusivity>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredCoupledVar("temp", "Coupled Temperature");
  params.addParam<Real>("D0", "Diffusion coefficient [um^2/s]");
  params.addParam<Real>("Q", "Activation energy [eV/mol]");
  params.addParam<Real>("factor", 1, "Scaling factor to multiply by diffusivity.");
  params.addParam<int>("model", 1, "Switch for diffusion coefficient model (0=user input, 1=uo2)");

  return params;
}

VacancyDiffusivity::VacancyDiffusivity(const std::string & name, InputParameters parameters) :
  Material(name, parameters),
  _temp(coupledValue("temp")),
  _factor(getParam<Real>("factor")),
  _model(getParam<int>("model")),

  _kB(8.6173324e-5), // Boltzmann constant, [ev/K]

  _vacancy_diffusivity(declareProperty<Real>("vacancy_diffusivity"))

{
   
  if ( _model == 0 )
  {
    if ( !isParamValid("D0") || !isParamValid("Q") )
      mooseError("In VacancyDiffusivity: if model = 0 (user supplied), D0 and Q must also be supplied");
    _D0 = getParam<Real>("D0");
    _Q = getParam<Real>("Q");
  }
  else
  {
    if ( isParamValid("D0") || isParamValid("Q") )
      mooseError("In VacancyDiffusivity: D0 and Q are supplied, model must = 0");

    if ( _model == 1 )
    {
      _D0 = 1.0e5; // um2/s
      _Q = 2.4; // eV
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
      mooseError("In VacancyDiffusivity: Invalid model value given.");
  }
}

void
VacancyDiffusivity::computeQpProperties()
{
  _vacancy_diffusivity[_qp] = _D0 * std::exp( -_Q / _kB / _temp[_qp] ) * _factor;
  // std::cout << "temp: " << _temp[_qp] << " diff: " << _vacancy_diffusivity[_qp] << std::endl;
}
