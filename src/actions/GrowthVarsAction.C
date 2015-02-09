#include "GrowthVarsAction.h"

#include "Factory.h"
#include "FEProblem.h"
#include "Parser.h"
#include "libmesh/string_to_enum.h"
#include "MooseApp.h"

template<>
InputParameters validParams<GrowthVarsAction>()
{
  InputParameters params = validParams<Action>();

  params.addParam<std::string>("order", "FIRST",  "Specifies the order of the FE shape function to use for this variable");
  params.addParam<std::string>("family", "LAGRANGE", "Specifies the family of FE shape functions to use for this variable");
  params.addParam<Real>("scaling", 1.0, "Specifies a scaling factor to apply to the L variables");
  params.addParam<std::string>("var_name_base", "c", "specifies the base name of the variables");
  params.addParam<int>("N_min", 4, "Smallest cluster size for growth model inclusion");
  params.addParam<int>("N_max", 10, "Largest cluster size for growth model inclusion");
  // params.addParam<Real>("N_min_ic", 0, "Initial condition for N_min concentration");

  return params;
}

GrowthVarsAction::GrowthVarsAction(const std::string & name,
                                                   InputParameters params) :
  Action(name, params),
  _order(getParam<std::string>("order")),
  _family(getParam<std::string>("family")),
  _var_name_base(getParam<std::string>("var_name_base")),
  _N_min(getParam<int>("N_min")),
  _N_max(getParam<int>("N_max"))
{
}

void
GrowthVarsAction::act()
{
  for (unsigned i = _N_min; i < _N_max+1; ++i)
  {
    // Create variable names
    std::string var_name = _var_name_base;
    std::stringstream out;
    out << i;
    var_name.append(out.str());

    // Add variable
    _problem->addVariable(var_name,
                          FEType(Utility::string_to_enum<Order>(_order),
                                 Utility::string_to_enum<FEFamily>(_family)),
                          getParam<Real>("scaling"));
  }

}
