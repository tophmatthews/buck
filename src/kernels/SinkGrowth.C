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
  params.addRequiredParam<int>("g", "Group number");
  params.addRequiredParam<int>("N_nuc", "Largest cluster size in nucleation model");

  return params;
}

SinkGrowth::SinkGrowth(const std::string & name, InputParameters parameters)
  :Kernel(name,parameters),
  _maxsize(getParam<std::vector<Real> >("coupled_maxsize")),
  _temp(coupledValue("temp")),
  _g(getParam<int>("g") - 1),
  _N_nuc(getParam<int>("N_nuc")),

  _atomic_diffusivity(getMaterialProperty<Real>("atomic_diffusivity"))

{
	_G = coupledComponents("coupled_conc");

  if ( _maxsize.size() != _G )
    mooseError("In SinkGrowth: coupled_conc and coupled_maxsize must be the same size!");

  for ( int i=0; i<_G; ++i )
  {
    if ( _maxsize[i] <= _N_nuc )
      _avgsize.push_back(_maxsize[i]);
    else
      _avgsize.push_back( ( _maxsize[i] + _maxsize[i-1] + 1.0 ) /2.0 );
  }

  for ( int i=0; i<_G-1; ++i )
  {
    if ( _maxsize[i] < _N_nuc )
      _jumpsize.push_back(0);
    else if ( _maxsize[i] == _N_nuc )
      _jumpsize.push_back( _maxsize[i+1]*0.5 - _maxsize[i]*0.5 + 0.5 );
    else
      _jumpsize.push_back( 0.5*_maxsize[i+1] - 0.5*_maxsize[i-1] );
  }

  for ( int i=0; i<_G; ++i)
    _c.push_back( &coupledValue("coupled_conc", i) );

  // Buck::iterateAndDisplay("avg", _avgsize);
  // Buck::iterateAndDisplay("max", _maxsize);
  // Buck::iterateAndDisplay("jumpsize", _jumpsize);
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

  losses = calcLosses();
  if ( _avgsize[_g] > _N_nuc )
    gains = calcGains();

  return -( gains - losses * _u[_qp] ) * _test[_i][_qp];
}

Real
SinkGrowth::computeQpJacobian()
{
  Real losses(0);
  // Real gains(0); // Gains are independent of u

  losses = calcLosses();

  return losses * _phi[_j][_qp] * _test[_i][_qp];
}


Real
SinkGrowth::calcLosses()
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
        radius = 1.0e9 * MaterialXeBubble::VDW_MtoR( _avgsize[i], _temp[_qp], sigma );
        losses += KoverR * radius * (*_c[i])[_qp] * _jumpsize[i]; // *_u[_qp] below
        // std::cout << "i: " << i << " size: " << _avgsize[i] << " width: " << _width[i] << " width/2: " << ceil(_width[i]/2) << std::endl;
      }
    }
  }
  else // if bubble
  {
    // Losses due to this cluster absorbing atoms
    if ( _g < _G-1 && _avgsize[_g] >= _N_nuc ) // make sure it's not the largest bubble size
    {
      radius = 1.0e9 * MaterialXeBubble::VDW_MtoR( _avgsize[_g], _temp[_qp], sigma );
      losses += KoverR * radius * (*_c[0])[_qp]; // *_u[_qp] below
    }
  }
  return losses;
}


Real
SinkGrowth::calcGains()
{
  // gains due to bubble size c_(m-1) absorbing single atoms
  Real gain(0);
  Real radius(0);
  Real sigma(0); // TODO: change this to be actual hydrostatic stress

  Real KoverR = 4.0 * M_PI * _atomic_diffusivity[_qp]; // reaction co-efficient divided by radius

  radius = 1.0e9 * MaterialXeBubble::VDW_MtoR( _avgsize[_g-1], _temp[_qp], sigma );
  gain = KoverR * radius * (*_c[0])[_qp] * (*_c[_g-1])[_qp];

  return gain;
}
