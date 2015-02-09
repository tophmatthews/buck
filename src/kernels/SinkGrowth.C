#include "SinkGrowth.h"

#include "MooseException.h"
#include "MaterialXeBubble.h"

template<>
InputParameters validParams<SinkGrowth>()
{
  InputParameters params = validParams<Kernel>();

  params.addRequiredCoupledVar("temp", "Coupled Temperature");
	params.addRequiredCoupledVar("growth_conc_vars", "List of concentration variables for growth model, starting with c1 and ending with the largest group.");
  params.addRequiredParam<int>("m", "Number of atoms in cluster.");

  return params;
}

SinkGrowth::SinkGrowth(const std::string & name, InputParameters parameters)
  :Kernel(name,parameters),
  _temp(coupledValue("temp")),
  _m(getParam<int>("m")),
  _atomic_diffusivity(getMaterialProperty<Real>("atomic_diffusivity"))

{
	_N = coupledComponents("growth_conc_vars");

  _c.resize(_N);

  for ( int i=0; i<_N; ++i)
    _c[i] = &coupledValue("growth_conc_vars", i);
  
  _atoms = parseAtomsFromVars();

  if ( std::find(_atoms.begin(), _atoms.end(), _m)==_atoms.end() )
  {
      std::stringstream errorMsg;
      errorMsg << "In SinkGrowth: Couldn't find the current atom number in the atom list.\n"
               << "Are you sure nucleation_conc_vars has the right number of variables?\n"
               << "\tcurrent m: " << _m << std::endl;
      mooseError(errorMsg.str());
  }
  // std::cout << "SinkGrowth tempetature: " << _temp[0] << std::endl;
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
  if ( _m > _atoms[1] )
    gains = calcGains();

  return -( gains - losses * _u[_qp] ) * _test[_i][_qp];
}

Real
SinkGrowth::computeQpJacobian()
{
  Real losses(0);
  Real gains(0);
  
  losses = calcLosses();
  if ( _m > _atoms[1] )
    gains = calcGains();

  return -( gains - losses * _phi[_j][_qp] ) * _test[_i][_qp];
}


std::vector<Real>
SinkGrowth::parseAtomsFromVars()
{ 
  // Pulls the dependent variables from the current kernel
  std::vector<std::string> depend_vars;

  std::map<std::string, std::vector<MooseVariable *> > coupled_vars = getCoupledVars();
  for (std::map<std::string, std::vector<MooseVariable *> >::iterator it = coupled_vars.begin(); it != coupled_vars.end(); ++it)
    for (std::vector<MooseVariable *>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
      depend_vars.push_back((*it2)->name());

  // parses the string list of variables into a vector of Reals
  std::vector<Real> atoms;
  std::string buf;
  Real num;

  for ( int i=0; i<depend_vars.size(); ++i)
  {
  	if ( depend_vars[i].find("c") == 0 )
  	{
	    depend_vars[i].erase( depend_vars[i].begin() );
	    num = std::stod(depend_vars[i]);
	    atoms.push_back(num);
	  }
  }

  return atoms;
}


Real
SinkGrowth::calcLosses()
{
  Real losses(0);
  Real radius(0);
  Real sigma(0); // TODO: change this to be actual hydrostatic stress

  Real KoverR = 4.0 * M_PI * _atomic_diffusivity[_qp]; // reaction co-efficient divided by radius

  if (_m == 1) // if single gas atom
  {
    for ( int i=1; i<_N-1; ++i ) // iterate through cluster, except for largest size
    {
      radius = 1.0e9 * MaterialXeBubble::VDW_MtoR( _atoms[i], _temp[_qp], sigma );
      losses += KoverR * radius * (*_c[i])[_qp]; // *_u[_qp] below
    }
  }
  else // if bubble
  {
    // Losses due to this cluster absorbing atoms
    if ( _m < _atoms.back() ) // make sure it's not the largest bubble size
    {
      radius = 1.0e9 * MaterialXeBubble::VDW_MtoR( _m, _temp[_qp], sigma );
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

  // First find position of next smaller bubble in _atoms
  int it(0);
  while (_atoms[it] < _m)
    ++it;
  --it;

  // Then calculate and add gains
  radius = 1.0e9 * MaterialXeBubble::VDW_MtoR( _atoms[it], _temp[_qp], sigma );
  gain = KoverR * radius * (*_c[0])[_qp] * (*_c[it])[_qp];

  return gain;
}
