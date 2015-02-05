#ifndef HOMCLUSTERDIFFUSION_H
#define HOMCLUSTERDIFFUSION_H

#include "Diffusion.h"

class HomClusterDiffusion : public Diffusion
{
public:
  HomClusterDiffusion( const std::string & name, InputParameters parameters );

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  MaterialProperty<std::vector<Real> > & _diffusivities;
  int _m;
};

template<>
InputParameters validParams<HomClusterDiffusion>();

#endif // HOMCLUSTERDIFFUSION_H
