#include "SinkGrowth.h"

// #include "MooseException.h"
#include "MaterialXeBubble.h"

template<>
InputParameters validParams<SinkGrowth>()
{
  InputParameters params = validParams<Kernel>();

  params.addRequiredCoupledVar("coupled_conc", "List of coupled concentration variables.");
  params.addRequiredParam<std::vector<Real> >("coupled_conc_atoms", "List of coupled concentration variables.");
  params.addRequiredCoupledVar("temp", "Coupled Temperature");
  params.addRequiredParam<int>("g", "Group number");
  params.addRequiredParam<int>("N_nuc", "Largest cluster size in nucleation model");

  return params;
}

SinkGrowth::SinkGrowth(const std::string & name, InputParameters parameters)
  :Kernel(name,parameters),
  _atoms(getParam<std::vector<Real> >("coupled_conc_atoms")),
  _temp(coupledValue("temp")),
  _g(getParam<int>("g") - 1),
  _N_nuc(getParam<int>("N_nuc")),

  _atomic_diffusivity(getMaterialProperty<Real>("atomic_diffusivity"))

{
	_G = coupledComponents("coupled_conc");

  for ( int i=0; i<_G; ++i)
    _c.push_back( &coupledValue("coupled_conc", i) );

  if ( _atoms.size() != _c.size() )
    mooseError("In SinkGrowth: coupled_conc.size() != coupled_conc_atoms.size()");
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
  if ( _atoms[_g] > _N_nuc )
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
      if ( _atoms[i] >= _N_nuc )
      {
        radius = 1.0e9 * MaterialXeBubble::VDW_MtoR( _atoms[i], _temp[_qp], sigma );
        losses += KoverR * radius * (*_c[i])[_qp]; // *_u[_qp] below
      }
    }
  }
  else // if bubble
  {
    // Losses due to this cluster absorbing atoms
    if ( _g < _G-1 && _atoms[_g] >= _N_nuc ) // make sure it's not the largest bubble size
    {
      radius = 1.0e9 * MaterialXeBubble::VDW_MtoR( _atoms[_g], _temp[_qp], sigma );
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

  radius = 1.0e9 * MaterialXeBubble::VDW_MtoR( _atoms[_g-1], _temp[_qp], sigma );
  gain = KoverR * radius * (*_c[0])[_qp] * (*_c[_g-1])[_qp];

  return gain;
}
