#include "GrowthKernelsAction.h"

#include "Factory.h"
#include "Parser.h"
#include "FEProblem.h"
#include "BuckUtils.h"

template<>
InputParameters validParams<GrowthKernelsAction>()
{
  InputParameters params = validParams<Action>();

  params.addParam<std::string>("var_name_base", "c", "Specifies the base name of the variables");
  params.addRequiredParam<NonlinearVariableName>("temp", "The temperature variable name");
  params.addRequiredParam<int>("N_nuc", "Largest cluster size from nucleation model");
  params.addRequiredParam<int>("N", "Largest cluster size for growth model inclusion");
  params.addParam<bool>("use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");
  params.addParam<bool>("transient", true, "Flag to determine if TimeDerivative kernels should be made for growth concentration variables");
  params.addParam<bool>("N_min_transient", false, "Flag to determine if TimeDerivative kernels should be made for N_min variable. A timeDerivative kernels is usually created in the Nucleation model.");

  return params;
}

GrowthKernelsAction::GrowthKernelsAction(const std::string & name,
                                                             InputParameters params) :
  Action(name, params),
  _var_name_base(getParam<std::string>("var_name_base")),
  _N_min(getParam<int>("N_nuc")),
  _N_max(getParam<int>("N")),
  _transient(getParam<bool>("transient")),
  _N_min_transient(getParam<bool>("N_min_transient"))
{
  if ( _N_min > _N_max )
  {
    std::stringstream errorMsg;
    errorMsg << "GrowthKernelsAction: N must be greater than N_min." <<std::endl;
    mooseError(errorMsg.str());
  }

  // Create variable list
  _growth_conc_vars.resize(_N_max - _N_min + 2);
  for ( int i=0; i<_growth_conc_vars.size(); ++i )
  {
    VariableName var_name = _var_name_base;
    std::stringstream out;
    // Need to ensure to make SinkGrowth kernel for c1
    if ( i == 0 )
      out << 1;
    else
      out << _N_min + i - 1;
    var_name.append(out.str());
    _growth_conc_vars[i] = var_name;
  }

  _atoms.push_back(1);
  Buck::atomsFromN(_atoms, _N_max, _N_min);

  for ( int i=0; i<_growth_conc_vars.size(); ++i )
    std::cout << _growth_conc_vars[i] << std::endl;
}


void
GrowthKernelsAction::act()
{
  for (unsigned int n=0; n < _growth_conc_vars.size(); ++n)
  {
    std::string var_name = _growth_conc_vars[n];

    // Create HomNucleation kernels
    InputParameters poly_params = _factory.getValidParams("SinkGrowth");
    poly_params.set<NonlinearVariableName>("variable") = var_name;
    poly_params.set<std::vector<VariableName> >("growth_conc_vars") = _growth_conc_vars;

    poly_params.addCoupledVar("temp", "");
    poly_params.set<std::vector<VariableName> >("temp") = std::vector<VariableName>(1, getParam<NonlinearVariableName>("temp"));

    poly_params.set<int>("m") = _atoms[n];
    poly_params.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");

    std::string kernel_name = var_name;
    kernel_name.append("_growth");

    _problem->addKernel("SinkGrowth", kernel_name, poly_params);

    // Create TimeDerivative kernels
    if ( _transient && n > 1)
    {
      poly_params = _factory.getValidParams("TimeDerivative");
      poly_params.set<NonlinearVariableName>("variable") = var_name;
      poly_params.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");

      kernel_name = var_name;
      kernel_name.append("_IE");

      _problem->addKernel("TimeDerivative", kernel_name, poly_params);
    }

    if ( _N_min_transient && n == 1)
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
