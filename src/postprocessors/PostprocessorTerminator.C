#include "PostprocessorTerminator.h"
// #include "PostprocessorInterface.h"

template<>
InputParameters validParams<PostprocessorTerminator>()
{
  InputParameters params = validParams<GeneralPostprocessor>();

  params.addRequiredParam<PostprocessorName>("postprocessor", "The postprocessor name");
  params.addRequiredParam<Real>("threshold", "Threshold above which to terminate the solve");

  return params;
}

PostprocessorTerminator::PostprocessorTerminator(const std::string & name, InputParameters parameters) :
    GeneralPostprocessor(name, parameters),
    _threshold(getParam<Real>("threshold")),
    _value(getPostprocessorValue("postprocessor"))
{
}

void
PostprocessorTerminator::execute()
{
  if (_value > _threshold)
    _fe_problem.terminateSolve();
}