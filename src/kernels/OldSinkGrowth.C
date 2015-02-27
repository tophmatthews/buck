#include "OldSinkGrowth.h"

#include "MaterialXeBubble.h"
#include "BuckUtils.h"

template<>
InputParameters validParams<OldSinkGrowth>()
{
  InputParameters params = validParams<Kernel>();

  params.addRequiredCoupledVar("coupled_conc", "List of coupled concentration variables.");
  params.addRequiredParam<std::vector<Real> >("coupled_maxsize", "List of coupled concentration variables.");
  params.addRequiredCoupledVar("temp", "Coupled Temperature");
  // params.addRequiredParam<int>("g", "Group number");
  params.addRequiredParam<int>("N_nuc", "Largest cluster size in nucleation model");

  return params;
}

OldSinkGrowth::OldSinkGrowth(const std::string & name, InputParameters parameters)
  :Kernel(name,parameters),
  _names(getParam<std::vector<VariableName> >("coupled_conc")),
  _this_var(getParam<NonlinearVariableName>("variable")),
  _maxsize(getParam<std::vector<Real> >("coupled_maxsize")),
  // _g(getParam<int>("g") - 1),
  _temp(coupledValue("temp")),
  _N_nuc(getParam<int>("N_nuc")),

  _atomic_diffusivity(getMaterialProperty<Real>("atomic_diffusivity"))

{
	_G = coupledComponents("coupled_conc");

  if ( _maxsize.size() != _G )
    mooseError("In OldSinkGrowth: coupled_conc and coupled_maxsize must be the same size!");

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

  for ( int i=0; i<_G; ++i )
  {
    if ( _maxsize[i] <= _N_nuc )
      _avgsize.push_back(_maxsize[i]);
    else
      _avgsize.push_back( ( _maxsize[i] + _maxsize[i-1] + 1.0 ) /2.0 );
  }

  for ( unsigned int i=0; i<_G; ++i )
  {
    if ( _maxsize[i] <= _N_nuc )
      _minsize.push_back(_maxsize[i]);
    else
      _minsize.push_back( _maxsize[i-1] +1.0 );
  }

  for ( int i=0; i<_G-1; ++i )
  {
    if ( _maxsize[i] < _N_nuc )
      _jumpsize.push_back(0);
    else
      _jumpsize.push_back( _maxsize[i] - _minsize[i] + 1.0 );
    // else
    //   _jumpsize.push_back( 0.5*_maxsize[i+1] - 0.5*_maxsize[i-1] );
  }
  _jumpsize.push_back(0);

  for ( int i=0; i<_G; ++i)
    _c.push_back( &coupledValue("coupled_conc", i) );

  mooseDoOnce(Buck::iterateAndDisplay("min", _minsize));
  mooseDoOnce(Buck::iterateAndDisplay("avg", _avgsize));
  mooseDoOnce(Buck::iterateAndDisplay("max", _maxsize));
  mooseDoOnce(Buck::iterateAndDisplay("jumpsize", _jumpsize));
}

Real
OldSinkGrowth::computeQpResidual()
{
  // Computes gains and losses based on point defects traveling to isolated and well spaced sinks
  //
  // Rx rate = k * point defect concentration * sink concentration
  // where k = 4 * pi * radius of sink * point defect diffusivity

  Real losses(0);
  Real gains(0);

  losses = calcLosses();
  if ( _avgsize[_g] > _N_nuc )
    gains = calcGains();

  // std::cout << std::setprecision(15) << "g: " << _g << " gains: " << gains << " losses: " << losses*_u[_qp] << std::endl;

  return -( gains - losses)  * _test[_i][_qp];
}

Real
OldSinkGrowth::computeQpJacobian()
{
  Real losses(0);
  // Real gains(0); // Gains are independent of u

  // losses = calcLosses();

  return losses * _phi[_j][_qp] * _test[_i][_qp];
}


Real
OldSinkGrowth::calcLosses()
{
  // Calculates losses due to absorbing single gas atoms
  Real losses(0);
  Real radius(0);
  Real sigma(0); // TODO: change this to be actual hydrostatic stress

  Real KoverR = 4.0 * M_PI * _atomic_diffusivity[_qp]; // reaction co-efficient divided by radius

  if ( _g == 0 ) // if single gas atom
  {
    for ( int i=1; i<_G-1; ++i ) // iterate through clusters, except for largest size
    {
      if ( _avgsize[i] >= _N_nuc ) // don't allow losses from nucleation model bubbles, except for N_nuc concentration
      {
        radius = 1.0e9 * MaterialXeBubble::VDW_MtoR( _minsize[i], _temp[_qp], sigma );
        losses += KoverR * radius * (*_c[i])[_qp] * std::pow(_u[_qp], _jumpsize[i]);
      }
    }
  }
  else // if bubble
  {
    // Losses due to this cluster absorbing atoms
    if ( _g < _G-1 && _avgsize[_g] >= _N_nuc ) // make sure it's not the largest bubble size
    {
      radius = 1.0e9 * MaterialXeBubble::VDW_MtoR( _avgsize[_g], _temp[_qp], sigma );
      losses += KoverR * radius * std::pow((*_c[0])[_qp], _jumpsize[_g]) * _u[_qp];
    }
  }
  return losses;
}


Real
OldSinkGrowth::calcGains()
{
  // gains due to bubble size c_(m-1) absorbing single atoms
  Real gain(0);
  Real radius(0);
  Real sigma(0); // TODO: change this to be actual hydrostatic stress

  Real KoverR = 4.0 * M_PI * _atomic_diffusivity[_qp]; // reaction co-efficient divided by radius

  radius = 1.0e9 * MaterialXeBubble::VDW_MtoR( _avgsize[_g-1], _temp[_qp], sigma );
  gain = KoverR * radius * std::pow((*_c[0])[_qp], _jumpsize[_g-1]) * (*_c[_g-1])[_qp];

  // std::cout << " g: " << _g << " KoverR: " << KoverR << " radius: " << radius << " conc0: " << (*_c[0])[_qp] << " conc-1: " << (*_c[_g-1])[_qp] << " conc: " << (*_c[_g-1])[_qp] << std::endl; 
  return gain;
}
