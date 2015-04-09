#include "BubblesKnockoutKernelsAction.h"

#include "Factory.h"
#include "FEProblem.h"
#include "BuckUtils.h"

template<>
InputParameters validParams<BubblesKnockoutKernelsAction>()
{
  InputParameters params = validParams<BubblesActionBase>();

  params.addParam<bool>("use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");

  params.addRequiredParam<VariableName>("fission_rate", "The fission_rate variable name");
  params.addParam<Real>("factor", 1, "Scaling factor");
  params.addParam<Real>("b", -1, "Value to set constant knockout parameter. B is automatically calculated if not given.");

  return params;
}

BubblesKnockoutKernelsAction::BubblesKnockoutKernelsAction(const std::string & name, InputParameters params) :
  BubblesActionBase(name, params)
{
}


void
BubblesKnockoutKernelsAction::act()
{
  for (int g=0; g<_G; ++g)
  {
    std::string base_kernel = "BubbleKnockout";

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

    // BubbleKnockout
    p.set<Real>("factor") = getParam<Real>("factor");
    p.set<Real>("b") = getParam<Real>("b");
    p.addCoupledVar("fission_rate", "");
    p.set<std::vector<VariableName> >("fission_rate") = std::vector<VariableName>(1, getParam<VariableName>("fission_rate"));


    _problem->addKernel(base_kernel, kernel_name, p);
  }
}
