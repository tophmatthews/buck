#ifndef KNOCKOUTKERNERLSACTION_H
#define KNOCKOUTKERNERLSACTION_H

#include "Action.h"

class KnockoutKernelsAction: public Action
{
public:
  KnockoutKernelsAction(const std::string & name, InputParameters params);

  virtual void act();

private:
	std::vector<VariableName> _vars;
	std::vector<int> _atoms;
  const std::string _var_name_base;
  const int _N;
};

template<>
InputParameters validParams<KnockoutKernelsAction>();

#endif //KNOCKOUTKERNERLSACTION_H
