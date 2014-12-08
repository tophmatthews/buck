#ifndef POROSITYAUXUR_H
#define POROSITYAUXUR_H

#include "AuxKernel.h"

class PorosityAuxUr : public AuxKernel
{
  public:
    PorosityAuxUr(const std::string & name, InputParameters parameters);

    virtual ~PorosityAuxUr() {}

  protected:
    virtual Real computeValue();

  private:
    const Real _initial_porosity;

    MaterialProperty<Real> & _gas_swelling;
    MaterialProperty<Real> & _solid_swelling;
    MaterialProperty<Real> & _densification;
};

template<>
InputParameters validParams<PorosityAuxUr>();

#endif // POROSITYAUXUR_H
