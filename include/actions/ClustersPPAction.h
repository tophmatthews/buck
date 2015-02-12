#ifndef CLUSTERSPPACTION_H
#define CLUSTERSPPACTION_H

#include "Action.h"
#include "MooseTypes.h"

class ClustersPPAction: public Action
{
public:
  ClustersPPAction(const std::string & name, InputParameters params);

  virtual void act();

private:
	const int _N;
	const std::string _var_name_base;

	const bool _conc;
  const bool _total_conc;
  const bool _total_atoms;

	std::vector<PostprocessorName> _vars;
	std::vector<Real> _atoms;
};

template<>
InputParameters validParams<ClustersPPAction>();

#endif //CLUSTERSPPACTION_H
