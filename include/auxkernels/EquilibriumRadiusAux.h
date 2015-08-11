#ifndef EQUILIBRIUMRADIUSAUX_H
#define EQUILIBRIUMRADIUSAUX_H

#include "AuxKernel.h"

class EquilibriumRadiusAux : public AuxKernel
{
public:
  EquilibriumRadiusAux(const InputParameters & parameters);
  virtual ~EquilibriumRadiusAux() {}

protected:
  virtual Real computeValue();

private:
  VariableValue & _temp;
  VariableValue & _sigma;
  const Real _m;
  const Real _gamma;
  const Real _B;
};

template<>
InputParameters validParams<EquilibriumRadiusAux>();

#endif //EQUILIBRIUMRADIUSAUX_H
