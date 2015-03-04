#include "ClustersTimeKernelAction.h"

#include "Factory.h"
#include "FEProblem.h"

template<>
InputParameters validParams<ClustersTimeKernelAction>()
{
  InputParameters params = validParams<ClustersActionBase>();

  params.addParam<bool>("use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");
  params.addParam<bool>("transient", true, "Flag to determine if TimeDerivative kernels should be made for nucleation concentration variables");

  return params;
}

ClustersTimeKernelAction::ClustersTimeKernelAction(const std::string & name,
                                                             InputParameters params) :
  ClustersActionBase(name, params)
{
}

void
ClustersTimeKernelAction::act()
{
  if ( getParam<bool>("transient") )
  {
    for (unsigned int g = 0; g < _N; ++g)
    {
      InputParameters poly_params = _factory.getValidParams("TimeDerivative");

      poly_params.set<NonlinearVariableName>("variable") = _vars[g];
      poly_params.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");

      std::string kernel_name = _vars[g];
      kernel_name.append("_time");

      _problem->addKernel("TimeDerivative", kernel_name, poly_params);
    }
  }
}
