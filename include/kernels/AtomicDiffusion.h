#ifndef ATOMICDIFFUSION_H
#define ATOMICDIFFUSION_H

#include "Diffusion.h"

class AtomicDiffusion : public Diffusion
{
public:
  AtomicDiffusion( const std::string & name, InputParameters parameters );

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  MaterialProperty<Real> & _atomic_diffusivity;
};

template<>
InputParameters validParams<AtomicDiffusion>();

#endif // ATOMICDIFFUSION_H
