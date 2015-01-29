#include "SumOfPostprocessors.h"
#include "PostprocessorInterface.h"

template<>
InputParameters validParams<SumOfPostprocessors>()
{
  InputParameters params = validParams<GeneralPostprocessor>();

  params.addRequiredParam<std::vector<PostprocessorName> >("postprocessors", "The postprocessors whose values are to be summed");

  return params;
}

SumOfPostprocessors::SumOfPostprocessors(const std::string & name, InputParameters parameters) :
    GeneralPostprocessor(name, parameters)
{
  std::vector<PostprocessorName> pps_names(getParam<std::vector<PostprocessorName> >("postprocessors"));

  for (unsigned int i=0; i<pps_names.size(); ++i)
  {
    if (!hasPostprocessorByName(pps_names[i]))
      mooseError("In SumOfPostprocessors, postprocessor with name: "<<pps_names[i]<<" does not exist");
    _postprocessor_values.push_back(&getPostprocessorValueByName(pps_names[i]));
  }
}

Real
SumOfPostprocessors::getValue()
{
  Real val(0.0);
  for (unsigned int i=0; i<_postprocessor_values.size(); ++i)
  {
    val += *_postprocessor_values[i];
  }
  return val;
}