#include "SwellingPostprocessor.h"

template<>
InputParameters validParams<SwellingPostprocessor>()
{
  InputParameters params = validParams<ElementIntegralVariablePostprocessor>();

  params.addRequiredCoupledVar("coupled_conc", "List of coupled concentration variables.");
  params.addRequiredCoupledVar("coupled_rad", "List of coupled radius variables.");

  return params;
}

SwellingPostprocessor::SwellingPostprocessor(const std::string & name, InputParameters parameters) :
  ElementIntegralVariablePostprocessor(name, parameters)
{
	_G = coupledComponents("coupled_conc");

	if ( _G != coupledComponents("coupled_rad") )
    mooseError("From BubbleBase: The number of coupled concentrations does not match coupled radii.");

	for ( unsigned int i=0; i<_G; ++i )
  {
    _c.push_back( &coupledValue("coupled_conc", i) );
    _r.push_back( &coupledValue("coupled_rad", i) );
  }
}

Real
SwellingPostprocessor::computeQpIntegral()
{
	Real swell(0);

	for ( unsigned int i=0; i<_G; ++i)
		swell += (*_c[i])[_qp] * 4.0/3.0 * M_PI * std::pow( (*_r[i])[_qp], 3.0 );

  return swell;
}
