#include "BubblesGrowthKernelsAction.h"

#include "FEProblem.h"

template<>
InputParameters validParams<BubblesGrowthKernelsAction>()
{
  InputParameters params = validParams<BubblesActionBase>();

  params.addRequiredParam<VariableName>("temp", "The temperature variable name");

  params.addParam<bool>("allow_loss", false, "Flag to allow losses from the largest bubble group.");
  params.addParam<bool>("include_c1", true, "Flag to create growth kernel for c1");
  params.addParam<bool>("include_c2", true, "Flag to create growth kernel for c2");

  return params;
}

BubblesGrowthKernelsAction::BubblesGrowthKernelsAction(InputParameters params) :
  BubblesActionBase(params),
  _include_c1(getParam<bool>("include_c1")),
  _include_c2(getParam<bool>("include_c2"))
{
}


void
BubblesGrowthKernelsAction::act()
{
  for (int g=0; g<_G; ++g)
  {
    if ( g==0 && !_include_c1 )
      continue;

    if ( g==1 && !_include_c2 )
      continue;

    std::string base_kernel = "BubbleGrowth";

    InputParameters p = _factory.getValidParams(base_kernel);
    std::string var_name = _c[g];
    std::string kernel_name = var_name;
    kernel_name.append(base_kernel);

    // BubbleBase variables
    p.set<NonlinearVariableName>("variable") = var_name;
    p.set<std::vector<VariableName> >("coupled_conc") = _c;
    p.set<std::vector<VariableName> >("coupled_rad") = _r;
    p.set<std::vector<Real> >("coupled_atoms") = _atoms;
    p.set<std::vector<Real> >("coupled_widths") = _widths;

    // BubbleGrowth variables
    p.set<bool>("allow_loss") = getParam<bool>("allow_loss");
    p.set<bool>("experimental") = _exp;
    p.addCoupledVar("temp", "");
    p.set<std::vector<VariableName> >("temp") = std::vector<VariableName>(1, getParam<VariableName>("temp"));

    _problem->addKernel(base_kernel, kernel_name, p);
  }
}
