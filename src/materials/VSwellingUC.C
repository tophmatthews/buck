#include "VSwellingUC.h"

#include "SymmTensor.h"

/*////////////////////////////////
 *
 *note that this override of modifyStrain is for STRAIN RATE INCREMENTS
 *
 *//////////////////////////////

template<>
InputParameters validParams<VSwellingUC>()
{
  InputParameters params = validParams<VolumetricModel>();
  params.addCoupledVar("burnup", "Coupled Burnup");
//  params.addRequiredParam<Real>("initial_density", "Initial fuel density");
//  params.addParam<Real>("total_densification",0.034, "The densification that will occur given as a fraction of theoretical density");
//  params.addParam<Real>("complete_burnup",0.05, "The burnup at which densification is complete input in units of MWd/kgU");
  params.addCoupledVar("temp", "Coupled Temperature");
  params.addPrivateParam("solid_factor", 0.5);
  params.addParam<bool>("save_solid_swell", false, "Should the solid swelling be saved in a material property");
  params.addParam<bool>("save_gas_swell", false, "Should the gaseous swelling be saved in a material property");
  params.addParam<bool>("save_densification", false, "Should the densification be saved in a material property");

  return params;
}

VSwellingUC::VSwellingUC( const std::string & name, InputParameters parameters)
  :VolumetricModel( name, parameters ),
   _has_burnup(isCoupled("burnup")),

   _burnup(_has_burnup ? coupledValue("burnup") : _zero),
   _burnup_old(_has_burnup ? coupledValueOld("burnup") : _zero),
        
//   _density(getParam<Real>("density")),
   _has_temp(isCoupled("temp")),
        
   _temperature(_has_temp ? coupledValue("temp") : _zero),
   _temperature_old(_has_temp ? coupledValueOld("temp") : _zero),
   _solid_factor(parameters.get<Real>("solid_factor")),
   _solid_swell(NULL),
   _solid_swell_old(NULL),
   _gas_swell(NULL),
   _gas_swell_old(NULL),
   _densification(NULL),
   _densification_old(NULL)
{
  if (parameters.get<bool>("save_solid_swell"))
  {
    _solid_swell = &declareProperty<Real>("solid_swell");
    _solid_swell_old = &declarePropertyOld<Real>("solid_swell");
  }
  // if (true || parameters.get<bool>("save_gas_swell"))
  {
    _gas_swell = &declareProperty<Real>("gas_swell");
    _gas_swell_old = &declarePropertyOld<Real>("gas_swell");
  }
  if (parameters.get<bool>("save_densification"))
  {
    _densification = &declareProperty<Real>("densification");
    _densification_old = &declarePropertyOld<Real>("densification");
  }
  
  if ( !_has_burnup )
    mooseError("VSwellingUC: burnup required");
}

////////////////////////////////////////////////////////////////////////

void
VSwellingUC::initStatefulProperties(unsigned n_points)
{
  for (unsigned qp(0); qp < n_points; ++qp)
  {
    if (_solid_swell)
      (*_solid_swell)[qp] = 0.;
    if (_solid_swell_old)
      (*_solid_swell_old)[qp] = 0.;
    if (_gas_swell)
      (*_gas_swell)[qp] = 0.;
    if (_gas_swell_old)
      (*_gas_swell_old)[qp] = 0.;
    if (_densification)
      (*_densification)[qp] = 0.;
    if (_densification_old)
      (*_densification_old)[qp] = 0.;
  }
}

void
VSwellingUC::modifyStrain(const unsigned int qp,
                           const Real v0OverVOld,
                           SymmTensor & strain_increment,
                           SymmTensor & dstrain_increment_dT)
{
  if ( _has_temp && _has_burnup && _t_step != 0 )
  {
    const Real burnup = _burnup[qp];
    const Real burnup_old = _burnup_old[qp];
    const Real burnup_increment = _burnup[qp] - _burnup_old[qp];
    const Real temperature = _temperature[qp];
    const Real temperature_old = _temperature_old[qp];

    //
    // Determinant of deformation gradient is current volume over original volume:
    // det(F) = V/V0
    // If F0 is the diagonal value of F for volumetric strain,
    // det(F) = F0^3
    // F0^3 = V/V0
    // F0 = (V/V0)^(1/3) = ((V0+dV)/V0)^(1/3) = (1+dV/V0)^(1/3)
    // Therefore, the volumetric strain increment is
    // F0_n - F0_(n-1)
    //



    // Solid swelling
    Real ssStrainOld(0);
    Real ssStrain(0);
    Real dssStrain_dTOld(0);
    Real dssStrain_dT(0);
    solidSwelling( _solid_factor, burnup_old, ssStrainOld, dssStrain_dTOld );
    solidSwelling( _solid_factor, burnup,     ssStrain,    dssStrain_dT );
    if (_solid_swell)
    {
      (*_solid_swell)[qp] = ssStrain;
    }

    // Gaseous swelling
    Real gsStrainInc(0);
    Real dgsStrain_dT(0);
//    gaseousSwelling( _gas_factor, _density, burnup, burnup_increment, temperature, gsStrainInc, dgsStrain_dT );
    if (_gas_swell && _gas_swell_old)
    {
      (*_gas_swell)[qp] = (*_gas_swell_old)[qp] + gsStrainInc;
    }

    // Densification
    Real dStrainOld(0);
    Real dStrain(0);
    Real ddStrain_dTOld(0);
    Real ddStrain_dT(0);
//    densification( _total_densification, _complete_burnup, _constant_dens_c_d, temperature_old, burnup_old, dStrainOld, ddStrain_dTOld );
//    densification( _total_densification, _complete_burnup, _constant_dens_c_d, temperature,     burnup,     dStrain,    ddStrain_dT );
    if (_densification && _densification_old)
    {
      (*_densification)[qp] = dStrain;
    }

    // Volumetric strain increments
    const Real oneThird = 1./3.;
    const Real VStrain = std::pow((ssStrain + (*_gas_swell)[qp] + dStrain + 1)*v0OverVOld, oneThird) - std::pow((ssStrainOld + (*_gas_swell_old)[qp] + dStrainOld + 1)*v0OverVOld, oneThird);
    const Real dVStrain_dT = (dssStrain_dT-dssStrain_dTOld) + dgsStrain_dT + (ddStrain_dT-ddStrain_dTOld);

    strain_increment.addDiag( -VStrain );
    dstrain_increment_dT.addDiag( -dVStrain_dT/3 * v0OverVOld );

  }
}

