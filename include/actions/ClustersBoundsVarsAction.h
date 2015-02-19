#ifndef CLUSTERSBOUNDSVARSACTION_H
#define CLUSTERSBOUNDSVARSACTION_H

#include "Action.h"

class ClustersBoundsVarsAction : public Action
{
public:
  ClustersBoundsVarsAction(const std::string & name, InputParameters params);
  virtual ~ClustersBoundsVarsAction(){}
  virtual void act();

private:
  const std::string _order;
  const std::string _family;
};

template<>
InputParameters validParams<ClustersBoundsVarsAction>();


#endif // CLUSTERSBOUNDSVARSACTION_H
