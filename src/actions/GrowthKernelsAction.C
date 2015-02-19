#include "GrowthKernelsAction.h"

#include "Factory.h"
#include "FEProblem.h"

template<>
InputParameters validParams<GrowthKernelsAction>()
{
  InputParameters params = validParams<ClustersActionBase>();

  params.addRequiredParam<NonlinearVariableName>("temp", "The temperature variable name");
  params.addParam<bool>("use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");

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
  for (int g=0; g<_G; ++g)
  {
    if ( g == 0 || g>=_N_nuc-1 )
    {
      std::string var_name = _vars[g];

      InputParameters p = _factory.getValidParams("SinkGrowth");
      p.set<NonlinearVariableName>("variable") = var_name;
      p.set<std::vector<VariableName> >("coupled_conc") = _vars;
      p.set<std::vector<Real> >("coupled_conc_atoms") = _atoms;
      p.set<int>("N_nuc") = _N_nuc;

      p.addCoupledVar("temp", "");
      p.set<std::vector<VariableName> >("temp") = std::vector<VariableName>(1, getParam<NonlinearVariableName>("temp"));

      p.set<int>("g") = g+1;
      p.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");

      std::string kernel_name = var_name;
      kernel_name.append("_growth");

      _problem->addKernel("SinkGrowth", kernel_name, p);
    }
  }
}
