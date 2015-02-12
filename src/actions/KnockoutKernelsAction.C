#include "KnockoutKernelsAction.h"
#include "Factory.h"
#include "Parser.h"
#include "FEProblem.h"
#include "BuckUtils.h"

template<>
InputParameters validParams<KnockoutKernelsAction>()
{
  InputParameters params = validParams<Action>();

  params.addParam<std::string>("var_name_base", "c", "specifies the base name of the variables");
  params.addRequiredParam<int>("N", "Largest nucleation cluster size");
  params.addParam<Real>("parameter", 1, "Re-solution parameter");
  params.addParam<bool>("use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");

  return params;
}

KnockoutKernelsAction::KnockoutKernelsAction(const std::string & name,
                                                             InputParameters params) :
  Action(name, params),
  _var_name_base(getParam<std::string>("var_name_base")),
  _N(getParam<int>("N"))
{
  Buck::varNamesFromN( _vars, _var_name_base, _N);
  Buck::atomsFromN( _atoms, _N);

  Buck::iterateAndDisplay("vars", _vars);
  Buck::iterateAndDisplay("atoms", _atoms);
}

void
KnockoutKernelsAction::act()
{
  for (unsigned int n = 0; n < _N; ++n)
  {
    std::string var_name = _vars[n];
    std::string orig_kernel = "Knockout";

    InputParameters p = _factory.getValidParams(orig_kernel);
    p.set<NonlinearVariableName>("variable") = var_name;
    p.set<std::vector<VariableName> >("coupled_vars") = _vars;
    p.set<int>("m") = _atoms[n];
    p.set<Real>("parameter") = getParam<Real>("parameter");
    p.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");

    std::string kernel_name = var_name;
    kernel_name.append("_knockout");

    _problem->addKernel(orig_kernel, kernel_name, p);
  }
}
