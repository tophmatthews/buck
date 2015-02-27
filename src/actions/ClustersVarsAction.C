#include "ClustersVarsAction.h"

#include "FEProblem.h"
#include "libmesh/string_to_enum.h"

template<>
InputParameters validParams<ClustersVarsAction>()
{
  InputParameters params = validParams<ClustersActionBase>();

  params.addParam<std::string>("order", "FIRST",  "Specifies the order of the FE shape function to use for this variable");
  params.addParam<std::string>("family", "LAGRANGE", "Specifies the family of FE shape functions to use for this variable");
  params.addParam<Real>("scaling", 1.0, "Specifies a scaling factor to apply to the L variables");
  params.addParam<Real>("initial_condition", "Specifies a scaling factor to apply to the L variables");

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
  if (_current_task == "add_variable")
  {
    for (unsigned int i = 0; i < _vars.size(); ++i)
    {
      Real scale = 1 + i*20;
      _problem->addVariable(_vars[i],
                            FEType(Utility::string_to_enum<Order>(_order),
                                   Utility::string_to_enum<FEFamily>(_family)),
                            scale);
    }
  }
  else if (_current_task == "add_ic" && isParamValid("initial_condition"))
  {
    // Set Initial Conditions

    for (unsigned int i = 0; i < _vars.size(); ++i)
    {
      InputParameters poly_params = _factory.getValidParams("ConstantIC");
      poly_params.set<VariableName>("variable") = _vars[i];
      // if ( i ==0 || i == 1)
      //   poly_params.set<Real>("value") = 1;
      // else
      poly_params.set<Real>("value") = getParam<Real>("initial_condition");
      _problem->addInitialCondition("ConstantIC", "Initialize_" + 1+i, poly_params);
    }
  }
}
