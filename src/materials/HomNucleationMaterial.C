#include "HomNucleationMaterial.h"
#include "MooseEnum.h"

// #include "MaterialXeBubble.h"

template<>
InputParameters validParams<HomNucleationMaterial>()
{
  InputParameters params = validParams<Material>();
  
  params.addCoupledVar("nucleation_conc_vars", "List of concentration variables for nucleation model");
  params.addCoupledVar("temp", 0, "Coupled Temperature");

  params.addParam<Real>("omega", 3.0e-2, "Lattice site volume [nm**3]");
  params.addParam<Real>("a", 0.5, "Lattice parameter [nm]");
  params.addParam<Real>("D0", 1.7e5, "Diffusion coefficient [nm^2/s]");
  params.addParam<Real>("Q", 2.3, "Activation energy [eV]");
  params.addParam<Real>("k", 8.617e-5, "Boltzmann's constant [eV/K]");

  params.addRequiredParam<std::vector<Real> >("diffusivity_multipliers", "Multipliers to diffusivity");

  params.addParam<std::vector<Real> >("c1_rx_coeffs", "List of c1 reaction coefficients.");
  params.addParam<std::vector<Real> >("c2_rx_coeffs", "List of c2 reaction coefficients.");
  params.addParam<std::vector<Real> >("c3_rx_coeffs", "List of c3 reaction coefficients.");
  params.addParam<std::vector<Real> >("c4_rx_coeffs", "List of c4 reaction coefficients.");
  params.addParam<std::vector<Real> >("c5_rx_coeffs", "List of c5 reaction coefficients.");
  params.addParam<std::vector<Real> >("c6_rx_coeffs", "List of c6 reaction coefficients.");
  params.addParam<std::vector<Real> >("c7_rx_coeffs", "List of c7 reaction coefficients.");
  params.addParam<std::vector<Real> >("c8_rx_coeffs", "List of c8 reaction coefficients.");
  params.addParam<std::vector<Real> >("c9_rx_coeffs", "List of c9 reaction coefficients.");

  return params;
}

HomNucleationMaterial::HomNucleationMaterial(const std::string & name, InputParameters parameters) :
  Material(name, parameters),

  _temp(coupledValue("temp")),

  _omega(getParam<Real>("omega")),
  _a(getParam<Real>("a")),
  _D0(getParam<Real>("D0")),
  _Q(getParam<Real>("Q")),
  _k(getParam<Real>("k")),

  _diffusivity_multipliers(getParam<std::vector<Real> >("diffusivity_multipliers")),

  _c1_rx_coeffs(getParam<std::vector<Real> >("c1_rx_coeffs")),
  _c2_rx_coeffs(getParam<std::vector<Real> >("c2_rx_coeffs")),
  _c3_rx_coeffs(getParam<std::vector<Real> >("c3_rx_coeffs")),
  _c4_rx_coeffs(getParam<std::vector<Real> >("c4_rx_coeffs")),
  _c5_rx_coeffs(getParam<std::vector<Real> >("c5_rx_coeffs")),
  _c6_rx_coeffs(getParam<std::vector<Real> >("c6_rx_coeffs")),
  _c7_rx_coeffs(getParam<std::vector<Real> >("c7_rx_coeffs")),
  _c8_rx_coeffs(getParam<std::vector<Real> >("c8_rx_coeffs")),
  _c9_rx_coeffs(getParam<std::vector<Real> >("c9_rx_coeffs")),

  _diffusivities(declareProperty<std::vector<Real> >("diffusivities")),
  _rx_rates(declareProperty<std::vector<std::vector<Real> > >("rx_rates")),

  _initialized(false)       // flag to see if coefficients were resized

