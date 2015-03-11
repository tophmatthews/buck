#ifndef BUBBLESCONCMOMENTTIMEKERNERLACTION_H
#define BUBBLESCONCMOMENTTIMEKERNERLACTION_H

#include "BubblesActionBase.h"

class BubblesConcMomentTimeKernelAction: public BubblesActionBase
{
public:
  BubblesConcMomentTimeKernelAction(const std::string & name, InputParameters params);

  virtual void act();
};

template<>
InputParameters validParams<BubblesConcMomentTimeKernelAction>();

#endif //BUBBLESCONCMOMENTTIMEKERNERLACTION_H
