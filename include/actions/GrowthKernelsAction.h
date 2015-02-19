#ifndef GROWTHKERNERLSACTION_H
#define GROWTHKERNERLSACTION_H

#include "ClustersActionBase.h"

class GrowthKernelsAction: public ClustersActionBase
{
public:
  GrowthKernelsAction(const std::string & name, InputParameters params);

  virtual void act();
};

template<>
InputParameters validParams<GrowthKernelsAction>();

#endif //GROWTHKERNERLSACTION_H
