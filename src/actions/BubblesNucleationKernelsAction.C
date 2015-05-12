#include "BubblesNucleationKernelsAction.h"

#include "Factory.h"
#include "FEProblem.h"

template<>
InputParameters validParams<BubblesNucleationKernelsAction>()
{
  InputParameters params = validParams<BubblesActionBase>();

  params.addRequiredParam<VariableName>("temp", "The temperature variable name");
  params.addParam<bool>("use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");
  params.addParam<Real>("a", 4.96e-4, "Lattice Parameter [um]");
  params.addParam<Real>("omega", 1.53e-11, " Atomic volume [um^3]");

  return params;
}

BubblesNucleationKernelsAction::BubblesNucleationKernelsAction(const std::string & name,
                                                             InputParameters params) :
  BubblesActionBase(name, params)
{
}

void
BubblesNucleationKernelsAction::act()
{
  for (unsigned int n = 0; n < 2; ++n)
  {
    std::string var_name = _c[n];

    InputParameters p = _factory.getValidParams("BubbleNucleation");

    p.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");
    p.set<Real>("a") = getParam<Real>("a");
    p.set<Real>("omega") = getParam<Real>("omega");

    p.set<NonlinearVariableName>("variable") = var_name;
    p.set<std::vector<VariableName> >("coupled_conc") = _c;
    p.set<std::vector<VariableName> >("coupled_rad") = _r;
    p.set<std::vector<Real> >("coupled_atoms") = _atoms;
    p.set<std::vector<Real> >("coupled_widths") = _widths;

    p.addCoupledVar("temp", "");
    p.set<std::vector<VariableName> >("temp") = std::vector<VariableName>(1, getParam<VariableName>("temp"));

    std::string kernel_name = var_name;
    kernel_name.append("_nucleation");

    _problem->addKernel("BubbleNucleation", kernel_name, p);
  }
}
