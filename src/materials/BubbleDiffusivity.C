#include "BubbleDiffusivity.h"
#include "MooseEnum.h"

template<>
InputParameters validParams<BubbleDiffusivity>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredCoupledVar("coupled_rad", "List of coupled radius variables.");
  params.addRequiredCoupledVar("temp", "Coupled Temperature");
  params.addParam<Real>("D0", "Diffusion coefficient [um^2/s]");
  params.addParam<Real>("Q", "Activation energy [J/mol]");
  params.addParam<Real>("R", 8.31446, "Ideal gas constant [J/(K*mo)]");
  params.addParam<Real>("factor", 1, "Scaling factor to multiply by diffusivity.");
  params.addParam<int>("model", 1, "Switch for diffusion coefficient model (0=user input, 1=Buescher");

  return params;
}

BubbleDiffusivity::BubbleDiffusivity(const std::string & name, InputParameters parameters) :
  Material(name, parameters),
  _temp(coupledValue("temp")),
  _R(getParam<Real>("R")),
  _factor(getParam<Real>("factor")),
  _model(getParam<int>("model")),
  _gas_diffusivity(getMaterialProperty<Real>("gas_diffusivity")),
  _init(false),

  _bubble_diffusivity(declareProperty<std::vector<Real> >("bubble_diffusivity"))
{
  _G = coupledComponents("coupled_rad");

  for ( unsigned int i=0; i<_G; ++i )
    _r.push_back( &coupledValue("coupled_rad", i) );

  if ( _model == 0 )
  {
    if ( !isParamValid("D0") || !isParamValid("Q") )
      mooseError("In BubbleDiffusivity: if model = 0 (user supplied), D0 and Q must also be supplied");
    _D0 = getParam<Real>("D0");
    _Q = getParam<Real>("Q");
  }
  else
  {
    if ( isParamValid("D0") || isParamValid("Q") )
      mooseError("In BubbleDiffusivity: D0 and Q are supplied, model must = 0");

    if ( _model == 1 )
    {
      _D0 = 1.19e-2;
      _Q = 418400.0;
    }
    else
      mooseError("In BubbleDiffusivity: Invalid model value given.");
  }
}

void
BubbleDiffusivity::initialize()
{
  for (unsigned int qp=0; qp<_qrule->n_points(); ++qp)
    _bubble_diffusivity[qp].resize(_G);

  _init = true;
}


void
BubbleDiffusivity::computeQpProperties()
{
  if (!_init)
    initialize();

  _bubble_diffusivity[_qp][0] = _gas_diffusivity[_qp];
  for ( unsigned int i=1; i<_G; ++i )
  {
    _bubble_diffusivity[_qp][i] = _D0 / std::pow( (*_r[i])[_qp], 3.0 ) * std::exp( -_Q / _R / _temp[_qp] );
    // std::cout << "i: " << i << " diff[i]: " << _bubble_diffusivity[_qp][i] << std::endl;
  }
}
