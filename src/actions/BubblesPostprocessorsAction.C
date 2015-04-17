#include "BubblesPostprocessorsAction.h"

#include "Factory.h"
#include "FEProblem.h"

template<>
InputParameters validParams<BubblesPostprocessorsAction>()
{
  InputParameters params = validParams<BubblesActionBase>();

  params.addRequiredParam<VariableName>("fission_rate", "fission rate density");

  params.addParam<std::vector<OutputName> >("concentrations", "Where to output concentration postprocessors");
  params.addParam<std::vector<OutputName> >("total_concentrations", "Where to output concentration postprocessor. Not calculated if empty.");
  params.addParam<std::vector<OutputName> >("total_atoms", "Where to output concentration postprocessor. Not calculated if empty.");
  params.addParam<std::vector<OutputName> >("swelling", "Where to output swelling postprocessor. Not calculated if empty.");
  params.addParam<std::vector<OutputName> >("total_swelling", "Where to output total swelling postprocessor. Not calculated if empty.");
  params.addParam<std::vector<OutputName> >("c1_loss", "Where to output c1 loss postprocessor. Not calculated if empty.");

  return params;
}

BubblesPostprocessorsAction::BubblesPostprocessorsAction(const std::string & name, InputParameters params) :
  BubblesActionBase(name, params),
  _conc( isParamValid("concentrations")? true : false),
  _total_conc( isParamValid("total_concentrations")? true : false),
  _total_atoms( isParamValid("total_atoms")? true : false),
  _swelling( isParamValid("swelling")? true : false),
  _total_swelling( isParamValid("total_swelling")? true : false),
  _c1_loss ( isParamValid("c1_loss")? true : false)
{
}

void
BubblesPostprocessorsAction::act()
{
  std::vector<PostprocessorName> pp_names;
  std::vector<PostprocessorName> swelling_pp_names;
  std::vector<PostprocessorName> c1_pp_names;

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

  if (_swelling || _total_swelling)
  {
    for ( int i=0; i<_G; ++i )
    {
      std::string pp_to_use = "SwellingPostprocessor";
      swelling_pp_names.push_back(_c[i] + "_swelling");

      InputParameters params = _factory.getValidParams(pp_to_use);
      params.set<MultiMooseEnum>("execute_on") = "timestep_end";
      params.set<VariableName>("variable") = _c[i];

      params.addCoupledVar("r", "");
      params.set<std::vector<VariableName> >("r") = std::vector<VariableName>(1, _r[i]);

      params.set<Real>("width") = _widths[i];

      std::vector<OutputName> outs;
      if ( _swelling )
        outs = getParam<std::vector<OutputName> >("swelling");
      else
        outs.push_back("none");

      params.set<std::vector<OutputName> >("outputs") = outs;

      _problem->addPostprocessor(pp_to_use, swelling_pp_names[i], params);
    }
  }

  if (_total_swelling)
  {
    std::string pp_to_use = "SumOfPostprocessors";
    std::string this_pp_name = "gas_swelling";

    InputParameters params = _factory.getValidParams(pp_to_use);
    params.set<MultiMooseEnum>("execute_on") = "timestep_end";
    params.set<std::vector<PostprocessorName> >("postprocessors") = swelling_pp_names;

    std::vector<OutputName> outs(getParam<std::vector<OutputName> >("total_swelling"));
    params.set<std::vector<OutputName> >("outputs") = outs;

    _problem->addPostprocessor(pp_to_use, this_pp_name, params);
  }

  if (_c1_loss)
  {
    for ( int i=0; i<_G; ++i )
    {
      std::string pp_to_use = "C1LossPostprocessor";
      c1_pp_names.push_back(_c[i] + "_c1_loss");

      InputParameters params = _factory.getValidParams(pp_to_use);
      params.set<MultiMooseEnum>("execute_on") = "timestep_end";
      params.set<VariableName>("variable") = _c[i];

      params.addCoupledVar("r", "");
      params.set<std::vector<VariableName> >("r") = std::vector<VariableName>(1, _r[i]);

      params.addCoupledVar("c1", "");
      params.set<std::vector<VariableName> >("c1") = std::vector<VariableName>(1, _c[0]);

      params.addCoupledVar("fission_rate", "");
      params.set<std::vector<VariableName> >("fission_rate") = std::vector<VariableName>(1, getParam<VariableName>("fission_rate"));

      params.set<Real>("width") = _widths[i];
      params.set<Real>("atoms") = _atoms[i];

      params.set<std::vector<OutputName> >("outputs") = getParam<std::vector<OutputName> >("c1_loss");

      _problem->addPostprocessor(pp_to_use, c1_pp_names[i], params);
    }
  }

}
