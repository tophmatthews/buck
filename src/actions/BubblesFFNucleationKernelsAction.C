#include "BubblesFFNucleationKernelsAction.h"

#include "Factory.h"
#include "FEProblem.h"

template<>
InputParameters validParams<BubblesFFNucleationKernelsAction>()
{
  InputParameters params = validParams<BubblesActionBase>();

  params.addRequiredParam<VariableName>("fission_rate", "The fission rate density variable name");
  params.addParam<bool>("use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");
  params.addParam<int>("number", 5, "Number of bubbles created per fission");
  params.addParam<int>("size", 4, "Size of bubbles created");
  params.addParam<Real>("factor", 1.0, "User supplied multiplier.");
  params.addParam<Real>("upper", 1e7, "Upper dead-band limit");
  params.addParam<Real>("lower", 1e6, "Lower dead-band limit");

  return params;
}

BubblesFFNucleationKernelsAction::BubblesFFNucleationKernelsAction(const std::string & name,
                                                             InputParameters params) :
  BubblesActionBase(name, params)
{
}

void
BubblesFFNucleationKernelsAction::act()
{
  int n=0;
  {
    std::string var_name = _c[n];

    InputParameters p = _factory.getValidParams("BubbleFFNucleation");

    p.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");

    p.set<Real>("factor") = getParam<Real>("factor");
    p.set<int>("number") = getParam<int>("number");
    p.set<int>("size") = getParam<int>("size");
    p.set<Real>("upper") = getParam<Real>("upper");
    p.set<Real>("lower") = getParam<Real>("lower");

    p.set<NonlinearVariableName>("variable") = var_name;
    p.set<std::vector<VariableName> >("coupled_conc") = _c;
    p.set<std::vector<VariableName> >("coupled_rad") = _r;
    p.set<std::vector<Real> >("coupled_atoms") = _atoms;
    p.set<std::vector<Real> >("coupled_widths") = _widths;

    p.addCoupledVar("fission_rate", "");
    p.set<std::vector<VariableName> >("fission_rate") = std::vector<VariableName>(1, getParam<VariableName>("fission_rate"));

    std::string kernel_name = var_name;
    kernel_name.append("_nucleation");

    _problem->addKernel("BubbleFFNucleation", kernel_name, p);
  }

  n = getParam<int>("size")-1;
  {
    std::string var_name = _c[n];

    InputParameters p = _factory.getValidParams("BubbleFFNucleation");

    p.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");

    p.set<Real>("factor") = getParam<Real>("factor");
    p.set<int>("number") = getParam<int>("number");
    p.set<int>("size") = getParam<int>("size");

    p.set<NonlinearVariableName>("variable") = var_name;
    p.set<std::vector<VariableName> >("coupled_conc") = _c;
    p.set<std::vector<VariableName> >("coupled_rad") = _r;
    p.set<std::vector<Real> >("coupled_atoms") = _atoms;
    p.set<std::vector<Real> >("coupled_widths") = _widths;

    p.addCoupledVar("fission_rate", "");
    p.set<std::vector<VariableName> >("fission_rate") = std::vector<VariableName>(1, getParam<VariableName>("fission_rate"));

    std::string kernel_name = var_name;
    kernel_name.append("_nucleation");

    _problem->addKernel("BubbleFFNucleation", kernel_name, p);
  }
}
