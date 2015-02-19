#include "ClustersPPAction.h"

#include "Factory.h"
#include "FEProblem.h"
#include "Parser.h"
#include "BuckUtils.h"

template<>
InputParameters validParams<ClustersPPAction>()
{
  InputParameters params = validParams<ClustersActionBase>();

  params.addParam<std::vector<OutputName> >("concentrations", "Where to output concentration postprocessors");
  params.addParam<std::vector<OutputName> >("total_concentrations", "Where to output concentration postprocessors. Not calculated if empty");
  params.addParam<std::vector<OutputName> >("total_atoms", "Where to output concentration postprocessors. Not calculated if empty");

  return params;
}

ClustersPPAction::ClustersPPAction(const std::string & name, InputParameters params) :
  ClustersActionBase(name, params),
  _conc( isParamValid("concentrations")? true : false),
  _total_conc( isParamValid("total_concentrations")? true : false),
  _total_atoms( isParamValid("total_atoms")? true : false)
{
}

void
ClustersPPAction::act()
{
  std::vector<PostprocessorName> pp_names;

  if ( _conc || _total_conc || _total_atoms )
  {
    for ( int i=0; i<_vars.size(); ++i )
    {
      std::string pp_to_use = "BoundedElementAverage";
      pp_names.push_back(_vars[i] + "_conc");

      InputParameters params = _factory.getValidParams(pp_to_use);
      params.set<MultiMooseEnum>("execute_on") = "timestep_end";
      params.set<VariableName>("variable") = _vars[i];
      params.set<Real>("lower") = 0;

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
