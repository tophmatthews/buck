#ifndef BUBBLESRADAUXVARSACTION_H
#define BUBBLESRADAUXVARSACTION_H

#include "BubblesActionBase.h"

class BubblesRadAuxVarsAction : public BubblesActionBase
{
public:
  BubblesRadAuxVarsAction(const std::string & name, InputParameters params);
  virtual ~BubblesRadAuxVarsAction(){}
  virtual void act();

private:
  const std::string _order;
  const std::string _family;
};

template<>
InputParameters validParams<BubblesRadAuxVarsAction>();


#endif //BUBBLESRADAUXVARSACTION_H
