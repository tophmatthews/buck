#ifndef SPECIESDIFFUSION_H
#define SPECIESDIFFUSION_H

#include "Diffusion.h"
#include "Material.h"

class SpeciesDiffusion : public Diffusion
{
public:
  SpeciesDiffusion( const std::string & name, InputParameters parameters );

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  MaterialProperty<std::vector<Real> > & _diffusivities;

  int _m;
};

template<>
InputParameters validParams<SpeciesDiffusion>();

#endif // SPECIESDIFFUSION_H
