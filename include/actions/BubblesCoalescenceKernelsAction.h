#ifndef BUBBLESCOALESCENCEKERNERLSACTION_H
#define BUBBLESCOALESCENCEKERNERLSACTION_H

#include "BubblesActionBase.h"

class BubblesCoalescenceKernelsAction: public BubblesActionBase
{
public:
  BubblesCoalescenceKernelsAction(const std::string & name, InputParameters params);

  virtual void act();
};

template<>
InputParameters validParams<BubblesCoalescenceKernelsAction>();

#endif //BUBBLESCOALESCENCEKERNERLSACTION_H
