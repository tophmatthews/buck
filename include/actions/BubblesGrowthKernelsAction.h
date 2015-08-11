#ifndef BUBBLESGROWTHKERNERLSACTION_H
#define BUBBLESGROWTHKERNERLSACTION_H

#include "BubblesActionBase.h"

class BubblesGrowthKernelsAction: public BubblesActionBase
{
public:
  BubblesGrowthKernelsAction(InputParameters params);
  virtual void act();

private:
	const bool _include_c1;
	const bool _include_c2;
};

template<>
InputParameters validParams<BubblesGrowthKernelsAction>();

#endif //BUBBLESGROWTHKERNERLSACTION_H
