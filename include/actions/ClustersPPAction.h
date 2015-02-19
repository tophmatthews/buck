#ifndef CLUSTERSPPACTION_H
#define CLUSTERSPPACTION_H

#include "ClustersActionBase.h"
// #include "MooseTypes.h"

class ClustersPPAction: public ClustersActionBase
{
public:
  ClustersPPAction(const std::string & name, InputParameters params);

  virtual void act();

private:
	const bool _conc;
  const bool _total_conc;
  const bool _total_atoms;
};

template<>
InputParameters validParams<ClustersPPAction>();

#endif //CLUSTERSPPACTION_H
