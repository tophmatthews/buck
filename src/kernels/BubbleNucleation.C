#include "BubbleNucleation.h"

#include "BuckUtils.h"

template<>
InputParameters validParams<BubbleNucleation>()
{
  InputParameters params = validParams<BubbleBase>();

  params.addRequiredCoupledVar("temp", "Temperature");
  params.addParam<Real>("a", 5.0e-4, "Lattice Parameter [um]");
  params.addParam<Real>("omega", 4.09e-11, " Atomic volume [um^3]");

  return params;
}

BubbleNucleation::BubbleNucleation(const std::string & name, InputParameters parameters)
  :BubbleBase(name,parameters),
  _temp(coupledValue("temp")),
  _a(getParam<Real>("a")),
  _omega(getParam<Real>("omega")),

  _Dg(getMaterialProperty<Real>("gas_diffusivity")),

  _Z11(84.0)
{
  if ( _g > 1 )
    mooseError("In BubbleNucleation: Cannont implement on non-dimer or non-single atoms.");
}

void
BubbleNucleation::calcLosses(Real & losses, bool jac)
{
  if (_g != 0)
    return;

  Real P11 = _Z11 * _omega * _Dg[_qp] * _u[_qp] / std::pow(_a, 2.0);

  if (!jac)
    losses += 2.0 * P11 * _u[_qp];
  else
    losses += 4.0 * P11;
}


void
BubbleNucleation::calcGains(Real & gains, bool jac)
{
  if ( _g != 1 )
    return;
  if (jac)
    return;

  Real P11 = _Z11 * _omega * _Dg[_qp] * std::pow( (*_c[0])[_qp]/_a, 2.0 );

  gains += P11;
}
