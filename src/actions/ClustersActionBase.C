#include "ClustersActionBase.h"

#include "BuckUtils.h"
#include <iomanip>

template<>
InputParameters validParams<ClustersActionBase>()
{
  InputParameters params = validParams<Action>();

  params.addParam<std::string>("var_name_base", "c", "Specifies the base name of the variables");
  params.addRequiredParam<int>("N", "Total number of groups");
  params.addRequiredParam<int>("M", "Number of ungrouped equations");

  return params;
}

ClustersActionBase::ClustersActionBase(const std::string & name, InputParameters params) :
  Action(name, params),
  _var_name_base(getParam<std::string>("var_name_base")),
  _N(getParam<int>("N")),
  _M(getParam<int>("M"))
{
  if ( _N < _M )
    mooseError("From ClustersActionBase: N_max must be equal to or greater than N_nuc.");

  varNamesFromG( _vars, _var_name_base, _N);

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
}


// void
// ClustersActionBase::avgSizeFromGroup(std::vector<Real> & sizes, const Real G, const Real N_max, const int N_nuc, const bool log)
// {
//   // Determines maximum cluster size for a given group.
//   // G = Max number of groups
//   // N_max = largest cluster size
//   // N_nuc = largest cluster size from nucleation model

//   // First setup nucleation clusters, which consist of N_nuc groups with one atom each

//   for ( int g=1; g<N_nuc+1; ++g )
//     sizes.push_back(g);

//   Real m; // slope
//   Real b; // intercept
//   if ( !log )
//   {
//     // Linear separation between max atoms and group number
//     b = N_nuc;
//     m = (N_max - N_nuc) / (G - N_nuc);

//     for ( int g=N_nuc; g<G; ++g )
//       sizes.push_back( b + m * (g-N_nuc+1) );
//   }
//   else
//   {
//     // roughly log separation
//     b = N_nuc - 1;
//     m = std::log10(N_max - N_nuc +1) / (G - N_nuc);

//     for ( int g=N_nuc; g<G; ++g )
//       sizes.push_back( b+ std::pow( 10, m * (g-N_nuc+1) ) );
//   }
// }


void 
ClustersActionBase::varNamesFromG(std::vector<VariableName> & vars, const std::string prefix, const int G, const int start)
{
  int digits = Buck::numDigits(G);

  for ( int i=start; i<G+1; ++i)
  {
    VariableName var_name = prefix;
    std::stringstream out;
    out << std::setw(digits) << std::setfill('0') << i;
    var_name.append(out.str());
    vars.push_back(var_name);
  }
}
