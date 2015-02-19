#ifndef NUCLEATIONKERNERLSACTION_H
#define NUCLEATIONKERNERLSACTION_H

#include "ClustersActionBase.h"

class NucleationKernelsAction: public ClustersActionBase
{
public:
  NucleationKernelsAction(const std::string & name, InputParameters params);

  virtual void act();
};

template<>
InputParameters validParams<NucleationKernelsAction>();

#endif //NUCLEATIONKERNERLSACTION_H
