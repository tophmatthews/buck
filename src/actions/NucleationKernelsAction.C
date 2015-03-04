#include "NucleationKernelsAction.h"

#include "Factory.h"
#include "FEProblem.h"

template<>
InputParameters validParams<NucleationKernelsAction>()
{
  InputParameters params = validParams<ClustersActionBase>();

  params.addParam<bool>("use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");

  return params;
}

NucleationKernelsAction::NucleationKernelsAction(const std::string & name,
                                                             InputParameters params) :
  ClustersActionBase(name, params)
{
}

void
NucleationKernelsAction::act()
{
  // for (unsigned int n = 0; n < _N_nuc; ++n)
  // {
  //   std::string var_name = _vars[n];

  //   InputParameters poly_params = _factory.getValidParams("HomNucleation");

  //   poly_params.set<NonlinearVariableName>("variable") = var_name;
  //   poly_params.set<std::vector<VariableName> >("coupled_conc") = _vars;
  //   poly_params.set<int>("g") = n+1;
  //   poly_params.set<int>("N_nuc") = _N_nuc;
  //   poly_params.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");

  //   std::string kernel_name = var_name;
  //   kernel_name.append("_nucleation");

  //   _problem->addKernel("HomNucleation", kernel_name, poly_params);
  // }
}
