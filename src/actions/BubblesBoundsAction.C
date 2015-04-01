#include "BubblesBoundsAction.h"

#include "FEProblem.h"
#include "Factory.h"

template<>
InputParameters validParams<BubblesBoundsAction>()
{
	InputParameters params = validParams<BubblesActionBase>();

  params.addRequiredParam<std::vector<SubdomainName> >("block", "The blocks where bounds should be applied.");
  params.addParam<Real>("lower", 0, "Lower bounded limit");

  return params;
}

BubblesBoundsAction::BubblesBoundsAction(const std::string & name, InputParameters params) :
  BubblesActionBase(name, params),
  _blocks(getParam<std::vector<SubdomainName> >("block"))
{
}

void
BubblesBoundsAction::act()
{
  InputParameters params = _factory.getValidParams("VectorBoundsAux");

  params.set<std::vector<SubdomainName> >("block") = _blocks;
  params.set<AuxVariableName>("variable") = "bounds_dummy";
  params.set<std::vector<VariableName> >("bounded_variable") = _c;
  params.set<Real>("lower") = getParam<Real>("lower");
  _problem->addAuxKernel("VectorBoundsAux", "Cubbles_BoundsAux", params);
}
