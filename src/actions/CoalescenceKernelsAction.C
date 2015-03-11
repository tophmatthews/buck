#include "CoalescenceKernelsAction.h"

#include "Factory.h"
#include "FEProblem.h"

template<>
InputParameters validParams<CoalescenceKernelsAction>()
{
  InputParameters params = validParams<BubblesActionBase>();

  params.addRequiredParam<NonlinearVariableName>("temp", "The temperature variable name");
  params.addParam<bool>("use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");

  return params;
}

CoalescenceKernelsAction::CoalescenceKernelsAction(const std::string & name, InputParameters params) :
  BubblesActionBase(name, params)
{
}


void
CoalescenceKernelsAction::act()
{
  for (int g=0; g<_G; ++g)
  {
    std::string var_name = _c[g];

    InputParameters p = _factory.getValidParams("BubbleCoalescence");
    p.set<NonlinearVariableName>("variable") = var_name;
    p.set<std::vector<VariableName> >("coupled_conc") = _c;
    p.set<std::vector<VariableName> >("coupled_rad") = _r;
    p.set<std::vector<Real> >("coupled_atoms") = _atoms;

    p.addCoupledVar("temp", "");
    p.set<std::vector<VariableName> >("temp") = std::vector<VariableName>(1, getParam<NonlinearVariableName>("temp"));

    std::string kernel_name = var_name;
    kernel_name.append("_growth");

    _problem->addKernel("BubbleCoalescence", kernel_name, p);
  }
}
