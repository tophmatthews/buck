#include "GrowthVarsAction.h"

#include "Factory.h"
#include "FEProblem.h"
#include "Parser.h"
#include "libmesh/string_to_enum.h"
#include "MooseApp.h"
#include "BuckUtils.h"

template<>
InputParameters validParams<GrowthVarsAction>()
{
  InputParameters params = validParams<Action>();

  params.addParam<std::string>("order", "FIRST",  "Specifies the order of the FE shape function to use for this variable");
  params.addParam<std::string>("family", "LAGRANGE", "Specifies the family of FE shape functions to use for this variable");
  params.addParam<Real>("scaling", 1.0, "Specifies a scaling factor to apply");
  params.addParam<std::string>("var_name_base", "c", "specifies the base name of the variables");
  params.addRequiredParam<int>("N_nuc", "Smallest cluster size for growth model inclusion");
  params.addRequiredParam<int>("N", "Largest cluster size for growth model inclusion");

  return params;
}

GrowthVarsAction::GrowthVarsAction(const std::string & name,
                                                   InputParameters params) :
  Action(name, params),
  _order(getParam<std::string>("order")),
  _family(getParam<std::string>("family")),
  _var_name_base(getParam<std::string>("var_name_base")),
  _N_min(getParam<int>("N_nuc")),
  _N_max(getParam<int>("N"))
{
  Buck::varNamesFromN( _vars, _var_name_base, _N_max, _N_min);
}

void
GrowthVarsAction::act()
{
  for (unsigned i = 0; i < _vars.size(); ++i)
  {
    _problem->addVariable(_vars[i],
                          FEType(Utility::string_to_enum<Order>(_order),
                                 Utility::string_to_enum<FEFamily>(_family)),
                          getParam<Real>("scaling"));
  }

}
