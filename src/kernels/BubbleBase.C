#include "BubbleBase.h"

#include "MaterialXeBubble.h"
#include "BuckUtils.h"

template<>
InputParameters validParams<BubbleBase>()
{
  InputParameters params = validParams<Kernel>();

  params.addRequiredCoupledVar("coupled_conc", "List of coupled concentration variables.");
  params.addRequiredCoupledVar("coupled_rad", "List of coupled radius variables.");
  params.addRequiredParam<int>("M", "Number of ungrouped equations");

  return params;
}

BubbleBase::BubbleBase(const std::string & name, InputParameters parameters)
  :Kernel(name,parameters),
  _names(getParam<std::vector<VariableName> >("coupled_conc")),
  _this_var(getParam<NonlinearVariableName>("variable")),
  _M(getParam<int>("M"))

{
	_G = coupledComponents("coupled_conc");
  if (_G != coupledComponents("coupled_rad"))
    mooseError("From BubbleBase: The number of coupled concentrations does not match coupled radii.");

  for ( unsigned int i=0; i<_G; ++i )
  {
    _c.push_back( &coupledValue("coupled_conc", i) );
    _r.push_back( &coupledValue("coupled_rad", i) );
  }

  // Determine which group current kernel acts on
  _g = -1;
  for ( unsigned int i=0; i<_G; ++i )
  {
    if ( _names[i].compare(_this_var) == 0 )
    {
      _g = i;
      break;
    }
  }
  if (_g == -1)
    mooseError("From BubbleBase: Variable not found in coupled_conc list. Check the list.");

  for ( int j=0; j<_M; ++j )
  {
    _width.push_back(1);
    _maxsize.push_back(j+1);
    _minsize.push_back(j+1);
    _avgsize.push_back(j+1);
  }
  for ( int j=_M; j<_G; ++j )
  {
    _width.push_back  ( (_maxsize.back() + 1.0)/_M );
    _minsize.push_back( _maxsize.back() + 1.0 );
    _maxsize.push_back( _width.back() + _maxsize.back() );
    _avgsize.push_back( 0.5 * _minsize.back() + 0.5 * _maxsize.back() );
  }

  // mooseDoOnce(Buck::iterateAndDisplay("max", _maxsize));
  // mooseDoOnce(Buck::iterateAndDisplay("min", _minsize));
  // mooseDoOnce(Buck::iterateAndDisplay("width", _width));
  // mooseDoOnce(Buck::iterateAndDisplay("avg", _avgsize));
}

Real
BubbleBase::computeQpResidual()
{
  Real losses = calcLosses(false);
  Real gains = calcGains(false);

  // std::cout << "\tg: " << _g << " gains: " << gains << " losses: " << losses << std::endl;

  return -( gains - losses ) * _test[_i][_qp];
}


Real
BubbleBase::computeQpJacobian()
{
  Real losses = calcLosses(true);
  Real gains = calcGains(true);

  return -( gains - losses ) * _phi[_j][_qp] * _test[_i][_qp];
}


Real
BubbleBase::calcLosses(bool jac)
{
  return 0;
}


Real
BubbleBase::calcGains(bool jac)
{
  return 0;
}
