#ifndef CAVITYPOINTDEFECTFLUX_H
#define CAVITYPOINTDEFECTFLUX_H

#include "ElementAverageValue.h"

class CavityPointDefectFlux;

template<>
InputParameters validParams<CavityPointDefectFlux>();

class CavityPointDefectFlux : public ElementAverageValue
{
public:
  CavityPointDefectFlux(const std::string & name, InputParameters parameters);

  virtual Real computeQpIntegral();
  virtual Real getValue();

protected:
  MaterialProperty<Real> & _diffusivity;

};

#endif // CAVITYPOINTDEFECTFLUX_H
