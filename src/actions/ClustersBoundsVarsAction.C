#include "ClustersBoundsVarsAction.h"

#include "Factory.h"
#include "FEProblem.h"
#include "Parser.h"
#include "libmesh/string_to_enum.h"
#include "MooseApp.h"

template<>
InputParameters validParams<ClustersBoundsVarsAction>()
{
  InputParameters params = validParams<Action>();

  params.addParam<std::string>("order", "FIRST",  "Specifies the order of the FE shape function to use for this variable");
  params.addParam<std::string>("family", "LAGRANGE", "Specifies the family of FE shape functions to use for this variable");
  params.addRequiredParam<std::vector<SubdomainName> >("block", "The block id where this variable lives");


  return params;
}

ClustersBoundsVarsAction::ClustersBoundsVarsAction(const std::string & name,
                                                   InputParameters params) :
  Action(name, params),
  _order(getParam<std::string>("order")),
  _family(getParam<std::string>("family"))
{
  mooseAssert(!getParam<std::vector<SubdomainName> >("block").empty(), "Blocks must be specified in ClustersBoundsVarsAction");
}

void
ClustersBoundsVarsAction::act()
{
  std::set<SubdomainID> blocks;

  std::vector<SubdomainName> block_param = getParam<std::vector<SubdomainName> >("block");
  for (std::vector<SubdomainName>::iterator it = block_param.begin(); it != block_param.end(); ++it)
    blocks.insert(_problem->mesh().getSubdomainID(*it));

  _problem->addAuxVariable("bounds_dummy",
                        FEType(Utility::string_to_enum<Order>(_order),
                                    Utility::string_to_enum<FEFamily>(_family)),
                         &blocks);
}
