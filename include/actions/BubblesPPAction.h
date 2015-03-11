#ifndef BUBBLESPPACTION_H
#define BUBBLESPPACTION_H

#include "BubblesActionBase.h"

class BubblesPPAction: public BubblesActionBase
{
public:
  BubblesPPAction(const std::string & name, InputParameters params);

  virtual void act();

private:
	const bool _conc;
  const bool _total_conc;
  const bool _total_atoms;
};

template<>
InputParameters validParams<BubblesPPAction>();

#endif //BUBBLESPPACTION_H
