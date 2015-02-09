#ifndef GROWTHVARSACTION_H
#define GROWTHVARSACTION_H

#include "Action.h"

class GrowthVarsAction : public Action
{
public:
  GrowthVarsAction(const std::string & name, InputParameters params);
  virtual ~GrowthVarsAction(){}
  virtual void act();

private:
  const std::string _order;
  const std::string _family;
  const std::string _var_name_base;
  const int _N_min;
  const int _N_max;
};

template<>
InputParameters validParams<GrowthVarsAction>();


#endif //GROWTHVARSACTION_H
