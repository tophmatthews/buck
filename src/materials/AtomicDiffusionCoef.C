#include "AtomicDiffusionCoef.h"
#include "MooseEnum.h"

template<>
InputParameters validParams<AtomicDiffusionCoef>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredCoupledVar("temp", "Coupled Temperature");

  params.addParam<Real>("D0", "Diffusion coefficient [nm^2/s]");
  params.addParam<Real>("Q", "Activation energy [J/mol]");
  params.addParam<Real>("R", 8.31446, "Ideal gas constant [J/(K*mo)]");
  params.addParam<Real>("factor", 1, "Scaling factor to multiply by diffusivity.");
  params.addParam<int>("model", 1, "Switch for diffusion coefficient model (0=user input, 1=Matzke, 2=Madrid, 3=Eyre");

  return params;
}

AtomicDiffusionCoef::AtomicDiffusionCoef(const std::string & name, InputParameters parameters) :
  Material(name, parameters),
  _temp(coupledValue("temp")),
  _R(getParam<Real>("R")),
  _factor(getParam<Real>("factor")),
  _model(getParam<int>("model")),
  _atomic_diffusivity(declareProperty<Real>("atomic_diffusivity"))
{

  if ( _model == 0 )
  {
    if ( !isParamValid("D0") || !isParamValid("Q") )
      mooseError("In AtomicDiffusionCoef: if model = 0 (user supplied), D0 and Q must also be supplied");
    _D0 = getParam<Real>("D0");
    _Q = getParam<Real>("Q");
  }
  else
  {
    if ( isParamValid("D0") || isParamValid("Q") )
      mooseError("In AtomicDiffusionCoef: D0 and Q are supplied, model must = 0");

    if ( _model == 1 )
    {
      _D0 = 3.0e13;
      _Q = 355000.0;
    }
    else if ( _model == 2 )
    {
      _D0 = 4.6e11;
      _Q = 326360.0;
    }
    else if ( _model == 3 )
    {
      _D0 = 1.66e5;
      _Q = 221154.0;
    }
    else
      mooseError("In AtomicDiffusionCoef: Invalid model value given.");
  }
}

void
AtomicDiffusionCoef::computeQpProperties()
{
  _atomic_diffusivity[_qp] = _D0 * std::exp( -_Q / _R / _temp[_qp] ) * _factor;
}
