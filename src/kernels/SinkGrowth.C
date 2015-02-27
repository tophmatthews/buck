#include "SinkGrowth.h"

#include "MaterialXeBubble.h"
#include "BuckUtils.h"

template<>
InputParameters validParams<SinkGrowth>()
{
  InputParameters params = validParams<Kernel>();

  params.addRequiredCoupledVar("coupled_conc", "List of coupled concentration variables.");
  params.addRequiredParam<std::vector<Real> >("coupled_maxsize", "List of coupled concentration variables.");
  params.addRequiredCoupledVar("temp", "Coupled Temperature");
  params.addRequiredParam<int>("N_nuc", "Largest cluster size in nucleation model");

  return params;
}

SinkGrowth::SinkGrowth(const std::string & name, InputParameters parameters)
  :Kernel(name,parameters),
  _names(getParam<std::vector<VariableName> >("coupled_conc")),
  _this_var(getParam<NonlinearVariableName>("variable")),
  _maxsize(getParam<std::vector<Real> >("coupled_maxsize")),
  _temp(coupledValue("temp")),
  _N_nuc(getParam<int>("N_nuc")),

  _atomic_diffusivity(getMaterialProperty<Real>("atomic_diffusivity"))
{
	_G = coupledComponents("coupled_conc");

  if ( _maxsize.size() != _G )
    mooseError("In SinkGrowth: coupled_conc and coupled_maxsize must be the same size!");

  for ( unsigned int i=0; i<_G; ++i )
    _c.push_back( &coupledValue("coupled_conc", i) );

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
    mooseError("In SinkGrowth: Variable not found in coupled_conc list. Check the list.");

  for ( unsigned int i=0; i<_G; ++i )
  {
    if ( _maxsize[i] <= _N_nuc )
      _minsize.push_back(_maxsize[i]);
    else
      _minsize.push_back( _maxsize[i-1] +1.0 );
  }

  _jumpsize.push_back(0);
  for ( unsigned int i=1; i<_G-1; ++i )
    _jumpsize.push_back( _maxsize[i] - _minsize[i] + 1.0);
  _jumpsize.push_back(0);

  mooseDoOnce(Buck::iterateAndDisplay("max", _maxsize));
  mooseDoOnce(Buck::iterateAndDisplay("min", _minsize));
  mooseDoOnce(Buck::iterateAndDisplay("jumpsize", _jumpsize));
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
  else if ( _g < _G-1 && _minsize[_g] >= _N_nuc )  // If current var bubble is not the largest, and not below the smallest
    losses = calcLossesForBubbles(false);

  if ( _minsize[_g] > _N_nuc ) // If bubble is not below smallest
    gains = calcGainsForBubbles(false);

  // std::cout << std::setprecision(15) << "\tg: " << _g << " gains: " << gains << " losses: " << losses << std::endl;

  return -( gains - losses ) * _test[_i][_qp];
}

Real
SinkGrowth::computeQpJacobian()
{
  Real losses(0);
  Real gains(0);

  if ( _g == 0 )
    losses = calcLossesForAtoms(true);
  else if ( _g < _G-1 && _minsize[_g] >= _N_nuc )
    losses = calcLossesForBubbles(true);

  if ( _minsize[_g] > _N_nuc )
    gains = calcGainsForBubbles(true);

  return -( gains - losses ) * _phi[_j][_qp] * _test[_i][_qp];
}

Real
SinkGrowth::calcLossesForAtoms(bool jac)
{
  Real losses(0);
  Real sigma(0); // TODO: change this to be actual hydrostatic stress

  Real KoverR = 4.0 * M_PI * _atomic_diffusivity[_qp]; // reaction co-efficient divided by radius

  for ( unsigned int g=1; g<_G-1; ++g ) // iterate through clusters, except for largest size
  {
    if ( _minsize[g] >= _N_nuc ) // don't allow losses from nucleation model bubbles, except for N_nuc concentration
    {
      Real m=_maxsize[g];
      Real radius = 1.0e9 * MaterialXeBubble::VDW_MtoR( m, _temp[_qp], sigma );

      Real conc;
      if ( _maxsize[g] == _minsize[g] )
        conc = (*_c[g])[_qp];
      else if ( (*_c[g])[_qp] == 0 )
        conc = 0;
      else if ( (*_c[g+1])[_qp] == 0 )
      {
        if ( g <= 2 && _maxsize[g] != _minsize[g] )
          mooseError("In SinkGrowth: Not enough groups specified.");

        conc = Buck::logEst( _minsize[g-1], _minsize[g], (*_c[g-1])[_qp], (*_c[g])[_qp], m);
      }
      else
        conc = Buck::logEst( _minsize[g], _minsize[g+1], (*_c[g])[_qp], (*_c[g+1])[_qp], m);

      losses += KoverR * radius * conc * _jumpsize[g]; // *_u[_qp] later
    }
  }

  if (!jac)
    return losses * _u[_qp];
  else
    return losses;
}

