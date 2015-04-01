#include "BubblesGrowthKernelsAction.h"

#include "Factory.h"
#include "FEProblem.h"

template<>
InputParameters validParams<BubblesGrowthKernelsAction>()
{
  InputParameters params = validParams<BubblesActionBase>();

  params.addRequiredParam<VariableName>("temp", "The temperature variable name");
  params.addParam<bool>("use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");

  params.addParam<bool>("allow_loss", false, "Flag to allow losses from the largest bubble group.");

  return params;
}

BubblesGrowthKernelsAction::BubblesGrowthKernelsAction(const std::string & name, InputParameters params) :
  BubblesActionBase(name, params)
{
}


void
BubblesGrowthKernelsAction::act()
{
  for (int g=0; g<_G; ++g)
  {
    std::string var_name = _c[g];

    InputParameters p = _factory.getValidParams("BubbleGrowth");
    p.set<NonlinearVariableName>("variable") = var_name;
    p.set<bool>("allow_loss") = getParam<bool>("allow_loss");
    p.set<std::vector<VariableName> >("coupled_conc") = _c;
    p.set<std::vector<VariableName> >("coupled_rad") = _r;
    p.set<std::vector<Real> >("coupled_atoms") = _atoms;

    p.addCoupledVar("temp", "");
    p.set<std::vector<VariableName> >("temp") = std::vector<VariableName>(1, getParam<VariableName>("temp"));

    std::string kernel_name = var_name;
    kernel_name.append("_growth");

    _problem->addKernel("BubbleGrowth", kernel_name, p);
  }
}
