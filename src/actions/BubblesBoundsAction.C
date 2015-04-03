#include "BubblesBoundsAction.h"

#include "FEProblem.h"
#include "Factory.h"
#include "libmesh/string_to_enum.h"

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
  if (_current_task == "add_aux_kernel")
  {
    InputParameters params = _factory.getValidParams("VectorBoundsAux");

    params.set<std::vector<SubdomainName> >("block") = _blocks;
    params.set<AuxVariableName>("variable") = "bounds_dummy";
    params.set<std::vector<VariableName> >("bounded_variable") = _c;
    params.set<Real>("lower") = getParam<Real>("lower");
    _problem->addAuxKernel("VectorBoundsAux", "Cubbles_BoundsAux", params);
  }
  else if (_current_task == "add_aux_variable")
  {
    std::set<SubdomainID> blocks;

    std::vector<SubdomainName> block_param = getParam<std::vector<SubdomainName> >("block");
    for (std::vector<SubdomainName>::iterator it = block_param.begin(); it != block_param.end(); ++it)
      blocks.insert(_problem->mesh().getSubdomainID(*it));

    _problem->addAuxVariable( "bounds_dummy",
                              FEType(Utility::string_to_enum<Order>("FIRST"),
                              Utility::string_to_enum<FEFamily>("LAGRANGE")),
                              &blocks);
  }
}
