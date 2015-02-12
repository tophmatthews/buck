#include "ClustersBoundsAction.h"
#include "FEProblem.h"
#include "Factory.h"
#include "BuckUtils.h"

template<>
InputParameters validParams<ClustersBoundsAction>()
{
	InputParameters params = validParams<Action>();

  params.addRequiredParam<std::vector<SubdomainName> >("block", "The blocks where bounds should be applied.");
  params.addParam<std::string>("var_name_base", "c", "specifies the base name of the variables");
  params.addRequiredParam<int>("N", "Largest cluster size for growth model inclusion");

  return params;
}

ClustersBoundsAction::ClustersBoundsAction(const std::string & name, InputParameters params) :
  Action(name, params),
  _blocks(getParam<std::vector<SubdomainName> >("block")),
  _var_name_base(getParam<std::string>("var_name_base")),
  _N(getParam<int>("N"))
{
  Buck::varNamesFromN( _vars, _var_name_base, _N );
}

void
ClustersBoundsAction::act()
{
  InputParameters params = _factory.getValidParams("VectorBoundsAux");

  params.set<std::vector<SubdomainName> >("block") = _blocks;
  params.set<AuxVariableName>("variable") = "bounds_dummy";
  params.set<std::vector<VariableName> >("bounded_variable") = _vars;
  params.set<Real>("lower") = 0;
  _problem->addAuxKernel("VectorBoundsAux", "Clusters_BoundsAux", params);
}
