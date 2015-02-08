#ifndef NUCLEATIONKERNERLSACTION_H
#define NUCLEATIONKERNERLSACTION_H

#include "Action.h"

class NucleationKernelsAction: public Action
{
public:
  NucleationKernelsAction(const std::string & name, InputParameters params);

  virtual void act();

private:
	std::vector<VariableName> _nucleation_conc_vars;
  const std::string _var_name_base;
  const int _N;
  const bool _transient;
};

template<>
InputParameters validParams<NucleationKernelsAction>();

#endif //NUCLEATIONKERNERLSACTION_H
