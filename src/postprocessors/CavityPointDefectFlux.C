#include "CavityPointDefectFlux.h"

template<>
InputParameters validParams<CavityPointDefectFlux>()
{
  InputParameters params = validParams<ElementAverageValue>();

  params.addRequiredParam<std::string>("diffusivity", "The name of the diffusivity material property that will be used in the flux computation.");

  return params;
}

CavityPointDefectFlux::CavityPointDefectFlux(const std::string & name, InputParameters parameters) :
  ElementAverageValue(name, parameters),

  _diffusivity(getMaterialProperty<Real>(parameters.get<std::string>("diffusivity")))
{
}


Real
CavityPointDefectFlux::computeQpIntegral()
{
  return _u[_qp] * _diffusivity[_qp];
}


Real
CavityPointDefectFlux::getValue()
{
  gatherSum(_integral_value);
  gatherSum(_volume);

  return _integral_value / _volume;
}
