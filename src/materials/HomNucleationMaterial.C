#include "HomNucleationMaterial.h"
#include "MooseEnum.h"

template<>
InputParameters validParams<HomNucleationMaterial>()
{
  InputParameters params = validParams<Material>();
  
  params.addRequiredParam<int>("N_nuc", "Largest cluster size.");

  params.addParam<Real>("omega", 3.0e-2, "Lattice site volume [nm**3]");
  params.addParam<Real>("a", 0.5, "Lattice parameter [nm]");
  params.addParam<bool>("cluster_diffusion", false, "Flag for whether or not clusters move");

  params.addParam<std::vector<Real> >("diffusivity_multipliers", "Multipliers to diffusivity");

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

  _N(getParam<int>("N_nuc")),
  _omega(getParam<Real>("omega")),
  _a(getParam<Real>("a")),
  _cluster_diffusion(getParam<bool>("cluster_diffusion")),

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

  _rx_rates(declareProperty<std::vector<std::vector<Real> > >("rx_rates")),
  _cluster_diffusivities(declareProperty<std::vector<Real> >("cluster_diffusivities")),

  _atomic_diffusivity(getMaterialProperty<Real>("atomic_diffusivity")),
  _initialized(false) // flag to see if coefficients were resized

{
  // Protect against not the right number of input for a needed cN coefficient
  if ( !_cluster_diffusion )
  {
    if ( _c1_rx_coeffs.size() != _N )
    {
      std::stringstream errorMsg;
      errorMsg << "In HomNucleationMaterial: Size of a c1_rx_coeff is wrong.\n"
               << "\tRequired size: " << _N << std::endl;
      mooseError(errorMsg.str());
    }
    // _rx_coeffs.resize(1);
    // _rx_coeffs[0].insert( _rx_coeffs[0].end(), _c1_rx_coeffs.begin(), _c1_rx_coeffs.end() );
  }

  else
  {
    // Protect against not the right number of input for a needed cN coefficient
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

    // Protect against not the right number of input for a needed cN coefficient
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

    if ( _diffusivity_multipliers.size() != _N-1 )
    {
      std::stringstream errorMsg;
      errorMsg << "In HomNucleationMaterial: The number of diffusivity_multipliers is incorrect .\n"
               << "\tRequired number: " << _N-1 << std::endl;
      mooseError(errorMsg.str());
    }
  }
  _rx_coeffs.resize(9);
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
    _rx_rates[qp].resize(_N);

    for ( int i=0; i<_N; ++i )
    {
      _cluster_diffusivities[qp].push_back(0);
      _rx_rates[qp][i].resize(_N);
      {
      for ( int j=0; j<_N; ++j )
        _rx_rates[qp][i][j] = 0;
      }
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
    if ( !_cluster_diffusion )
    {
      // Only assign rx_rates to the single gas atom
      for ( int j=0; j<_N; ++j)
        _rx_rates[qp][0][j] = _omega / _a / _a * _atomic_diffusivity[qp] * _rx_coeffs[0][j];
    }
    else
    {
      // Assign the first rx_rate to the single gas atom, and then include the remainder of the clusters
      _cluster_diffusivities[qp][0] = _atomic_diffusivity[qp];
      for ( int i=0; i<_N-1; ++i )
        _cluster_diffusivities[qp][i+1] = _atomic_diffusivity[qp] * _diffusivity_multipliers[i];
  
      for ( int i=0; i<_N; ++i )
      {
        for (int j=1; j<_N; ++j)
          _rx_rates[qp][i][j] = _omega / _a / _a * _cluster_diffusivities[qp][i] * _rx_coeffs[i][j];       
      }
    }
  }
  // std::cout << "rates" << std::endl;
  // for ( int i=0; i<_N; ++i)
  // {
  //   for ( int j=0; j<_N; ++j)
  //   {
  //     std::cout << _rx_rates[0][i][j] << " ";
  //   }
  //   std::cout << std::endl;
  // }
}
