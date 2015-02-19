#ifndef CLUSTERSVARSACTION_H
#define CLUSTERSVARSACTION_H

#include "ClustersActionBase.h"

class ClustersVarsAction : public ClustersActionBase
{
public:
  ClustersVarsAction(const std::string & name, InputParameters params);
  virtual ~ClustersVarsAction(){}
  virtual void act();

private:
  const std::string _order;
  const std::string _family;
};

template<>
InputParameters validParams<ClustersVarsAction>();


#endif //CLUSTERSVARSACTION_H
