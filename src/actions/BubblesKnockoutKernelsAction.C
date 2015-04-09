#include "BubblesKnockoutKernelsAction.h"

#include "Factory.h"
#include "FEProblem.h"

template<>
InputParameters validParams<BubblesKnockoutKernelsAction>()
{
  InputParameters params = validParams<BubblesActionBase>();

  params.addRequiredParam<VariableName>("fission_rate", "The fission_rate variable name");
  params.addParam<Real>("factor", 1, "Scaling factor");
  params.addParam<bool>("use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");

  params.addParam<bool>("include_c1", true, "Flag to create growth kernel for c1");
  params.addParam<bool>("include_c2", true, "Flag to create growth kernel for c2");
  params.addParam<bool>("constant_b", false, "Flag to hold b at constant value");

  return params;
}

BubblesKnockoutKernelsAction::BubblesKnockoutKernelsAction(const std::string & name, InputParameters params) :
  BubblesActionBase(name, params),
  _include_c1(getParam<bool>("include_c1")),
  _include_c2(getParam<bool>("include_c2"))
{
}


void
BubblesKnockoutKernelsAction::act()
{
  for (int g=0; g<_G; ++g)
  {
    if ( g==0 && !_include_c1 )
      continue;

    if ( g==1 && !_include_c2 )
      continue;

    std::string var_name = _c[g];

    InputParameters p = _factory.getValidParams("BubbleKnockout");
    p.set<NonlinearVariableName>("variable") = var_name;
    p.set<Real>("factor") = getParam<Real>("factor");
    p.set<bool>("constant_b") = getParam<bool>("constant_b");
    p.set<std::vector<VariableName> >("coupled_conc") = _c;
    p.set<std::vector<VariableName> >("coupled_rad") = _r;
    p.set<std::vector<Real> >("coupled_atoms") = _atoms;
    p.set<std::vector<Real> >("coupled_widths") = _widths;

    p.addCoupledVar("fission_rate", "");
    p.set<std::vector<VariableName> >("fission_Rate") = std::vector<VariableName>(1, getParam<VariableName>("fission_rate"));

    std::string kernel_name = var_name;
    kernel_name.append("_knockout");

    _problem->addKernel("BubbleKnockout", kernel_name, p);
  }
}
