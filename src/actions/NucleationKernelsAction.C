#include "NucleationKernelsAction.h"
#include "Factory.h"
#include "Parser.h"
#include "FEProblem.h"

template<>
InputParameters validParams<NucleationKernelsAction>()
{
  InputParameters params = validParams<Action>();

  params.addParam<std::string>("var_name_base", "c", "specifies the base name of the variables");
  params.addParam<int>("N", 2, "Largest cluster size");
  params.addParam<bool>("use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");
  params.addParam<bool>("transient", true, "Flag to determine if TimeDerivative kernels should be made for nucleation concentration variables");

  return params;
}

NucleationKernelsAction::NucleationKernelsAction(const std::string & name,
                                                             InputParameters params) :
  Action(name, params),
  _var_name_base(getParam<std::string>("var_name_base")),
  _N(getParam<int>("N")),
  _transient(getParam<bool>("transient"))
{
  _nucleation_conc_vars.resize(_N);
  for ( int i=0; i<_N; ++i )
  {
    VariableName var_name = _var_name_base;
    std::stringstream out;
    out << i+1;
    var_name.append(out.str());
    _nucleation_conc_vars[i] = var_name;
  }

  if ( _N > 9 )
  {
    std::stringstream errorMsg;
    errorMsg << "NucleationKernelsAction: Requested cluster size is too big for the current HomNucleation model (9 atoms max)." <<std::endl;
    mooseError(errorMsg.str());
  }
}

void
NucleationKernelsAction::act()
{
  for (unsigned int n = 0; n < _N; ++n)
  {
    std::string var_name = _nucleation_conc_vars[n];

    // Create HomNucleation kernels
    InputParameters poly_params = _factory.getValidParams("HomNucleation");
    poly_params.set<NonlinearVariableName>("variable") = var_name;
    poly_params.set<std::vector<VariableName> >("nucleation_conc_vars") = _nucleation_conc_vars;
    poly_params.set<int>("m") = n+1;
    poly_params.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");

    std::string kernel_name = var_name;
    kernel_name.append("_nucleation");

    _problem->addKernel("HomNucleation", kernel_name, poly_params);

    // Create TimeDerivative kernels
    if ( _transient && n != 0)
    {
      poly_params = _factory.getValidParams("TimeDerivative");
      poly_params.set<NonlinearVariableName>("variable") = var_name;
      poly_params.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");

      kernel_name = var_name;
      kernel_name.append("_IE");

      _problem->addKernel("TimeDerivative", kernel_name, poly_params);
    }
  }
}
