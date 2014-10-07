#ifndef BUBBLEVARSACTION_H
#define BUBBLEVARSACTION_H

#include "Action.h"

class BubbleVarsAction : public Action
{
public:
  BubbleVarsAction(const std::string & name, InputParameters params);
  virtual ~BubbleVarsAction(){}
  virtual void act();

private:
  const std::string _order;
  const std::string _family;
  const Real _R_min;
  const Real _R_max;
  const Real _G;
};

template<>
InputParameters validParams<BubbleVarsAction>();


#endif
