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
  params.addRequiredParam<int>("G", "Total number of groups");
  params.addRequiredParam<int>("s", "Total number of ungrouped equations");
  params.addRequiredParam<Real>("M", "Scaling parameter");

  return params;
}

BubblesActionBase::BubblesActionBase(const std::string & name, InputParameters params) :
  Action(name, params),
  _conc_name_base(getParam<std::string>("conc_name_base")),
  _conc_1stM_name_base(getParam<std::string>("conc_1stM_name_base")),
  _rad_name_base(getParam<std::string>("rad_name_base")),
  _G(getParam<int>("G")),
  _s(getParam<int>("s")),
  _M(getParam<Real>("M"))
{
  if ( _G < _s )
    mooseError("From BubblesActionBase: Total number of equations must be equal or less than number of ungrouped equations.");

  varNamesFromG( _c, _conc_name_base, _G );
  varNamesFromG( _m, _conc_1stM_name_base, _G );
  varNamesFromG( _r, _rad_name_base, _G );

  for ( int j=0; j<_s; ++j )
    _atoms.push_back(j+1);

  for ( int j=_s; j<_G; ++j )
    _atoms.push_back( _atoms.back() * _M );
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
