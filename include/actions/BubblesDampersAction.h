#ifndef BUBBLESDAMPERSACTION_H
#define BUBBLESDAMPERSACTION_H

#include "BubblesActionBase.h"

class BubblesDampersAction;

template<>
InputParameters validParams<BubblesDampersAction>();

class BubblesDampersAction : public BubblesActionBase
{

public:
  BubblesDampersAction(const std::string & name, InputParameters params);
  virtual void act();

private:
	const int _index;
};

#endif
