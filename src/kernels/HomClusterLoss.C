#include "HomClusterLoss.h"

#include "MooseException.h"

template<>
InputParameters validParams<HomClusterLoss>()
{
  InputParameters params = validParams<Kernel>();

  params.addRequiredParam<int>("var_size", "Number of atoms in the current variable");

  params.addCoupledVar("c1", "Single point concentration.");

  return params;
}

HomClusterLoss::HomClusterLoss(const std::string & name, InputParameters parameters)
  :Kernel(name,parameters),
   _var_size(getParam<int>("var_size")),
   _c1(coupledValue("c1")),
   _coeffs(getMaterialProperty<std::vector<Real> >("coeffs"))
{
  if ( _var_size > _coeffs.size() )
  {
    std::stringstream errorMsg;
    errorMsg << "In HomClusterCombination: Size of coefficients is wrong.\n"
             << "\tvar cluster size: " << _var_size << "\tcoeffs size: " << _coeffs.size();
    mooseError(errorMsg.str());
  }
}

Real HomClusterLoss::computeQpResidual()
{
  Real diff;
  diff = _coeffs[_qp][_var_size-1] * _c1[_qp] * _u[_qp] * _test[_i][_qp];

  return diff;
}

Real HomClusterLoss::computeQpJacobian()
{
  Real diff;
  diff = _coeffs[_qp][_var_size-1] * _c1[_qp] * _phi[_j][_qp] * _test[_i][_qp];

  return diff;
}
