#include "BubblesRadAuxVarsAction.h"

#include "FEProblem.h"
#include "libmesh/string_to_enum.h"

template<>
InputParameters validParams<BubblesRadAuxVarsAction>()
{
  InputParameters params = validParams<BubblesActionBase>();

  params.addParam<std::string>("order", "FIRST",  "Specifies the order of the FE shape function to use for this variable");
  params.addParam<std::string>("family", "LAGRANGE", "Specifies the family of FE shape functions to use for this variable");
  params.addRequiredParam<std::vector<SubdomainName> >("block", "The block id where this variable lives");

  return params;
}

BubblesRadAuxVarsAction::BubblesRadAuxVarsAction(InputParameters params) :
  BubblesActionBase(params),
  _order(getParam<std::string>("order")),
  _family(getParam<std::string>("family"))
{
  mooseAssert(!getParam<std::vector<SubdomainName> >("block").empty(), "Blocks must be specified in BubblesRadAuxVarsAction");
}

void
BubblesRadAuxVarsAction::act()
{
  std::set<SubdomainID> blocks;

  std::vector<SubdomainName> block_param = getParam<std::vector<SubdomainName> >("block");
  for (std::vector<SubdomainName>::iterator it = block_param.begin(); it != block_param.end(); ++it)
    blocks.insert(_problem->mesh().getSubdomainID(*it));

  for ( unsigned int i=0; i<_G; ++i )
  {
    _problem->addAuxVariable(_r[i],
                          FEType(Utility::string_to_enum<Order>(_order),
                                      Utility::string_to_enum<FEFamily>(_family)),
                           &blocks);
  }
}
