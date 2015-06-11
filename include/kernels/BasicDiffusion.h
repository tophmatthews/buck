#ifndef BASICDIFFUSION_H
#define BASICDIFFUSION_H

#include "Diffusion.h"

class BasicDiffusion : public Diffusion
{
public:
  BasicDiffusion( const std::string & name, InputParameters parameters );

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  const MaterialProperty<Real> & _diffusivity;
};

template<>
InputParameters validParams<BasicDiffusion>();

#endif // BASICDIFFUSION_H
