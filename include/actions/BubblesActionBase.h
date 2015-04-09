#ifndef BUBBLESACTIONBASE_H
#define BUBBLESACTIONBASE_H

#include "Action.h"

class BubblesActionBase: public Action
{
public:
  BubblesActionBase(const std::string & name, InputParameters params);

  virtual void act(){}

protected:
  const std::string _conc_name_base;
  const std::string _conc_1stM_name_base;
  const std::string _rad_name_base;
  const unsigned int _s;
  const bool _exp;

  unsigned int _N;
  std::vector<VariableName> _c;
  std::vector<VariableName> _m;
  std::vector<VariableName> _r;
  std::vector<Real> _atoms;
  std::vector<Real> _widths;
  unsigned int _G;

private:
  void varNamesFromG(std::vector<VariableName> & vars, const std::string prefix, const int G, const int start=1);
};

template<>
InputParameters validParams<BubblesActionBase>();

#endif //BUBBLESACTIONBASE_H
