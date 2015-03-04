#ifndef CAVITYPOINTDEFECTEMISSION_H
#define CAVITYPOINTDEFECTEMISSION_H

#include "CavityPointDefectFlux.h"

class CavityPointDefectEmission;

template<>
InputParameters validParams<CavityPointDefectEmission>();

class CavityPointDefectEmission : public CavityPointDefectFlux
{
public:
  CavityPointDefectEmission(const std::string & name, InputParameters parameters);

  virtual Real computeQpIntegral();

protected:
	VariableValue & _temp;
	VariableValue & _rad;
	const Real _N;
	const bool _is_vac;
	const Real _gamma;
	const Real _omega;
	const Real _kB;

	MaterialProperty<Real> * _X;
  Real _sign;
	

};

#endif // CAVITYPOINTDEFECTEMISSION_H
