#include "BubblesPostprocessorsAction.h"

#include "Factory.h"
#include "FEProblem.h"

template<>
InputParameters validParams<BubblesPostprocessorsAction>()
{
  InputParameters params = validParams<BubblesActionBase>();

  params.addParam<std::vector<OutputName> >("concentrations", "Where to output concentration postprocessors");
  params.addParam<std::vector<OutputName> >("total_concentrations", "Where to output concentration postprocessor. Not calculated if empty.");
  params.addParam<std::vector<OutputName> >("total_atoms", "Where to output concentration postprocessor. Not calculated if empty.");
  params.addParam<std::vector<OutputName> >("swelling", "Where to output swelling postprocessor. Not calculated if empty.");

  return params;
}

BubblesPostprocessorsAction::BubblesPostprocessorsAction(const std::string & name, InputParameters params) :
  BubblesActionBase(name, params),
  _conc( isParamValid("concentrations")? true : false),
  _total_conc( isParamValid("total_concentrations")? true : false),
  _total_atoms( isParamValid("total_atoms")? true : false),
  _swelling( isParamValid("swelling")? true : false)
{
}

void
BubblesPostprocessorsAction::act()
{
  std::vector<PostprocessorName> pp_names;

  if ( _conc || _total_conc || _total_atoms )
  {
    for ( int i=0; i<_G; ++i )
    {
      std::string pp_to_use = "BoundedElementAverage";
      pp_names.push_back(_c[i] + "_conc");

      InputParameters params = _factory.getValidParams(pp_to_use);
      params.set<MultiMooseEnum>("execute_on") = "timestep_end";
      params.set<VariableName>("variable") = _c[i];
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

  if (_swelling)
  {
    std::string pp_to_use = "SwellingPostprocessor";
    std::string this_pp_name = "gas_swelling";

    InputParameters params = _factory.getValidParams(pp_to_use);
    params.set<MultiMooseEnum>("execute_on") = "timestep_end";

    params.set<std::vector<VariableName> >("coupled_conc") = _c;
    params.set<std::vector<VariableName> >("coupled_rad") = _r;
    params.set<std::vector<Real> >("coupled_atoms") = _atoms;
    params.set<std::vector<Real> >("coupled_widths") = _widths;

    params.set<VariableName>("variable") = _c[0];

    std::vector<OutputName> outs(getParam<std::vector<OutputName> >("swelling"));
    params.set<std::vector<OutputName> >("outputs") = outs;

    _problem->addPostprocessor(pp_to_use, this_pp_name, params);
  }
}
