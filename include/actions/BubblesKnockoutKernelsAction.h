#ifndef BUBBLESKNOCKOUTKERNERLSACTION_H
#define BUBBLESKNOCKOUTKERNERLSACTION_H

#include "BubblesActionBase.h"

class BubblesKnockoutKernelsAction: public BubblesActionBase
{

public:
  BubblesKnockoutKernelsAction(const std::string & name, InputParameters params);
  virtual void act();

};

template<>
InputParameters validParams<BubblesKnockoutKernelsAction>();

#endif //BUBBLESKNOCKOUTKERNERLSACTION_H
