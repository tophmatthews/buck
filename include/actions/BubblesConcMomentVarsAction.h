#ifndef BUBBLESCONCMOMENTVARSACTION_H
#define BUBBLESCONCMOMENTVARSACTION_H

#include "BubblesActionBase.h"

class BubblesConcMomentVarsAction : public BubblesActionBase
{
public:
  BubblesConcMomentVarsAction(const std::string & name, InputParameters params);
  virtual ~BubblesConcMomentVarsAction(){}
  virtual void act();

private:
  const std::string _order;
  const std::string _family;
};

template<>
InputParameters validParams<BubblesConcMomentVarsAction>();


#endif //BUBBLESCONCMOMENTVARSACTION_H
