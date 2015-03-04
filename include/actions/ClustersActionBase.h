#ifndef CLUSTERSACTIONBASE_H
#define CLUSTERSACTIONBASE_H

#include "Action.h"

class ClustersActionBase: public Action
{
public:
  ClustersActionBase(const std::string & name, InputParameters params);

  virtual void act(){}

protected:
  const std::string _var_name_base;
  const int _N;
  const int _M;
  
  std::vector<VariableName> _vars;
  std::vector<Real> _width;
  std::vector<Real> _maxsize;
  std::vector<Real> _minsize;
  std::vector<Real> _avgsize;

private:
  // void avgSizeFromGroup(std::vector<Real> & avgs, const Real G, const Real N_max, const int N_nuc, const bool log);
  void varNamesFromG(std::vector<VariableName> & vars, const std::string prefix, const int G, const int start=1);
};

template<>
InputParameters validParams<ClustersActionBase>();

#endif //CLUSTERSACTIONBASE_H
