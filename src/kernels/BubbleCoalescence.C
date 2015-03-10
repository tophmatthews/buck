#include "BubbleCoalescence.h"

#include "MaterialXeBubble.h"
#include "BuckUtils.h"

template<>
InputParameters validParams<BubbleCoalescence>()
{
  InputParameters params = validParams<BubbleNucleation>();

  return params;
}

BubbleCoalescence::BubbleCoalescence(const std::string & name, InputParameters parameters)
  :BubbleNucleation(name,parameters)
{
  // mooseDoOnce(Buck::iterateAndDisplay("max", _maxsize));
  // mooseDoOnce(Buck::iterateAndDisplay("min", _minsize));
  // mooseDoOnce(Buck::iterateAndDisplay("width", _width));
  // mooseDoOnce(Buck::iterateAndDisplay("avg", _avgsize));
}

Real
BubbleCoalescence::calcLosses(bool jac)
{
	for (int i=0; i<_G; ++i)
	{
		
	}
  return 0;
}


Real
BubbleCoalescence::calcGains(bool jac)
{
  return 0;
}
