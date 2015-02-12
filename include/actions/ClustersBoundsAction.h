#ifndef CLUSTERSBOUNDSACTION_H
#define CLUSTERSBOUNDSACTION_H

#include "MooseObjectAction.h"

class ClustersBoundsAction;

template<>
InputParameters validParams<ClustersBoundsAction>();

class ClustersBoundsAction : public Action
{
public:
  ClustersBoundsAction(const std::string & name, InputParameters params);

  virtual void act();

private:
	std::vector<SubdomainName> _blocks;
	const std::string _var_name_base;
  const int _N;

  std::vector<VariableName> _vars;
};

#endif // CLUSTERSBOUNDSACTION_H
