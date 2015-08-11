#ifndef BUBBLESDAMPERSACTION_H
#define BUBBLESDAMPERSACTION_H

#include "BubblesActionBase.h"

class BubblesDampersAction : public BubblesActionBase
{
public:
  BubblesDampersAction(InputParameters params);
  virtual void act();

private:
	const int _index;
};

template<>
InputParameters validParams<BubblesDampersAction>();

#endif
