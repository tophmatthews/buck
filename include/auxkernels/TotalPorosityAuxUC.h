#ifndef TOTALPOROSITYAUXUC_H
#define TOTALPOROSITYAUXUC_H

#include "AuxKernel.h"

class TotalPorosityAuxUC : public AuxKernel
{
  public:
    TotalPorosityAuxUC(const std::string & name, InputParameters parameters);

    virtual ~TotalPorosityAuxUC() {}

  protected:
    virtual Real computeValue();

  private:
    MaterialProperty<Real> & _P0_porosity;
    MaterialProperty<Real> & _P1_porosity;
    MaterialProperty<Real> & _P2_porosity;
    MaterialProperty<Real> & _P3_porosity;
};

template<>
InputParameters validParams<TotalPorosityAuxUC>();

#endif // TOTALPOROSITYAUXUC_H
