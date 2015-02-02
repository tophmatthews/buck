#include "MaterialXeBubbleTester.h"

#include "MaterialXeBubble.h"

template<>
InputParameters validParams<MaterialXeBubbleTester>()
{
  InputParameters params = validParams<GeneralPostprocessor>();

  params.addParam<Real>("temp" , 1000, "Number of atoms.");
  params.addParam<Real>("sigma", 0, "stress");
  params.addParam<bool>("testing", true, "testing output flag");

  params.addRequiredParam<PostprocessorName>("m_mag", "The postprocessor that has m order of magnitude");

  return params;
}

MaterialXeBubbleTester::MaterialXeBubbleTester(const std::string & name, InputParameters parameters) :
    GeneralPostprocessor(name, parameters),

    _temp(getParam<Real>("temp")),
    _sigma(getParam<Real>("sigma")),
    _testing(getParam<bool>("testing")),

    _m_mag(getPostprocessorValueByName(getParam<PostprocessorName>("m_mag")))
{
}

Real
MaterialXeBubbleTester::getValue()
{
	return MaterialXeBubble::VDW_MtoR(std::pow(10, _m_mag), _temp, _sigma, _testing);
}
