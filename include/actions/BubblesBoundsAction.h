#ifndef BUBBLESBOUNDSACTION_H
#define BUBBLESBOUNDSACTION_H

#include "BubblesActionBase.h"

class BubblesBoundsAction;

template<>
InputParameters validParams<BubblesBoundsAction>();

class BubblesBoundsAction : public BubblesActionBase
{
public:
  BubblesBoundsAction(const std::string & name, InputParameters params);

  virtual void act();

private:
	std::vector<SubdomainName> _blocks;
};

#endif // BUBBLESBOUNDSACTION_H
