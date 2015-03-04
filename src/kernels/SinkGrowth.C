#include "SinkGrowth.h"

#include "MaterialXeBubble.h"
#include "BuckUtils.h"

template<>
InputParameters validParams<SinkGrowth>()
{
  InputParameters params = validParams<Kernel>();

  params.addRequiredCoupledVar("coupled_conc", "List of coupled concentration variables.");
  params.addRequiredCoupledVar("temp", "Coupled Temperature");
  params.addRequiredParam<int>("M", "Number of ungrouped equations");
  params.addRequiredParam<Real>("a", "Lattice parameter");
  params.addRequiredParam<Real>("omega", "Atomic volume");
  

  return params;
}

SinkGrowth::SinkGrowth(const std::string & name, InputParameters parameters)
  :Kernel(name,parameters),
  _names(getParam<std::vector<VariableName> >("coupled_conc")),
  _this_var(getParam<NonlinearVariableName>("variable")),
  _temp(coupledValue("temp")),
  _M(getParam<int>("M")),
  _a(getParam<Real>("a")),
  _omega(getParam<Real>("omega")),
  _atomic_diffusivity(getMaterialProperty<Real>("atomic_diffusivity"))
{
	_N = coupledComponents("coupled_conc");

  for ( unsigned int i=0; i<_N; ++i )
    _c.push_back( &coupledValue("coupled_conc", i) );

  // Determine which group current kernel acts on
  _g = -1;
  for ( unsigned int i=0; i<_N; ++i )
  {
    if ( _names[i].compare(_this_var) == 0 )
    {
      _g = i;
      break;
    }
  }
  if (_g == -1)
    mooseError("In SinkGrowth: Variable not found in coupled_conc list. Check the list.");


  for ( int j=0; j<_M; ++j )
  {
    _width.push_back(1);
    _maxsize.push_back(j+1);
    _minsize.push_back(j+1);
    _avgsize.push_back(j+1);
  }
  for ( int j=_M; j<_N; ++j )
  {
    _width.push_back  ( (_maxsize.back() + 1.0)/_M );
    _minsize.push_back( _maxsize.back() + 1.0 );
    _maxsize.push_back( _width.back() + _maxsize.back() );
    _avgsize.push_back( 0.5 * _minsize.back() + 0.5 * _maxsize.back() );
  }

  mooseDoOnce(Buck::iterateAndDisplay("max", _maxsize));
  mooseDoOnce(Buck::iterateAndDisplay("min", _minsize));
  mooseDoOnce(Buck::iterateAndDisplay("width", _width));
  mooseDoOnce(Buck::iterateAndDisplay("avg", _avgsize));
}

Real
SinkGrowth::computeQpResidual()
{
  // Computes gains and losses based on point defects traveling to isolated and well spaced sinks
  //
  // Rx rate = k * point defect concentration * sink concentration
  // where k = 4 * pi * radius of sink * point defect diffusivity

  Real losses(0);
  Real gains(0);

  if ( _g == 0 )  // If variable is a single gas atom
    losses = calcLossesForAtoms(false);
  else
    losses = calcLossesForBubbles(false);

  gains = calcGainsForBubbles(false);

  // std::cout << "\tg: " << _g << " gains: " << gains << " losses: " << losses << std::endl;

  return -( gains - losses ) * _test[_i][_qp];
}

Real
SinkGrowth::computeQpJacobian()
{
  Real losses(0);
  Real gains(0);

  if ( _g == 0 )  // If variable is a single gas atom
    losses = calcLossesForAtoms(true);
  else if (_g < _N-1)
    losses = calcLossesForBubbles(true);

  gains = calcGainsForBubbles(true);

  return -( gains - losses ) * _phi[_j][_qp] * _test[_i][_qp];
}

Real
SinkGrowth::calcLossesForAtoms(bool jac)
{
  Real losses(0);
  Real sigma(0); // TODO: change this to be actual hydrostatic stress

  // Losses due to single atoms combining to form dimers
  Real K1 = 84.0 / std::pow(_a, 2.0) * _atomic_diffusivity[_qp];
  if (!jac)
    losses += 2.0 * K1 * std::pow(_u[_qp], 2.0);
  else
    losses += 4.0 * K1 * _u[_qp];

  for ( unsigned int j=1; j<_N-1; ++j ) // iterate through clusters, except for largest size
  {
    Real radius = 1.0e9 * MaterialXeBubble::VDW_MtoR( _avgsize[j], _temp[_qp], sigma );
    Real upper = 4 * M_PI * radius;
    Real lower = _omega * ( 1 + std::sqrt(2) * _a / radius);
    Real Kj = upper / lower * _atomic_diffusivity[_qp];

    if (!jac)
      losses += _width[j] * Kj * _u[_qp] * (*_c[j])[_qp];
    else
      losses += _width[j] * Kj * (*_c[j])[_qp];
  }

  return losses;
}

Real
SinkGrowth::calcLossesForBubbles(bool jac)
{
  // Calculates losses due to absorbing single gas atoms
  Real sigma(0); // TODO: change this to be actual hydrostatic stress

  if ( _g == _N-1 )
    return 0;

  Real radius = 1.0e9 * MaterialXeBubble::VDW_MtoR( _avgsize[_g], _temp[_qp], sigma );
  Real upper = 4 * M_PI * radius;
  Real lower = _omega * ( 1 + std::sqrt(2) * _a / radius);
  Real Kj = upper / lower * _atomic_diffusivity[_qp];

  Real coeff = 2.0 / ( _width[_g] + _width[_g+1] );

  if (!jac)
    return coeff * Kj * (*_c[0])[_qp] * _u[_qp];
  else
    return coeff * Kj * (*_c[0])[_qp];
}


Real
SinkGrowth::calcGainsForBubbles(bool jac)
{
  // gains due to bubble size c_(m-1) absorbing single atoms
  Real sigma(0); // TODO: change this to be actual hydrostatic stress

  if (_g == 0 || jac)
    return 0;
  else if (_g == 1)
  {
    Real K1 = 84.0 / std::pow(_a, 2.0) * _atomic_diffusivity[_qp];
    return K1 * std::pow((*_c[0])[_qp], 2.0);
  }
  else
  {
    Real radius = 1.0e9 * MaterialXeBubble::VDW_MtoR( _avgsize[_g-1], _temp[_qp], sigma );
    Real upper = 4 * M_PI * radius;
    Real lower = _omega * ( 1 + std::sqrt(2) * _a / radius);
    Real Kj = upper / lower * _atomic_diffusivity[_qp];

    Real coeff = 2.0 * _width[_g-1] / _width[_g] / ( _width[_g] + _width[_g-1] );

    return coeff * Kj * (*_c[0])[_qp] * (*_c[_g-1])[_qp];
  }
}
