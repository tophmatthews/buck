#include "ClustersBoundsAction.h"

#include "FEProblem.h"
#include "Factory.h"

template<>
InputParameters validParams<ClustersBoundsAction>()
{
	InputParameters params = validParams<ClustersActionBase>();

  params.addRequiredParam<std::vector<SubdomainName> >("block", "The blocks where bounds should be applied.");
  params.addParam<Real>("lower", 0, "Lower bounded limit");

  return params;
}

ClustersBoundsAction::ClustersBoundsAction(const std::string & name, InputParameters params) :
  ClustersActionBase(name, params),
  _blocks(getParam<std::vector<SubdomainName> >("block"))
{
}

void
ClustersBoundsAction::act()
{
  InputParameters params = _factory.getValidParams("VectorBoundsAux");

  params.set<std::vector<SubdomainName> >("block") = _blocks;
  params.set<AuxVariableName>("variable") = "bounds_dummy";
  params.set<std::vector<VariableName> >("bounded_variable") = _vars;
  params.set<Real>("lower") = getParam<Real>("lower");
  _problem->addAuxKernel("VectorBoundsAux", "Clusters_BoundsAux", params);
}
