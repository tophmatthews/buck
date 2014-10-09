#include "BubblesUC.h"
#include "MooseEnum.h"

template<>
InputParameters validParams<BubblesUC>()
{
  InputParameters params = validParams<Material>();
  
  params.addParam<Real>("initial_P0_porosity", "The initial P0 porosity value");
  params.addParam<Real>("initial_P1_porosity", "The initial P1 porosity value");
  params.addParam<Real>("initial_P2_porosity", "The initial P2 porosity value");
  params.addParam<Real>("initial_P3_porosity", "The initial P3 porosity value");
  
  return params;
}

BubblesUC::BubblesUC(const std::string & name, InputParameters parameters) :
    Material(name, parameters),
        
    _initial_P0_porosity(getParam<Real>("initial_P0_porosity")),
    _initial_P1_porosity(getParam<Real>("initial_P1_porosity")),
    _initial_P2_porosity(getParam<Real>("initial_P2_porosity")),
    _initial_P3_porosity(getParam<Real>("initial_P3_porosity")),
        
    _P0_porosity(declareProperty<Real>("P0_porosity")),
    _P1_porosity(declareProperty<Real>("P1_porosity")),
    _P2_porosity(declareProperty<Real>("P2_porosity")),
    _P3_porosity(declareProperty<Real>("P3_porosity"))

        
{
}

void
BubblesUC::initQpStatefulProperties()
{
  _P0_porosity[_qp] = 0;
  _P1_porosity[_qp] = 0;
  _P2_porosity[_qp] = 0;
  _P3_porosity[_qp] = 0;
}

void
BubblesUC::computeProperties()
{
  for(unsigned int qp(0); qp < _qrule->n_points(); ++qp)
  {
    // FIXME: these should be integrals over the concentration distribution. Just placeholders for now
    _P0_porosity[qp] = _initial_P0_porosity;
    _P1_porosity[qp] = _initial_P1_porosity;
    _P2_porosity[qp] = _initial_P2_porosity;
    _P3_porosity[qp] = _initial_P3_porosity;
  }
}
