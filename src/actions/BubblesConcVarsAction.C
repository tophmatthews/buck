#include "BubblesConcVarsAction.h"

#include "FEProblem.h"
#include "libmesh/string_to_enum.h"

template<>
InputParameters validParams<BubblesConcVarsAction>()
{
  InputParameters params = validParams<BubblesActionBase>();

  params.addParam<std::string>("order", "FIRST",  "Specifies the order of the FE shape function to use for this variable");
  params.addParam<std::string>("family", "LAGRANGE", "Specifies the family of FE shape functions to use for this variable");
  params.addParam<Real>("scaling", 1.0, "Specifies a scaling factor to apply to the variables");
  params.addParam<Real>("c1_initial_condition", "Specifies a initial condtion to apply to c1");
  params.addParam<Real>("c2_initial_condition", "Specifies a initial condtion to apply to c2");
  params.addParam<Real>("initial_condition", "Specifies a initial condtion apply the rest of the variables");

  return params;
}

BubblesConcVarsAction::BubblesConcVarsAction(InputParameters params) :
  BubblesActionBase(params),
  _order(getParam<std::string>("order")),
  _family(getParam<std::string>("family")),
  _ic(getParam<Real>("initial_condition"))
{
}

void
BubblesConcVarsAction::act()
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
  else if (_current_task == "add_ic" && isParamValid("initial_condition"))
  {
    for (unsigned int i = 0; i < _G; ++i)
    {
      InputParameters poly_params = _factory.getValidParams("ConstantIC");
      poly_params.set<VariableName>("variable") = _c[i];
      if ( i == 0 )
        poly_params.set<Real>("value") = getParam<Real>("c1_initial_condition");
      else if ( i == 1 )
        poly_params.set<Real>("value") = getParam<Real>("c2_initial_condition");
      else
      {
        poly_params.set<Real>("value") = _ic / _widths[i];
      }
      _problem->addInitialCondition("ConstantIC", "Initialize_" + 1+i, poly_params);
    }
  }
}
