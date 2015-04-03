#include "SumOfPostprocessors.h"
#include "PostprocessorInterface.h"

template<>
InputParameters validParams<SumOfPostprocessors>()
{
  InputParameters params = validParams<GeneralPostprocessor>();

  params.addRequiredParam<std::vector<PostprocessorName> >("postprocessors", "The postprocessors whose values are to be summed");
  params.addParam<std::vector<Real> >("factors", "Factors that postprocessors are multiplied against.");

  return params;
}

SumOfPostprocessors::SumOfPostprocessors(const std::string & name, InputParameters parameters) :
    GeneralPostprocessor(name, parameters),
    _factors(getParam<std::vector<Real> >("factors"))
{
  std::vector<PostprocessorName> pps_names(getParam<std::vector<PostprocessorName> >("postprocessors"));

  // Create vector of pps
  unsigned int _N = pps_names.size();
  for ( unsigned int i=0; i<_N; ++i )
  {
    if (!hasPostprocessorByName(pps_names[i]))
      mooseError("In SumOfPostprocessors, postprocessor with name: "<<pps_names[i]<<" does not exist");
    _postprocessor_values.push_back(&getPostprocessorValueByName(pps_names[i]));
  }

  // Create default factors if it doesn't exist
  if ( _factors.size() == 0 )
  {
    for ( unsigned int i=0; i<_N; ++i )
      _factors.push_back(1);
  }
  else if (_factors.size() != _N)
  {
    mooseError("In SumOfPostprocessors: Size of factors does not match number of postprocessors.");
  }
}

Real
SumOfPostprocessors::getValue()
{
  Real val(0.0);
  for ( unsigned int i=0; i<_postprocessor_values.size(); ++i)
  {
    val += *_postprocessor_values[i] * _factors[i];
  }
  return val;
}
