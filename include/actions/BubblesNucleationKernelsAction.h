#ifndef BUBBLESNUCLEATIONKERNELSACTION_H
#define BUBBLESNUCLEATIONKERNELSACTION_H

#include "BubblesActionBase.h"

class BubblesNucleationKernelsAction: public BubblesActionBase
{
public:
  BubblesNucleationKernelsAction(const std::string & name, InputParameters params);

  virtual void act();
};

template<>
InputParameters validParams<BubblesNucleationKernelsAction>();

#endif //BUBBLESNUCLEATIONKERNELSACTION_H
