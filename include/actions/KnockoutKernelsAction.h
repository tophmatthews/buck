#ifndef KNOCKOUTKERNERLSACTION_H
#define KNOCKOUTKERNERLSACTION_H

#include "ClustersActionBase.h"

class KnockoutKernelsAction: public ClustersActionBase
{
public:
  KnockoutKernelsAction(const std::string & name, InputParameters params);

  virtual void act();
};

template<>
InputParameters validParams<KnockoutKernelsAction>();

#endif //KNOCKOUTKERNERLSACTION_H
