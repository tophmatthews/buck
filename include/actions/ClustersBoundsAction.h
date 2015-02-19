#ifndef CLUSTERSBOUNDSACTION_H
#define CLUSTERSBOUNDSACTION_H

#include "ClustersActionBase.h"

class ClustersBoundsAction;

template<>
InputParameters validParams<ClustersBoundsAction>();

class ClustersBoundsAction : public ClustersActionBase
{
public:
  ClustersBoundsAction(const std::string & name, InputParameters params);

  virtual void act();

private:
	std::vector<SubdomainName> _blocks;
};

#endif // CLUSTERSBOUNDSACTION_H
