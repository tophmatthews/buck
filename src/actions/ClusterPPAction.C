#include "ClusterPPAction.h"

#include "Factory.h"
#include "FEProblem.h"
#include "Parser.h"

// #include "Console.h"

// #include "Executioner.h"



template<>
InputParameters validParams<ClusterPPAction>()
{
  InputParameters params = validParams<Action>();

  // params.addParam<int>("N_nuc", 2, "Largest Nucleation cluster size");
  params.addParam<int>("N", 10, "Largest cluster size for growth model inclusion");
  params.addParam<std::string>("var_name_base", "c", "specifies the base name of the variables");


  params.addParam<bool>("concentrations", false, "Flag to create concentration postproceesors");
  params.addParam<bool>("total_concentrations", false, "Flag to create total concentration postproceesor");
  params.addParam<bool>("total_atoms", false, "Flag to create total atoms postproceesor");

  return params;
}

ClusterPPAction::ClusterPPAction(const std::string & name, InputParameters params) :
  Action(name, params),
  _N(getParam<int>("N")),
  _var_name_base(getParam<std::string>("var_name_base")),
  _conc_pp(getParam<bool>("concentrations")),
  _total_conc(getParam<bool>("total_concentrations")),
  _total_atoms(getParam<bool>("total_atoms"))
{
  for ( int i=1; i<_N+1; ++i )
  {
    std::ostringstream name;
    name << _var_name_base << i;
    _vars.push_back(name.str()); // TODO fix vars

    _atoms.push_back(i);
  }

  if ( (_total_atoms || _total_conc ) && !_conc_pp)
    mooseError("In ClusterPPAction: Concentrations must be true to calculate Total concentrations or atoms");
}

void
ClusterPPAction::act()
{
  if (_conc_pp)
  {
    std::vector<PostprocessorName> pp_names;
    for ( int i=0; i<_vars.size(); ++i )
    {
      std::string pp_to_use = "ElementAverageValue";
      pp_names.push_back(_vars[i] + "_conc");
      
      InputParameters params = _factory.getValidParams(pp_to_use);
      params.set<MultiMooseEnum>("execute_on") = "timestep_end";
      params.set<VariableName>("variable") = _vars[i];

      _problem->addPostprocessor(pp_to_use, pp_names[i], params);
    }

    if (_total_conc)
    {
      std::string pp_to_use = "SumOfPostprocessors";
      std::string this_pp_name = "total_conc";
      
      InputParameters params = _factory.getValidParams(pp_to_use);
      params.set<MultiMooseEnum>("execute_on") = "timestep_end";
      params.set<std::vector<PostprocessorName> >("postprocessors") = pp_names;

      _problem->addPostprocessor(pp_to_use, this_pp_name, params);
    }

    if (_total_atoms)
    {
      std::string pp_to_use = "SumOfPostprocessors";
      std::string this_pp_name = "total_atoms";
      
      InputParameters params = _factory.getValidParams(pp_to_use);
      params.set<MultiMooseEnum>("execute_on") = "timestep_end";
      params.set<std::vector<PostprocessorName> >("postprocessors") = pp_names;
      params.set<std::vector<Real> >("factors") = _atoms;

      _problem->addPostprocessor(pp_to_use, this_pp_name, params);
    }
  }
}
