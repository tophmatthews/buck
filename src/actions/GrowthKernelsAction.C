#include "GrowthKernelsAction.h"
#include "Factory.h"
#include "Parser.h"
#include "FEProblem.h"

template<>
InputParameters validParams<GrowthKernelsAction>()
{
  InputParameters params = validParams<Action>();

  params.addParam<std::string>("var_name_base", "c", "Specifies the base name of the variables");
  params.addParam<std::string>("temp_name", "temp", "Coupled Temperature variable name");
  params.addParam<int>("N_min", 4, "Smallest cluster size for growth model inclusion");
  params.addParam<int>("N_max", 10, "Largest cluster size for growth model inclusion");
  params.addParam<bool>("use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");
  params.addParam<bool>("transient", true, "Flag to determine if TimeDerivative kernels should be made for growth concentration variables");
  params.addParam<bool>("N_min_transient", false, "Flag to determine if TimeDerivative kernels should be made for N_min variable. A timeDerivative kernels is usually created in the Nucleation model.");

  return params;
}

GrowthKernelsAction::GrowthKernelsAction(const std::string & name,
                                                             InputParameters params) :
  Action(name, params),
  _var_name_base(getParam<std::string>("var_name_base")),
  _N_min(getParam<int>("N_min")),
  _N_max(getParam<int>("N_max")),
  _transient(getParam<bool>("transient")),
  _N_min_transient(getParam<bool>("N_min_transient"))
{
  _temp.push_back(getParam<std::string>("temp_name"));

  // Create variable list
  _growth_conc_vars.resize(_N_max - _N_min + 2);
  for ( int i=0; i<_growth_conc_vars.size(); ++i )
  {
    VariableName var_name = _var_name_base;
    std::stringstream out;
    if ( i == 0 )
      out << 1;
    else
      out << _N_min + i-1;
    var_name.append(out.str());
    _growth_conc_vars[i] = var_name;
  }

  if ( _N_min > _N_max )
  {
    std::stringstream errorMsg;
    errorMsg << "GrowthKernelsAction: N_max must be greater than N_min." <<std::endl;
    mooseError(errorMsg.str());
  }
}


void
GrowthKernelsAction::act()
{
  for (unsigned int n=0; n < _growth_conc_vars.size(); ++n)
  {
    int atoms;
    if ( n == 0 )
      atoms = 1;
    else
      atoms = _N_min + n-1;

    std::string var_name = _growth_conc_vars[n];

    // Create HomNucleation kernels
    InputParameters poly_params = _factory.getValidParams("SinkGrowth");
    poly_params.set<NonlinearVariableName>("variable") = var_name;
    poly_params.set<std::vector<VariableName> >("growth_conc_vars") = _growth_conc_vars;
    poly_params.set<std::vector<VariableName> >("temp") = _temp;
    poly_params.set<int>("m") = atoms;
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
