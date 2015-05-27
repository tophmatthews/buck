#include "BubblesRadAuxKernelAction.h"

#include "FEProblem.h"
#include "Factory.h"

template<>
InputParameters validParams<BubblesRadAuxKernelAction>()
{
	InputParameters params = validParams<BubblesActionBase>();

  params.addRequiredParam<NonlinearVariableName>("temp", "The temperature variable name");
  params.addRequiredParam<std::vector<SubdomainName> >("block", "The blocks where bounds should be applied.");

  return params;
}


BubblesRadAuxKernelAction::BubblesRadAuxKernelAction(const std::string & name, InputParameters params) :
  BubblesActionBase(name, params),
  _blocks(getParam<std::vector<SubdomainName> >("block"))
{
}


void
BubblesRadAuxKernelAction::act()
{
  for ( unsigned int i=0; i<_G; ++i)
  {
    InputParameters params = _factory.getValidParams("EquilibriumRadiusAux");

    params.set<std::vector<SubdomainName> >("block") = _blocks;
    params.set<AuxVariableName>("variable") = _r[i];
    params.set<Real>("m") = _atoms[i];

    params.addCoupledVar("temp", "");
    params.set<std::vector<VariableName> >("temp") = std::vector<VariableName>(1, getParam<NonlinearVariableName>("temp"));

    std::string kernel_name = _r[i];
    kernel_name.append("_eq");
    _problem->addAuxKernel("EquilibriumRadiusAux", kernel_name, params);
  }
}
