#include "ClustersActionBase.h"

#include "Factory.h"
#include "Parser.h"
#include "FEProblem.h"
#include "BuckUtils.h"

template<>
InputParameters validParams<ClustersActionBase>()
{
  InputParameters params = validParams<Action>();

  params.addParam<std::string>("var_name_base", "c", "Specifies the base name of the variables");
  params.addRequiredParam<int>("G", "Number of groups");
  params.addRequiredParam<int>("N_nuc", "Largest cluster size in nucleation model");
  params.addRequiredParam<Real>("N_max", "Largest cluster size");
  params.addRequiredParam<bool>("log", "Flag whether or not to use log spacing when relating atoms to group number");

  return params;
}

ClustersActionBase::ClustersActionBase(const std::string & name, InputParameters params) :
  Action(name, params),
  _var_name_base(getParam<std::string>("var_name_base")),
  _G(getParam<int>("G")),
  _N_nuc(getParam<int>("N_nuc")),
  _N_max(getParam<Real>("N_max")),
  _log(getParam<bool>("log"))
{
  setup();
}


void 
ClustersActionBase::setup()
{
  if ( _G < _N_max )
    mooseError("From ClustersActionBase: G must be equal to or greater than N_max.");
  if ( _N_nuc > _N_max )
    mooseError("From ClustersActionBase: N_max must be equal to or greater than N_nuc.");

  Buck::varNamesFromG( _vars, _var_name_base, _G);
  avgSizeFromGroup( _atoms, _G, getParam<Real>("N_max"), getParam<int>("N_nuc"), getParam<bool>("log") );
  // Buck::iterateAndDisplay("vars", _vars);
  // Buck::iterateAndDisplay("atoms", _atoms);
}


void
ClustersActionBase::avgSizeFromGroup(std::vector<Real> & avgs, const Real G, const Real N_max, const int N_nuc, const bool log)
{
  // Determines maximum cluster size for a given group.
  // G = Max number of groups
  // N_max = largest cluster size
  // N_nuc = largest cluster size from nucleation model

  // First setup nucleation clusters, which consist of N_nuc groups with one atom each
  
  std::vector<Real> sizes;
  
  for ( int g=1; g<N_nuc+1; ++g )
    sizes.push_back(g);

  Real m; // slope
  Real b; // intercept
  if ( !log )
  {
    // Linear separation between max atoms and group number
    b = N_nuc;
    m = (N_max - N_nuc) / (G - N_nuc);

    for ( int g=N_nuc; g<G+1; ++g )
      sizes.push_back( b + m * (g-N_nuc+1) );
  }
  else
  {
    // roughly log separation
    b = N_nuc - 1;
    m = std::log10(N_max - N_nuc +1) / (G - N_nuc);

    for ( int g=N_nuc; g<G+1; ++g )
      sizes.push_back( b+ std::pow( 10, m * (g-N_nuc+1) ) );
  }

  avgs.push_back(1.0);
  for ( int g=0; g<N_max-1; ++g )
  {
    Real tempavg = 0.5*sizes[g] + 0.5*sizes[g+1];
    avgs.push_back( ceil(tempavg) );
  }
}
