#ifndef BUBBLESCONCTIMEKERNERLACTION_H
#define BUBBLESCONCTIMEKERNERLACTION_H

#include "BubblesActionBase.h"

class BubblesConcTimeKernelAction: public BubblesActionBase
{
public:
  BubblesConcTimeKernelAction(const std::string & name, InputParameters params);

  virtual void act();
};

template<>
InputParameters validParams<BubblesConcTimeKernelAction>();

#endif //BUBBLESCONCTIMEKERNERLACTION_H
