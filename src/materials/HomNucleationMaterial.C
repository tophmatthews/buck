#include "HomNucleationMaterial.h"
#include "MooseEnum.h"

template<>
InputParameters validParams<HomNucleationMaterial>()
{
  InputParameters params = validParams<Material>();
  
  params.addRequiredParam<std::vector<Real> >("input_coeffs", "List of coefficients.");
  
  return params;
}

HomNucleationMaterial::HomNucleationMaterial(const std::string & name, InputParameters parameters) :
  Material(name, parameters),

  _coeffs(declareProperty<std::vector<Real> >("coeffs")),
  _input(getParam<std::vector<Real> >("input_coeffs"))
{
  _num = _input.size(); // number of inputs to cycle through
  _initialized = false; // flag to see if coefficients were resized
}

void
HomNucleationMaterial::initialize()
{
  for(unsigned int qp(0); qp < _qrule->n_points(); ++qp)
    _coeffs[qp].resize(_num);

  _initialized = true;
}

void
HomNucleationMaterial::computeProperties()
{
  if (!_initialized)
    initialize();

  for(unsigned int qp(0); qp < _qrule->n_points(); ++qp)
  {
    for ( int i=0; i<_num; ++i )
      _coeffs[qp][i] = _input[i];
  }
}
