#ifndef BUBBLESCONCVARSACTION_H
#define BUBBLESCONCVARSACTION_H

#include "BubblesActionBase.h"

class BubblesConcVarsAction : public BubblesActionBase
{
public:
  BubblesConcVarsAction(InputParameters params);
  virtual ~BubblesConcVarsAction(){}
  virtual void act();

private:
  const std::string _order;
  const std::string _family;

  const Real _ic;
};

template<>
InputParameters validParams<BubblesConcVarsAction>();

#endif //BUBBLESCONCVARSACTION_H
