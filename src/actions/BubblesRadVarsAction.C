#include "BubblesRadVarsAction.h"

#include "FEProblem.h"
#include "libmesh/string_to_enum.h"

template<>
InputParameters validParams<BubblesRadVarsAction>()
{
  InputParameters params = validParams<BubblesActionBase>();

  params.addParam<std::string>("order", "FIRST",  "Specifies the order of the FE shape function to use for this variable");
  params.addParam<std::string>("family", "LAGRANGE", "Specifies the family of FE shape functions to use for this variable");
  params.addParam<Real>("scaling", 1.0, "Specifies a scaling factor to apply to the variables");

  return params;
}

BubblesRadVarsAction::BubblesRadVarsAction(const std::string & name,
                                                   InputParameters params) :
  BubblesActionBase(name, params),
  _order(getParam<std::string>("order")),
  _family(getParam<std::string>("family"))
{
}

void
BubblesRadVarsAction::act()
{
  if (_current_task == "add_variable")
  {
    for (unsigned int i = 0; i < _G; ++i)
    {
      Real scale = 1.0;
      _problem->addVariable(_c[i],
                            FEType(Utility::string_to_enum<Order>(_order),
                                   Utility::string_to_enum<FEFamily>(_family)),
                            scale);
    }
  }
}
