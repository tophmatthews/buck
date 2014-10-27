#ifndef POROSITYAUXUC_H
#define POROSITYAUXUC_H

#include "AuxKernel.h"

class PorosityAuxUC : public AuxKernel
{
  public:
    PorosityAuxUC(const std::string & name, InputParameters parameters);

    virtual ~PorosityAuxUC() {}

  protected:
    virtual Real computeValue();

  private:
    const Real _initial_porosity;

    MaterialProperty<Real> & _P1_swelling;
    MaterialProperty<Real> & _P2_swelling;
    MaterialProperty<Real> & _P3_swelling;
};

template<>
InputParameters validParams<PorosityAuxUC>();

#endif // POROSITYAUXUC_H
