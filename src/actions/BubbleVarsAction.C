#include "BubbleVarsAction.h"

#include "Factory.h"
#include "FEProblem.h"
#include "Parser.h"
#include "libmesh/string_to_enum.h"
#include "MooseApp.h"

template<>
InputParameters validParams<BubbleVarsAction>()
{
  InputParameters params = validParams<Action>();

  params.addParam<std::string>("order", "CONSTANT",  "Specifies the order of the FE shape function to use for this variable");
  params.addParam<std::string>("family", "MONOMIAL", "Specifies the family of FE shape functions to use for this variable");
  params.addRequiredParam<std::vector<SubdomainName> >("block", "The block id where this variable lives");
  params.addRequiredParam<Real>("R_min", "", "Specifies the minimum bubble radius [nm]");
  params.addRequiredParam<Real>("R_max", "", "Specifies the maximum bubble radius [nm].");
  params.addREquiredParam<Real>("G", "", "Specifies the number of groups.");

  return params;
}

BubbleVarsAction::BubbleVarsAction(const std::string & name,
                                                   InputParameters params) :
  Action(name, params),
  _order(getParam<std::string>("order")),
  _family(getParam<std::string>("family")),
  _R_min(getParam<Real>("R_min")),
  _R_max(getParam<Real>("R_max")),
  _G(getParam<Real>("G"))
{
  mooseAssert(!getParam<std::vector<SubdomainName> >("block").empty(), "Blocks must be specified in BubbleVarsAction");
}

void
BubbleVarsAction::act()
{
  std::set<SubdomainID> blocks;

  std::vector<SubdomainName> block_param = getParam<std::vector<SubdomainName> >("block");
  for (std::vector<SubdomainName>::iterator it = block_param.begin(); it != block_param.end(); ++it)
    blocks.insert(_problem->mesh().getSubdomainID(*it));

  std::vector<std::string> var_value;
  var_value.push_back("burnup"); // These always included by default
  var_value.push_back("fission_rate");

// Something here to determine # of blocks...don't know what yet.

  for (unsigned i = 0; i < var_value.size(); ++i)
  {

    _problem->addAuxVariable(var_value[i],
                             FEType(Utility::string_to_enum<Order>(_order),
                                    Utility::string_to_enum<FEFamily>(_family)),
                             &blocks);
  }

}
