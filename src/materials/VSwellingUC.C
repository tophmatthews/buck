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
  params.addCoupledVar("burnup", 0, "Coupled Burnup");
//  params.addRequiredParam<Real>("initial_density", "Initial fuel density");
//  params.addParam<Real>("total_densification",0.034, "The densification that will occur given as a fraction of theoretical density");
//  params.addParam<Real>("complete_burnup",0.05, "The burnup at which densification is complete input in units of MWd/kgU");
  params.addCoupledVar("temp", 0, "Coupled Temperature");
  params.addParam<Real>("solid_factor", 0.5, "Factor multiplied against Burnup [FIMA] to determine swelling due to solid fission products");
  params.addParam<bool>("calculate_gas_swelling", true, "Flag to calculate gas swelling");
  params.addParam<bool>("save_solid_swell", false, "Should the solid swelling be saved in a material property");
  params.addParam<bool>("save_gas_swell", false, "Should the gaseous swelling be saved in a material property");
  // params.addParam<bool>("save_densification", false, "Should the densification be saved in a material property");

  return params;
}

VSwellingUC::VSwellingUC( const std::string & name, InputParameters parameters)
  :VolumetricModel( name, parameters ),

   _burnup(coupledValue("burnup")),
   _burnup_old(coupledValueOld("burnup")),
        
   _temperature(coupledValue("temp")),
   _temperature_old(coupledValueOld("temp")),

   _solid_factor(parameters.get<Real>("solid_factor")),
   _calc_gas_swell(parameters.get<bool>("calculate_gas_swelling")),

   _density(getMaterialProperty<Real>("density")),
   _density_old(getMaterialPropertyOld<Real>("density")),

   _zone(getMaterialProperty<Real>("zone")),
   _zone_old(getMaterialPropertyOld<Real>("zone")),
   _T2(getMaterialProperty<Real>("T2")),
   _T2_old(getMaterialPropertyOld<Real>("T2")),

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
  if ( parameters.get<bool>("save_gas_swell"))
  {
    _gas_swell = &declareProperty<Real>("gas_swell");
    _gas_swell_old = &declarePropertyOld<Real>("gas_swell");
  }
  // if (parameters.get<bool>("save_densification"))
  // {
  //   _densification = &declareProperty<Real>("densification");
  //   _densification_old = &declarePropertyOld<Real>("densification");
  // }
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
  if ( _t_step != 0 )
  {
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
    solidSwelling( _solid_factor, _burnup_old[qp], ssStrainOld, dssStrain_dTOld );
    solidSwelling( _solid_factor, _burnup[qp], ssStrain, dssStrain_dT );
    if (_solid_swell)
    {
      (*_solid_swell)[qp] = ssStrain;
    }

    // Gaseous swelling
    Real gsStrain(0.0);
    Real gsStrainOld(0.0);
    Real dgsStrain_dT(0.0);
    Real dgsStrain_dTOld(0.0);
    if ( _calc_gas_swell )
    {
      if ( _zone[qp] == 3 || _zone[qp] == 4)
      {
        gaseousSwelling( _zone[qp], _T2[qp], _density[qp], _burnup[qp], _temperature[qp], gsStrain, dgsStrain_dT );
        gaseousSwelling( _zone_old[qp], _T2_old[qp], _density_old[qp], _burnup_old[qp], _temperature_old[qp], gsStrainOld, dgsStrain_dTOld );
      }
    }

    if (_gas_swell && _gas_swell_old && _calc_gas_swell)
    {
      (*_gas_swell)[qp] = gsStrain;
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
    const Real VStrain = std::pow((ssStrain + gsStrain + dStrain + 1)*v0OverVOld, oneThird) - std::pow((ssStrainOld + gsStrainOld + dStrainOld + 1)*v0OverVOld, oneThird);
    const Real dVStrain_dT = (dssStrain_dT - dssStrain_dTOld) + (dgsStrain_dT - dgsStrain_dTOld) + (ddStrain_dT - ddStrain_dTOld);

    strain_increment.addDiag( -VStrain );
    dstrain_increment_dT.addDiag( -dVStrain_dT/3 * v0OverVOld );

  }
}

//////////////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////////////

void
VSwellingUC::gaseousSwelling( const Real zone,
                              const Real T2,
                              const Real fuel_density,
                              const Real burnup,
                              const Real temperature,
                              Real & fract_volumetric,
                              Real & dfract_volumetric_dtemp )
{
  // Swelling calculated from Blank, "Nonoxide Ceramic Nuclear Fuels," (1994) pg 318
  // Swelling contribution broken into P1 and P2 bubble contributions
  //
  // Zones 1 and 2 contribute no swelling.
  //
  // P1 in Zones 3 & 4:
  //
  // mu1 = S1 * (F- F01)  [%]
  //   S1 = 0.032 [% per a/o]
  //   F01 = 1.5 [a/o]
  //   F = Burnup [a/o]
  //
  // P2 in Zone 3 & 4
  //
  // mu2+ = (S2+) ( F - F02) [%]
  // mu2 = (mu2+) - K2 ( F - F02 ) ( T2* - T ) [%] 
  //   S2+ = 0.613 [% per a/o]
  //   F02 = 0.83 [a/o]
  //   T2* is the critical temperature calculated in ZonalUC
  //   F = burnup [a/o]
  // 
  //   in zone 4: K2 = 0.0018
  //   in zone 3: K2 = 0.014
  //

  const Real F = burnup * 100; // turns burnup from [FIMA] to [a/o]

  // P1 strain
  const Real S1( 0.032 ); // [% per a/o]
  const Real F01( 1.5 ); // [a/o]
  Real gsStrainP1 = S1 * (F - F01);

  //gsStrainP1 = 0;

  //P2 strain
  const Real S2( 0.613 ); // [% per a/o]
  const Real F02( 0.83 ); // [a/o]
  const Real mu = S2 * ( F - F02 );

  Real K2;
  if ( zone == 4 )
    K2 = 0.0018;
  else if ( zone == 3 )
    K2 = 0.014;

  Real gsStrainP2 = mu - K2 * ( F - F02 ) * ( T2 - temperature );
  Real dgsStrainP2 = K2 * ( F - F02 );

  if ( gsStrainP1 < 0 )
    gsStrainP1 = 0;
  if ( gsStrainP2 < 0)
    gsStrainP2 = 0;
  if ( dgsStrainP2 < 0 )
    dgsStrainP2 = 0;

  fract_volumetric = ( gsStrainP1 + gsStrainP2 ) / 100.0;  // increment of fractonal volume gaseous swelling [/]
  dfract_volumetric_dtemp = dgsStrainP2 / 100.0;
}

//////////////////////////////////////////////////////////////////////////////////////

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
