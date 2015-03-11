#ifndef BUBBLESRADVARSACTION_H
#define BUBBLESRADVARSACTION_H

#include "BubblesActionBase.h"

class BubblesRadVarsAction : public BubblesActionBase
{
public:
  BubblesRadVarsAction(const std::string & name, InputParameters params);
  virtual ~BubblesRadVarsAction(){}
  virtual void act();

private:
  const std::string _order;
  const std::string _family;
};

template<>
InputParameters validParams<BubblesRadVarsAction>();


#endif //BUBBLESRADVARSACTION_H
