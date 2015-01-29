#include "SumPostprocessor.h"

template<>
InputParameters validParams<SumPostprocessor>()
{
  InputParameters params = validParams<DifferencePostprocessor>();
  return params;
}

SumPostprocessor::SumPostprocessor(const std::string & name, InputParameters parameters) :
    DifferencePostprocessor(name, parameters)
{
}

PostprocessorValue
SumPostprocessor::getValue()
{
  return _value1 + _value2;
}