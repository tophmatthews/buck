#include "HomPointLoss.h"

#include "MooseException.h"

template<>
InputParameters validParams<HomPointLoss>()
{
  InputParameters params = validParams<Kernel>();

  params.addCoupledVar("vars", "List of bubbles variables.");

  return params;
}

HomPointLoss::HomPointLoss(const std::string & name, InputParameters parameters)
  :Kernel(name,parameters),
   _coeffs(getMaterialProperty<std::vector<Real> >("coeffs"))
{
  _n = coupledComponents("vars");
  _vars.resize(_n);
  _vals.resize(_n);

  for (unsigned int i=0; i<_vals.size(); ++i)
  {
    _vars[i] = coupled("vars", i);
    _vals[i] = &coupledValue("vars", i);
  }

  // Coefficient size should be self interaction coeff, then the remaining size coeffs
  // if ( 1+_n != _coeffs.size() )
  // {
  //   std::stringstream errorMsg;
  //   errorMsg << "In HomPointLoss: Number of variables does not match the (number of coefficients  - 1).\n"
  //            << "\tvars size: " << _vars.size() << "\tcoeffs size: " << _coeffs.size();
  //   mooseError(errorMsg.str());
  // }
}

Real HomPointLoss::computeQpResidual()
{
  Real diff(0);

  // Self combination loss. 2 is to signify two points are lost.
  diff += 2 * _coeffs[_qp][0] * _u[_qp];

  // Other combination losses
  for ( int i=0; i<_n; ++i )
    diff += _coeffs[_qp][i+1] * (*_vals[i])[_qp];

  return diff * _u[_qp] * _test[_i][_qp];
}

Real HomPointLoss::computeQpJacobian()
{
  Real diff(0);

  // Self combination loss. 2 is to signify two lost multiplied by derivative of u**2
  diff += 4 * _coeffs[_qp][0] * _u[_qp];

  // Other combination losses
  for ( int i=0; i<_n; ++i )
    diff += _coeffs[_qp][i+1] * (*_vals[i])[_qp];

  return diff * _phi[_j][_qp] * _test[_i][_qp];
}
