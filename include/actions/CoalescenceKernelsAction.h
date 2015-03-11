#ifndef COALESCENCEKERNERLSACTION_H
#define COALESCENCEKERNERLSACTION_H

#include "BubblesActionBase.h"

class CoalescenceKernelsAction: public BubblesActionBase
{
public:
  CoalescenceKernelsAction(const std::string & name, InputParameters params);

  virtual void act();
};

template<>
InputParameters validParams<CoalescenceKernelsAction>();

#endif //COALESCENCEKERNERLSACTION_H
