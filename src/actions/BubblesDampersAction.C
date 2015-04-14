#include "BubblesDampersAction.h"

#include "FEProblem.h"

template<>
InputParameters validParams<BubblesDampersAction>()
{
	InputParameters params = validParams<BubblesActionBase>();

  params.addParam<Real>("damping", 0.1, "The maximum newton increment.");
  params.addParam<int>("starting_index", 1, "Variable list index at which to start applying damper.");

  return params;
}

BubblesDampersAction::BubblesDampersAction(const std::string & name, InputParameters params) :
  BubblesActionBase(name, params),
  _index(getParam<int>("starting_index"))
{
  if (_index > _G)
    mooseError("In BubblesDamperAction: starting_index must be less than then the total number of groups, G");
}

void
BubblesDampersAction::act()
{
  for (int g=_index; g<_G; ++g)
  {
    std::string base_kernel = "PositiveDamper";

    InputParameters p = _factory.getValidParams(base_kernel);
    std::string var_name = _c[g];
    std::string kernel_name = var_name;
    kernel_name.append(base_kernel);

    // BubbleBase
    p.set<NonlinearVariableName>("variable") = var_name;
    p.set<std::vector<VariableName> >("coupled_conc") = _c;
    p.set<std::vector<VariableName> >("coupled_rad") = _r;
    p.set<std::vector<Real> >("coupled_atoms") = _atoms;
    p.set<std::vector<Real> >("coupled_widths") = _widths;

    // BubbleGrowth
    p.set<Real>("damping") = getParam<Real>("damping");

    _problem->addDamper(base_kernel, kernel_name, p);
  }
}