//////////////////////////////////////////////////////////////////

void
VSwellingUC::solidSwelling( const Real solid_swelling_factor,
                             const Real burnup,
                             Real & fract_volumetric,
                             Real & dfract_volumetric_dtemp )
{
  // This function computes the solid fission product swelling in UC according to:
  // Matzke: Science of Advanced LMFBR Fuels, pg 466
  //
  // dV/V = 0.5 * Bu (FIMA)
  //
  fract_volumetric = solid_swelling_factor * burnup;
  dfract_volumetric_dtemp = 0.;
}

// FIXME: add in gaseous swelling for UC
//void
//VSwellingUC::gaseousSwelling( const Real gaseous_swelling_factor,
//                               const Real fuel_density,
//                               const Real burnup,
//                               const Real burnup_increment,
//                               const Real temperature,
//                               Real & fract_volumetric_increment,
//                               Real & dfract_volumetric_increment_dtemp )
//{
//  // This function computes the gaseous fission product swelling according to:
//  // MATPRO - A Library of Material Properties for LWR Accident Analysis, INEEL/EXT-02-00589 (2003).
//  //
//  // delta e_sw-g = 8.8e-56*(2800-T)^11.73*exp(-.0162*(2800-T))*exp(-8e-27*Bu)*delBu
//  //   where T is Kelvin
//  //         Bu is fissions/m^3
//  //
//  // fission | 6.023e23 atom | rho kg-UO2    fission                 fission
//  // ------------------------------------ -> ------- = 2.23e24 * rho -------
//  //  atom   |  .270 kg-UO2  |  m^3-UO2       atom                   m^3-UO2
//  //
//  // Using this to convert units, we get
//  //
//  // delta e_sw-g =
//  //     1.96e-31*rho*(2800-T)^11.73*exp(-.0162*(2800-T))*exp(-1.78e-2*rho*Bu)*delBu
//
//  const Real gsStrainC0( gaseous_swelling_factor * fuel_density * burnup_increment * std::exp(-0.0178 * fuel_density * burnup) );
//  const Real gsStrain_pow( std::pow(2800. - temperature, 11.73) );
//  const Real gsStrain_exp( std::exp(-0.0162 * (2800. - temperature)) );
//
//  fract_volumetric_increment = gsStrainC0 * gsStrain_pow * gsStrain_exp;  // increment of fractonal volume gaseous swelling [/]
//  dfract_volumetric_increment_dtemp = gsStrainC0 * ( -11.73 * std::pow(2800. - temperature, 10.73 ) * gsStrain_exp +
//                                      gsStrain_pow * gsStrain_exp * 0.0162 );
//}
// FIXME: add densification for UC
//void
//VSwellingUC::densification( const Real total_densification,
//                            const Real complete_densification_burnup,
//                            const Real density_ratio,
//                            const Real burnup,
//                            const Real burnup_old,
//                            Real & fract_volumetric,
//                            Real & dfract_volumetric_dtemp )
//{
//  // Taken from W. Dienst, JNM 124 (1984) 153-158
//  // densification porosity: p = p_total * (1 - exp( -burnup / 0.6 at% ) )
//  // dp = p_total * ( exp( -burnup_old / 0.6 ) - exp( -burnup / 0.6 ) )
//  
//  if (burnup_old >= complete_densification_burnup)
//  {
//    fract_volumetric = 0;
//    dfract_volumentric_dtemp = 0;
//  }
//  else
//  {
//    const Real C(0.6);
//    const Real Ex(std::exp( -burnup / C ));
//    const Real Ex_old(std::exp( -burnup_old / C ));
//
//    // increment of fractonal volume gaseous swelling [/]
//    fract_volumetric = - total_densification * (exp_old - exp) * density_ratio;
//    dfract_volumetric_dtemp = 0;
//  }
//}