{
  _N = coupledComponents("nucleation_conc_vars"); // number of variables that need coefficients

  // Protect against not the right number of input for a needed cn coefficient
  if ( _c1_rx_coeffs.size() != _N ||
     ( _c2_rx_coeffs.size() != _N && _N > 1 ) ||
     ( _c3_rx_coeffs.size() != _N && _N > 2 ) ||
     ( _c4_rx_coeffs.size() != _N && _N > 3 ) ||
     ( _c5_rx_coeffs.size() != _N && _N > 4 ) ||
     ( _c6_rx_coeffs.size() != _N && _N > 5 ) ||
     ( _c7_rx_coeffs.size() != _N && _N > 6 ) ||
     ( _c8_rx_coeffs.size() != _N && _N > 7 ) ||
     ( _c9_rx_coeffs.size() != _N && _N > 8 ) )
  {
    std::stringstream errorMsg;
    errorMsg << "In HomNucleationMaterial: Size of a rx_coeff is wrong.\n"
             << "\tRequired number of coefficient lists: c1 to c" << _N 
             << "\tRequired size of each coefficient list: " << _N << std::endl;
    mooseError(errorMsg.str());
  }

  // Protect against not the right number of input for a needed cn coefficient
  if ( ( _c2_rx_coeffs.size() != 0 && _N < 2 ) ||
       ( _c3_rx_coeffs.size() != 0 && _N < 3 ) ||
       ( _c4_rx_coeffs.size() != 0 && _N < 4 ) ||
       ( _c5_rx_coeffs.size() != 0 && _N < 5 ) ||
       ( _c6_rx_coeffs.size() != 0 && _N < 6 ) ||
       ( _c7_rx_coeffs.size() != 0 && _N < 7 ) ||
       ( _c8_rx_coeffs.size() != 0 && _N < 8 ) ||
       ( _c9_rx_coeffs.size() != 0 && _N < 9 ) )
  {
    std::stringstream errorMsg;
    errorMsg << "In HomNucleationMaterial: An extra rx_coeff list is present.\n"
             << "\tRequired number of coefficient lists: c1 to c" << _N << std::endl;
    mooseError(errorMsg.str());
  }

  if ( _diffusivity_multipliers.size() != _N )
  {
    std::stringstream errorMsg;
    errorMsg << "In HomNucleationMaterial: The number of diffusivity_multipliers is incorrect .\n"
             << "\tRequired number: " << _N << std::endl;
    mooseError(errorMsg.str());
  }

  _rx_coeffs.resize(_N);

  _rx_coeffs[0].insert( _rx_coeffs[0].end(), _c1_rx_coeffs.begin(), _c1_rx_coeffs.end() );
  _rx_coeffs[1].insert( _rx_coeffs[1].end(), _c2_rx_coeffs.begin(), _c2_rx_coeffs.end() );
  _rx_coeffs[2].insert( _rx_coeffs[2].end(), _c3_rx_coeffs.begin(), _c3_rx_coeffs.end() );
  _rx_coeffs[3].insert( _rx_coeffs[3].end(), _c4_rx_coeffs.begin(), _c4_rx_coeffs.end() );
  _rx_coeffs[4].insert( _rx_coeffs[4].end(), _c5_rx_coeffs.begin(), _c5_rx_coeffs.end() );
  _rx_coeffs[5].insert( _rx_coeffs[5].end(), _c6_rx_coeffs.begin(), _c6_rx_coeffs.end() );
  _rx_coeffs[6].insert( _rx_coeffs[6].end(), _c7_rx_coeffs.begin(), _c7_rx_coeffs.end() );
  _rx_coeffs[7].insert( _rx_coeffs[7].end(), _c8_rx_coeffs.begin(), _c8_rx_coeffs.end() );
  _rx_coeffs[8].insert( _rx_coeffs[8].end(), _c9_rx_coeffs.begin(), _c9_rx_coeffs.end() );
}

void
HomNucleationMaterial::initialize()
{
  for( unsigned int qp(0); qp < _qrule->n_points(); ++qp)
  {
    _diffusivities[qp].resize(_N);

    _rx_rates[qp].resize(_N);
    for ( int i=0; i<_N; ++i )
    {
      _rx_rates[qp][i].resize(_N);
    }
  }
  _initialized = true;
}

void
HomNucleationMaterial::computeProperties()
{
  if (!_initialized)
    initialize();

  for( unsigned int qp(0); qp < _qrule->n_points(); ++qp)
  {
    for ( int i=0; i<_N; ++i )
    {
      _diffusivities[qp][i] = _D0 * std::exp( -_Q / _k / _temp[qp] ) * _diffusivity_multipliers[i];
      // std::cout <<  "i: " << i <<  " diffusivities: " << _diffusivities[qp][i] << std::endl;
    }
  
    for ( int i=0; i<_N; ++i )
    {
      for (int j=0; j<_N; ++j)
      {
        // _rx_rates[qp][i][j] = 4.0e8 * std::exp( -_Q / _k / _temp[qp] ) * _rx_coeffs[i][j];
        _rx_rates[qp][i][j] = _omega / _a / _a * _diffusivities[qp][i] * _rx_coeffs[i][j];
      }
    }
  }
}
