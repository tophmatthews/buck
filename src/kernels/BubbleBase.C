#include "BubbleBase.h"

#include "MaterialXeBubble.h"
#include "BuckUtils.h"

template<>
InputParameters validParams<BubbleBase>()
{
  InputParameters params = validParams<Kernel>();

  params.addRequiredCoupledVar("coupled_conc", "List of coupled concentration variables.");
  params.addRequiredCoupledVar("coupled_rad", "List of coupled radius variables.");
  params.addRequiredParam<std::vector<Real> >("coupled_atoms", "List of atom sizes for coupled variables.");

  return params;
}

BubbleBase::BubbleBase(const std::string & name, InputParameters parameters)
  :Kernel(name,parameters),
  _names(getParam<std::vector<VariableName> >("coupled_conc")),
  _this_var(getParam<NonlinearVariableName>("variable")),
  _atoms(getParam<std::vector<Real> >("coupled_atoms"))
{
	_G = coupledComponents("coupled_conc");
  if ( _G != coupledComponents("coupled_rad") )
    mooseError("From BubbleBase: The number of coupled concentrations does not match coupled radii.");
  if ( _G != _atoms.size() )
    mooseError("From BubbleBase: The number of coupled concentrations does not match atom sizes list.");

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

  for ( unsigned int i=0; i<_G-1; ++i)
    _width.push_back(_atoms[i+1] - _atoms[i]);
  _width.push_back(1.0);

  mooseDoOnce( displayBubbleInfo() );
}

Real
BubbleBase::computeQpResidual()
{
  Real losses(0);
  Real gains(0);
  calcLosses(losses, false);
  calcGains(gains, false);

  // std::cout << "\tg: " << _g << " gains: " << gains << " losses: " << losses << std::endl;

  return -( gains - losses ) * _test[_i][_qp];
}


Real
BubbleBase::computeQpJacobian()
{
  Real losses(0);
  Real gains(0);
  calcLosses(losses, true);
  calcGains(gains, true);

  return -( gains - losses ) * _phi[_j][_qp] * _test[_i][_qp];
}

void
BubbleBase::displayBubbleInfo()
{
  std::cout.precision(6);
  std::cout << std::scientific;
  std::cout << "=======================================\n";
  std::cout << "    --< BUCK Bubble Information >--     \n";
  std::cout << "=======================================\n";
  std::cout << " group \t| avg atoms\t| width\n";
  for (int i=0; i<_G; ++i)
    std::cout << " " << i << " \t| " << _atoms[i] << "\t| " << _width[i] << "\n";
  std::cout << "=======================================\n" << std::endl;
}

