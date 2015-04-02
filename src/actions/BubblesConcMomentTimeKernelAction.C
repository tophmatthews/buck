#include "BubblesConcMomentTimeKernelAction.h"

#include "Factory.h"
#include "FEProblem.h"

template<>
InputParameters validParams<BubblesConcMomentTimeKernelAction>()
{
  InputParameters params = validParams<BubblesActionBase>();

  params.addParam<bool>("use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");
  params.addParam<bool>("transient", true, "Flag to determine if TimeDerivative kernels should be made for nucleation concentration variables");

  return params;
}

BubblesConcMomentTimeKernelAction::BubblesConcMomentTimeKernelAction(const std::string & name,
                                                             InputParameters params) :
  BubblesActionBase(name, params)
{
}

void
BubblesConcMomentTimeKernelAction::act()
{
  if ( getParam<bool>("transient") )
  {
    for (unsigned int g = 0; g < _G; ++g)
    {
      InputParameters poly_params = _factory.getValidParams("TimeDerivative");

      poly_params.set<NonlinearVariableName>("variable") = _m[g];
      poly_params.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");

      std::string kernel_name = _m[g];
      kernel_name.append("_time");

      _problem->addKernel("TimeDerivative", kernel_name, poly_params);
    }
  }
}
