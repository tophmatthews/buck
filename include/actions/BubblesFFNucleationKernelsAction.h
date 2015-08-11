#ifndef BUBBLESFFNUCLEATIONKERNELSACTION_H
#define BUBBLESFFNUCLEATIONKERNELSACTION_H

#include "BubblesActionBase.h"

class BubblesFFNucleationKernelsAction: public BubblesActionBase
{
public:
  BubblesFFNucleationKernelsAction(InputParameters params);
  virtual void act();
};

template<>
InputParameters validParams<BubblesFFNucleationKernelsAction>();

#endif //BubblesFFNucleationKernelsAction_H
