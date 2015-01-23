#include "HomNucleationMaterial.h"
#include "MooseEnum.h"

template<>
InputParameters validParams<HomNucleationMaterial>()
{
  InputParameters params = validParams<Material>();
  
  params.addCoupledVar("nucleation_conc_vars", "List of concentration variables for nucleation model");

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

  _diff_coeff(getParam<std::vector<Real> >("diffusivity_multipliers")),

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
  _rx_rates(declareProperty<std::vector<std::vector<Real> > >("rx_rates"))

{
  _initialized = false;       // flag to see if coefficients were resized

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

  // std::cout << "_rx_coeffs[i][j]" << std::endl;
  // for ( int i=0; i<_N; ++i )
  // {
  //   for ( int j=0; j<_N; ++j )
  //   {
  //     std::cout << _rx_coeffs[i][j] << " ";
  //   }
  //   std::cout << std::endl;
  // }

  //   for ( int j=0; j<_N; ++j )
  //     _input_rx_coeffs[i][j] = 0;
  // }

  // if ( _input_num != _var_num )
  // {
  //   std::stringstream errorMsg;
  //   errorMsg << "In HomNucleationMaterial: Size of input_coeffs is wrong.\n"
  //            << "\tnumber of variables: " << _var_num << "\tnumber of input coefficients: " << _input_num;
  //   mooseError(errorMsg.str());
  // }
}

void
HomNucleationMaterial::initialize()
{
  for( unsigned int qp(0); qp < _qrule->n_points(); ++qp)
  {
    // resize and set default values
    _diffusivities[qp].resize(_N);

    // resize
    _rx_rates[qp].resize(_N);
  }
  _initialized = true;
}

void
HomNucleationMaterial::computeProperties()
{
  if (!_initialized)
    initialize();

  for( unsigned int qp(0); qp < _qrule->n_points(); ++qp) // iterate over quadrature points
  {
    for ( int i=0; i<_N; ++i )
    {
      _diffusivities[qp][i] = _diff_coeff[i]; // TODO: need to create temperature dependent diffusivity
    }
  
    for ( int i=0; i<_N; ++i )
    {
      _rx_rates[qp][i].insert(_rx_rates[qp][i].end(), _rx_coeffs[i].begin(), _rx_coeffs[i].end());
    }
  }

  // for ( int i=0; i<_N; ++i )
  // {
  //   for ( int j=0; j<_N; ++j )
  //   {
  //     std::cout << _rx_rates[0][i][j] << " ";
  //   }
  //   std::cout << std::endl;
  // }

  // for ( int i=0; i<_N; ++i )
  //   std::cout << _c1_rx_coeffs[i] << " ";
  // std::cout << std::endl;
}
