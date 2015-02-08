#include "NucleationVarsAction.h"

#include "Factory.h"
#include "FEProblem.h"
#include "Parser.h"
#include "libmesh/string_to_enum.h"
#include "MooseApp.h"

template<>
InputParameters validParams<NucleationVarsAction>()
{
  InputParameters params = validParams<Action>();

  params.addParam<std::string>("order", "FIRST",  "Specifies the order of the FE shape function to use for this variable");
  params.addParam<std::string>("family", "LAGRANGE", "Specifies the family of FE shape functions to use for this variable");
  params.addParam<Real>("scaling", 1.0, "Specifies a scaling factor to apply to the L variables");
  params.addParam<int>("N", 2, "Largest cluster size");

  return params;
}

NucleationVarsAction::NucleationVarsAction(const std::string & name,
                                                   InputParameters params) :
  Action(name, params),
  _order(getParam<std::string>("order")),
  _family(getParam<std::string>("family")),
  _N(getParam<int>("N"))

{
}

void
NucleationVarsAction::act()
{
  std::vector<std::string> var_value;

  const char* args[] = {"c2", "c3", "c4", "c5", "c6", "c7", "c8", "c9"};
  std::vector<std::string> names(args, args + 8);

  for ( int i=0; i<_N-1; ++i)
  {
    var_value.push_back(names[i]);
  }

  for (unsigned i = 0; i < var_value.size(); ++i)
  {

    _problem->addVariable(var_value[i],
                          FEType(Utility::string_to_enum<Order>(_order),
                                 Utility::string_to_enum<FEFamily>(_family)),
                          getParam<Real>("scaling"));
  }

}
