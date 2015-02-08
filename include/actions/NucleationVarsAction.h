#ifndef NUCLEATIONVARSACTION_H
#define NUCLEATIONVARSACTION_H

#include "Action.h"

class NucleationVarsAction : public Action
{
public:
  NucleationVarsAction(const std::string & name, InputParameters params);
  virtual ~NucleationVarsAction(){}
  virtual void act();

private:
  const std::string _order;
  const std::string _family;
  const int _N;
};

template<>
InputParameters validParams<NucleationVarsAction>();


#endif //NUCLEATIONVARSACTION_H
