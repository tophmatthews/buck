#ifndef CLUSTERSACTIONBASE_H
#define CLUSTERSACTIONBASE_H

#include "Action.h"

class ClustersActionBase: public Action
{
public:
  ClustersActionBase(const std::string & name, InputParameters params);

  virtual void act(){}
  virtual void setup();

protected:
	std::vector<VariableName> _vars;
	std::vector<Real> _atoms;
  const std::string _var_name_base;
  const int _G;
  const int _N_nuc;
  const Real _N_max;
  const bool _log;

private:
  void avgSizeFromGroup(std::vector<Real> & avgs, const Real G, const Real N_max, const int N_nuc, const bool log);
};

template<>
InputParameters validParams<ClustersActionBase>();

#endif //CLUSTERSACTIONBASE_H
