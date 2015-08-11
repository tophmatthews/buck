#ifndef BUBBLESPOSTPROCESSORSACTION_H
#define BUBBLESPOSTPROCESSORSACTION_H

#include "BubblesActionBase.h"

class BubblesPostprocessorsAction: public BubblesActionBase
{
public:
  BubblesPostprocessorsAction(InputParameters params);
  virtual void act();

private:
	const bool _conc;
  const bool _total_conc;
  const bool _total_atoms;
  const bool _swelling;
  const bool _total_swelling;
  const bool _c1_loss;
  const bool _gain_rate;
  const bool _knockout_rate;
};

template<>
InputParameters validParams<BubblesPostprocessorsAction>();

#endif
