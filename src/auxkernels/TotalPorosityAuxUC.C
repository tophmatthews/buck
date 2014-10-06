#include "TotalPorosityAuxUC.h"
#include "Material.h"

template<>
InputParameters validParams<TotalPorosityAuxUC>()
{
  InputParameters params = validParams<AuxKernel>();
  
  return params;
}

TotalPorosityAuxUC::TotalPorosityAuxUC(const std::string & name, InputParameters parameters) : 
    AuxKernel(name, parameters),

    _P0_porosity(getMaterialProperty<Real>("P0_porosity")),
    _P1_porosity(getMaterialProperty<Real>("P1_porosity")),
    _P2_porosity(getMaterialProperty<Real>("P2_porosity")),
    _P3_porosity(getMaterialProperty<Real>("P3_porosity"))
{}

Real
TotalPorosityAuxUC::computeValue()
{
  Real total_porosity = _P0_porosity[_qp] + _P1_porosity[_qp] + _P2_porosity[_qp] + _P3_porosity[_qp];
  if ( total_porosity < 0. ) total_porosity = 0.;

  return total_porosity;
}
