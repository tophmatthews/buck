#include "GrowthKernelsAction.h"

#include "Factory.h"
#include "FEProblem.h"

template<>
InputParameters validParams<GrowthKernelsAction>()
{
  InputParameters params = validParams<ClustersActionBase>();

  params.addRequiredParam<NonlinearVariableName>("temp", "The temperature variable name");
  params.addParam<bool>("use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");
  params.addParam<Real>("a", "Lattice paramter");
  params.addParam<Real>("omega", "Atomic volume");

  return params;
}

GrowthKernelsAction::GrowthKernelsAction(const std::string & name,
                                                             InputParameters params) :
  ClustersActionBase(name, params)
{
}


void
GrowthKernelsAction::act()
{
  for (int g=0; g<_N; ++g)
  {
    std::string var_name = _vars[g];

    InputParameters p = _factory.getValidParams("SinkGrowth");
    p.set<NonlinearVariableName>("variable") = var_name;
    p.set<std::vector<VariableName> >("coupled_conc") = _vars;
    p.set<int>("M") = _M;
    p.set<int>("N") = _N;
    p.set<Real>("a") = getParam<Real>("a");
    p.set<Real>("omega") = getParam<Real>("omega");

    p.addCoupledVar("temp", "");
    p.set<std::vector<VariableName> >("temp") = std::vector<VariableName>(1, getParam<NonlinearVariableName>("temp"));

    std::string kernel_name = var_name;
    kernel_name.append("_growth");

    _problem->addKernel("SinkGrowth", kernel_name, p);
  }
}
