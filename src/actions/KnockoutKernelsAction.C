#include "KnockoutKernelsAction.h"
#include "Factory.h"
#include "Parser.h"
#include "FEProblem.h"
#include "BuckUtils.h"

template<>
InputParameters validParams<KnockoutKernelsAction>()
{
  InputParameters params = validParams<ClustersActionBase>();

  params.addParam<Real>("parameter", 1, "Re-solution parameter");
  params.addParam<VariableName>("fission_rate", "Fission rate variable name");
  params.addParam<bool>("use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");

  return params;
}

KnockoutKernelsAction::KnockoutKernelsAction(const std::string & name,
                                                             InputParameters params) :
  ClustersActionBase(name, params)
{
}

void
KnockoutKernelsAction::act()
{
  for (unsigned int g = 0; g<_G; ++g)
  {
    std::string var_name = _vars[g];
    std::string orig_kernel = "Knockout";

    InputParameters p = _factory.getValidParams(orig_kernel);
    p.set<NonlinearVariableName>("variable") = var_name;
    p.set<std::vector<VariableName> >("coupled_conc") = _vars;
    p.set<std::vector<Real> >("coupled_conc_atoms") = _atoms;
    p.set<int>("g") = g+1;
    p.set<Real>("parameter") = getParam<Real>("parameter");

    if ( isParamValid("fission_rate") )
    {
      p.addCoupledVar("fission_rate", "");
      p.set<std::vector<VariableName> >("fission_rate") = std::vector<VariableName>(1, getParam<VariableName>("fission_rate"));
    }

    p.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");

    std::string kernel_name = var_name;
    kernel_name.append("_knockout");

    _problem->addKernel(orig_kernel, kernel_name, p);
  }
}
