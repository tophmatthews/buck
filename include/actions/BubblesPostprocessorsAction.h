#ifndef BUBBLESPOSTPROCESSORSACTION_H
#define BUBBLESPOSTPROCESSORSACTION_H

#include "BubblesActionBase.h"

class BubblesPostprocessorsAction: public BubblesActionBase
{
public:
  BubblesPostprocessorsAction(const std::string & name, InputParameters params);
  virtual void act();

private:
	const bool _conc;
  const bool _total_conc;
  const bool _total_atoms;
  const bool _swelling;
  const bool _total_swelling;
  const bool _c1_loss;
};

template<>
InputParameters validParams<BubblesPostprocessorsAction>();

#endif
