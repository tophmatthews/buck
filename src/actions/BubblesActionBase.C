#include "BubblesActionBase.h"

#include "BuckUtils.h"
#include <iomanip>

template<>
InputParameters validParams<BubblesActionBase>()
{
  InputParameters params = validParams<Action>();

  params.addParam<std::string>("conc_name_base", "c", "Specifies the base name of the variables");
  params.addParam<std::string>("conc_1stM_name_base", "m", "Specifies the base name of the variables");
  params.addParam<std::string>("rad_name_base", "r", "Specifies the base name of the variables");
  params.addRequiredParam<int>("N", "Largest group size");
  params.addRequiredParam<int>("s", "Total number of ungrouped equations");

  return params;
}

BubblesActionBase::BubblesActionBase(const std::string & name, InputParameters params) :
  Action(name, params),
  _conc_name_base(getParam<std::string>("conc_name_base")),
  _conc_1stM_name_base(getParam<std::string>("conc_1stM_name_base")),
  _rad_name_base(getParam<std::string>("rad_name_base")),
  _N(getParam<int>("N")),
  _s(getParam<int>("s"))
{
  if ( _N < _s )
    mooseError("From BubblesActionBase: N must be equal or less than s.");

  for ( int j=0; j<_s; ++j )
    _atoms.push_back(j+1);

  for ( int j=_s; j<_N; ++j )
  {
    Real x = (_atoms.back() + 1) / _s + _atoms.back();

    if ( x <= _N )
      _atoms.push_back( x );
    else
    {
      _atoms.push_back( _N );
      break;
    }
  }

  _G = _atoms.size();

  for ( unsigned int i=0; i<_atoms.size()-1; ++i)
    _widths.push_back(_atoms[i+1] - _atoms[i]);
  _widths.push_back(1.0);

  varNamesFromG( _c, _conc_name_base, _G );
  varNamesFromG( _m, _conc_1stM_name_base, _G );
  varNamesFromG( _r, _rad_name_base, _G );
}


void
BubblesActionBase::varNamesFromG(std::vector<VariableName> & vars, const std::string prefix, const int G, const int start)
{
  int digits = Buck::numDigits(G);

  for ( int i=start; i<G+1; ++i)
  {
    VariableName var_name = prefix;
    std::stringstream out;
    out << std::setw(digits) << std::setfill('0') << i;
    var_name.append(out.str());
    vars.push_back(var_name);
  }
}
