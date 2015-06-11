#ifndef GRAINBOUNDARYGASFLUX_H
#define GRAINBOUNDARYGASFLUX_H

#include "SideIntegralVariablePostprocessor.h"

class GrainBoundaryGasFlux;

template<>
InputParameters validParams<GrainBoundaryGasFlux>();

class GrainBoundaryGasFlux : public SideIntegralVariablePostprocessor
{
public:
  GrainBoundaryGasFlux(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpIntegral();

  const MaterialProperty<Real> & _diffusivity;
};

#endif // GRAINBOUNDARYGAS_H
