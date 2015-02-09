#ifndef GROWTHKERNERLSACTION_H
#define GROWTHKERNERLSACTION_H

#include "Action.h"

class GrowthKernelsAction: public Action
{
public:
  GrowthKernelsAction(const std::string & name, InputParameters params);

  virtual void act();

private:
	std::vector<VariableName> _growth_conc_vars;
  const std::string _var_name_base;
  std::vector<VariableName> _temp;
  const int _N_min;
  const int _N_max;
  const bool _transient;
  const bool _N_min_transient;
};

template<>
InputParameters validParams<GrowthKernelsAction>();

#endif //GROWTHKERNERLSACTION_H
