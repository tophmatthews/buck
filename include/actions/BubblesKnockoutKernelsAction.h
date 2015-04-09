#ifndef BUBBLESKNOCKOUTKERNERLSACTION_H
#define BUBBLESKNOCKOUTKERNERLSACTION_H

#include "BubblesActionBase.h"

class BubblesKnockoutKernelsAction: public BubblesActionBase
{

public:
  BubblesKnockoutKernelsAction(const std::string & name, InputParameters params);
  virtual void act();

private:
	const bool _include_c1;
	const bool _include_c2;
};

template<>
InputParameters validParams<BubblesKnockoutKernelsAction>();

#endif //BUBBLESKNOCKOUTKERNERLSACTION_H
