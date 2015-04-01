#ifndef BUBBLESGROWTHKERNERLSACTION_H
#define BUBBLESGROWTHKERNERLSACTION_H

#include "BubblesActionBase.h"

class BubblesGrowthKernelsAction: public BubblesActionBase
{
	
public:
  BubblesGrowthKernelsAction(const std::string & name, InputParameters params);
  virtual void act();

};

template<>
InputParameters validParams<BubblesGrowthKernelsAction>();

#endif //BUBBLESGROWTHKERNERLSACTION_H
