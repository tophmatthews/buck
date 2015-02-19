#ifndef CLUSTERSTIMEKERNERLACTION_H
#define CLUSTERSTIMEKERNERLACTION_H

#include "ClustersActionBase.h"

class ClustersTimeKernelAction: public ClustersActionBase
{
public:
  ClustersTimeKernelAction(const std::string & name, InputParameters params);

  virtual void act();
};

template<>
InputParameters validParams<ClustersTimeKernelAction>();

#endif //CLUSTERSTIMEKERNERLACTION_H
