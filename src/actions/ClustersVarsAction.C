#include "ClustersVarsAction.h"

#include "FEProblem.h"
#include "libmesh/string_to_enum.h"
#include "MooseApp.h"

template<>
InputParameters validParams<ClustersVarsAction>()
{
  InputParameters params = validParams<ClustersActionBase>();

  params.addParam<std::string>("order", "FIRST",  "Specifies the order of the FE shape function to use for this variable");
  params.addParam<std::string>("family", "LAGRANGE", "Specifies the family of FE shape functions to use for this variable");
  params.addParam<Real>("scaling", 1.0, "Specifies a scaling factor to apply to the L variables");

  return params;
}

ClustersVarsAction::ClustersVarsAction(const std::string & name,
                                                   InputParameters params) :
  ClustersActionBase(name, params),
  _order(getParam<std::string>("order")),
  _family(getParam<std::string>("family"))
{
}

void
ClustersVarsAction::act()
{
  for (unsigned i = 0; i < _vars.size(); ++i)
  {
    _problem->addVariable(_vars[i],
                          FEType(Utility::string_to_enum<Order>(_order),
                                 Utility::string_to_enum<FEFamily>(_family)),
                          getParam<Real>("scaling"));
  }

}
