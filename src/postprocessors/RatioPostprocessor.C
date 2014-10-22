#include "RatioPostprocessor.h"

template<>
InputParameters validParams<RatioPostprocessor>()
{
  InputParameters params = validParams<GeneralPostprocessor>();
  params.addRequiredParam<PostprocessorName>("top", "Top value");
  params.addRequiredParam<PostprocessorName>("bottom", "Bottom value");

  return params;
}

RatioPostprocessor::RatioPostprocessor(const std::string & name, InputParameters parameters) :
    GeneralPostprocessor(name, parameters),
    _top(getPostprocessorValue("top")),
    _bottom(getPostprocessorValue("bottom"))
{
}

RatioPostprocessor::~RatioPostprocessor()
{
}

void
RatioPostprocessor::initialize()
{
}

void
RatioPostprocessor::execute()
{
}

PostprocessorValue
RatioPostprocessor::getValue()
{
  return _top / _bottom;
}

void
RatioPostprocessor::threadJoin(const UserObject & /*uo*/)
{
  // nothing to do here, general PPS do not run threaded
}