Real
SinkGrowth::calcLossesForBubbles(bool jac)
{
  // Calculates losses due to absorbing single gas atoms
  Real sigma(0); // TODO: change this to be actual hydrostatic stress

  Real KoverR = 4.0 * M_PI * _atomic_diffusivity[_qp]; // reaction co-efficient divided by radius

  Real radius = 1.0e9 * MaterialXeBubble::VDW_MtoR( _maxsize[_g], _temp[_qp], sigma );

  Real conc;
  if ( _maxsize[_g] == _minsize[_g] )
  {
    if (!jac)
      conc = _u[_qp];
    else
      conc = 1.0;
  }
  else if ( _u[_qp] == 0 )
    conc = 0;
  else if ( (*_c[_g+1])[_qp] == 0 )
  {
    if ( _g <= 2 && _maxsize[_g] != _minsize[_g] )
      mooseError("In SinkGrowth: Not enough groups specified.");

    if (!jac)
      conc = Buck::logEst( _minsize[_g-1], _minsize[_g], (*_c[_g-1])[_qp], _u[_qp], _maxsize[_g] ); // if _minsize = _maxsize, reduces to _u[_qp]
    else
      conc = Buck::dlogEstdRight( _minsize[_g-1], _minsize[_g], (*_c[_g-1])[_qp], _u[_qp], _maxsize[_g] ); // if _minsize = _maxsize, reduces to 1
  }
  else
  {
    if (!jac)
      conc = Buck::logEst( _minsize[_g], _minsize[_g+1], _u[_qp], (*_c[_g+1])[_qp], _maxsize[_g] ); // if _minsize = _maxsize, reduces to _u[_qp]
    else
      conc = Buck::dlogEstdLeft( _minsize[_g], _minsize[_g+1], _u[_qp], (*_c[_g+1])[_qp], _maxsize[_g] ); // if _minsize = _maxsize, reduces to 1
  }

  return KoverR * radius * conc * (*_c[0])[_qp];
}


Real
SinkGrowth::calcGainsForBubbles(bool jac)
{
  // gains due to bubble size c_(m-1) absorbing single atoms
  Real sigma(0); // TODO: change this to be actual hydrostatic stress

  Real KoverR = 4.0 * M_PI * _atomic_diffusivity[_qp];

  Real m = _maxsize[_g-1];
  Real radius = 1.0e9 * MaterialXeBubble::VDW_MtoR( m, _temp[_qp], sigma );

  Real conc;
  if ( _maxsize[_g-1] == _minsize[_g-1] )
  {
    if (!jac)
      conc = (*_c[_g-1])[_qp];
    else
      conc = 0;
  }
  else if ( (*_c[_g-1])[_qp] == 0 )
    conc = 0;
  else if ( _u[_qp] == 0  )
  {
    if ( _g <= 2  )
      mooseError("In SinkGrowth: Not enough groups specified.");

    if (!jac)
      conc = Buck::logEst( _minsize[_g-2], _minsize[_g-1], (*_c[_g-2])[_qp], (*_c[_g-1])[_qp], m); // if m=_minsize[g-1], then returns _c[_g-1]
    else
      conc = 0; // does not depend on _u
  }
  else
  {
    if (!jac)
      conc = Buck::logEst( _minsize[_g-1], _minsize[_g], (*_c[_g-1])[_qp], _u[_qp], m);// if m=_minsize[g-1], then returns _c[_g-1]
    else
      conc = Buck::dlogEstdRight( _minsize[_g-1], _minsize[_g], (*_c[_g-1])[_qp], _u[_qp], m); // if m=_minsize[g-1], then returns 0
  }

  // std::cout << "K: " << KoverR * radius << " 10^(conc*u): " << conc * (*_c[0])[_qp] << std::endl;
    
  return KoverR * radius * conc * (*_c[0])[_qp];
  // return 0.005;
}
