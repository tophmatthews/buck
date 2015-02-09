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

  params.addParam<int>("N", 10, "Largest cluster size for growth model inclusion");
  params.addParam<std::string>("var_name_base", "c", "specifies the base name of the variables");


  params.addParam<std::vector<OutputName> >("concentrations", "Where to output concentration postprocessors");
  params.addParam<std::vector<OutputName> >("total_concentrations", "Where to output concentration postprocessors. Not calculated if empty");
  params.addParam<std::vector<OutputName> >("total_atoms", "Where to output concentration postprocessors. Not calculated if empty");

  return params;
}

ClusterPPAction::ClusterPPAction(const std::string & name, InputParameters params) :
  Action(name, params),
  _N(getParam<int>("N")),
  _var_name_base(getParam<std::string>("var_name_base")),
  _conc( isParamValid("concentrations")? true : false),
  _total_conc( isParamValid("total_concentrations")? true : false),
  _total_atoms( isParamValid("total_atoms")? true : false)
 
{
  for ( int i=1; i<_N+1; ++i )
  {
    std::ostringstream name;
    name << _var_name_base << i;
    _vars.push_back(name.str()); // TODO fix vars

    _atoms.push_back(i);
  }
}

void
ClusterPPAction::act()
{
  std::vector<PostprocessorName> pp_names;

  if ( _conc || _total_conc || _total_atoms )
  { 
    for ( int i=0; i<_vars.size(); ++i )
    {
      std::string pp_to_use = "ElementAverageValue";
      pp_names.push_back(_vars[i] + "_conc");
      
      InputParameters params = _factory.getValidParams(pp_to_use);
      params.set<MultiMooseEnum>("execute_on") = "timestep_end";
      params.set<VariableName>("variable") = _vars[i];

      std::vector<OutputName> outs;
      if ( _conc )
        outs = getParam<std::vector<OutputName> >("concentrations");
      else
        outs.push_back("none");

      params.set<std::vector<OutputName> >("outputs") = outs;

      _problem->addPostprocessor(pp_to_use, pp_names[i], params);
    }
  }

  if (_total_conc)
  {
    std::string pp_to_use = "SumOfPostprocessors";
    std::string this_pp_name = "total_conc";
    
    InputParameters params = _factory.getValidParams(pp_to_use);
    params.set<MultiMooseEnum>("execute_on") = "timestep_end";
    params.set<std::vector<PostprocessorName> >("postprocessors") = pp_names;

    std::vector<OutputName> outs(getParam<std::vector<OutputName> >("total_concentrations"));
    params.set<std::vector<OutputName> >("outputs") = outs;

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

    std::vector<OutputName> outs(getParam<std::vector<OutputName> >("total_atoms"));
    params.set<std::vector<OutputName> >("outputs") = outs;

    _problem->addPostprocessor(pp_to_use, this_pp_name, params);
  }
}
