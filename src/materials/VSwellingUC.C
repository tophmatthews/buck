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
  params.addParam<Real>("total_densification",0.034, "The total fractional densification that can occur");
  params.addParam<Real>("burnup_constant",0.006, "Constant that divides burnup in Densifcation equation in units of FIMA");
  params.addCoupledVar("temp", 0, "Coupled Temperature");
  params.addParam<Real>("solid_factor", 0.5, "Factor multiplied against Burnup [FIMA] to determine swelling due to solid fission products");
  params.addParam<bool>("calculate_gas_swelling", true, "Flag to calculate gas swelling");
  params.addParam<bool>("save_solid_swell", false, "Should the solid swelling be saved in a material property");
  params.addParam<bool>("save_gas_swell", false, "Should the gaseous swelling be saved in a material property");
  params.addParam<bool>("save_densification", false, "Should the densification be saved in a material property");
  params.addParam<bool>("allow_central_swelling", false, "Should swelling occur in zones 1 and 2");

  return params;
}

VSwellingUC::VSwellingUC( const std::string & name, InputParameters parameters)
  :VolumetricModel( name, parameters ),

   _burnup(coupledValue("burnup")),
   _burnup_old(coupledValueOld("burnup")),
        
   _temperature(coupledValue("temp")),
   _temperature_old(coupledValueOld("temp")),

   _total_densification(parameters.get<Real>("total_densification")),
   _burnup_constant(parameters.get<Real>("burnup_constant")),
   _solid_factor(parameters.get<Real>("solid_factor")),
   _calc_gas_swell(parameters.get<bool>("calculate_gas_swelling")),
   _allow_central_swelling(parameters.get<bool>("allow_central_swelling")),

   _zone(_calc_gas_swell ? &getMaterialProperty<Real>("zone") : NULL),
   _T2(_calc_gas_swell ? &getMaterialProperty<Real>("T2") : NULL),
   _T3(_calc_gas_swell ? &getMaterialProperty<Real>("T3") : NULL),

   _zone_old(_calc_gas_swell ? &getMaterialPropertyOld<Real>("zone") : NULL),
   _T2_old(_calc_gas_swell ? &getMaterialPropertyOld<Real>("T2") : NULL),
   _T3_old(_calc_gas_swell ? &getMaterialPropertyOld<Real>("T3") : NULL),

   _solid_swelling(NULL),
   _P1_swelling(NULL),
   _P2_swelling(NULL),
   _P3_swelling(NULL),
   _densification(NULL),

   _solid_swelling_old(NULL),
   _P1_swelling_old(NULL),
   _P2_swelling_old(NULL),
   _P3_swelling_old(NULL),
   _densification_old(NULL)
{
  if (parameters.get<bool>("save_solid_swell"))
  {
    _solid_swelling = &declareProperty<Real>("solid_swell");
    _solid_swelling_old = &declarePropertyOld<Real>("solid_swell");
  }
  if ( parameters.get<bool>("save_gas_swell") && _calc_gas_swell )
  {
    _P1_swelling = &declareProperty<Real>("P1_swelling");
    _P2_swelling = &declareProperty<Real>("P2_swelling");
    _P3_swelling = &declareProperty<Real>("P3_swelling");

    _P1_swelling_old = &declarePropertyOld<Real>("P1_swelling");
    _P2_swelling_old = &declarePropertyOld<Real>("P2_swelling");
    _P3_swelling_old = &declarePropertyOld<Real>("P3_swelling");
  }
  if (parameters.get<bool>("save_densification"))
  {
    _densification = &declareProperty<Real>("densification");
    _densification_old = &declarePropertyOld<Real>("densification");
  }
}

////////////////////////////////////////////////////////////////////////

void
VSwellingUC::initStatefulProperties(unsigned n_points)
{
  for (unsigned qp(0); qp < n_points; ++qp)
  {
    if (_solid_swelling)
      (*_solid_swelling)[qp] = 0.;
    if (_solid_swelling_old)
      (*_solid_swelling_old)[qp] = 0.;

    if (_P1_swelling)
      (*_P1_swelling)[qp] = 0.;
    if (_P1_swelling_old)
      (*_P1_swelling_old)[qp] = 0.;

    if (_P2_swelling)
      (*_P2_swelling)[qp] = 0.;
    if (_P2_swelling_old)
      (*_P2_swelling_old)[qp] = 0.;

    if (_P3_swelling)
      (*_P3_swelling)[qp] = 0.;
    if (_P3_swelling_old)
      (*_P3_swelling_old)[qp] = 0.;

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
    solidSwelling( _solid_factor, _burnup[qp],     ssStrain,    dssStrain_dT );

    if (_solid_swelling)
    {
      (*_solid_swelling)[qp] = ssStrain;
    }

    // P1 swelling
    Real P1Strain(0.0);
    Real P1StrainOld(0.0);
    Real dP1Strain_dT(0.0);
    Real dP1Strain_dTOld(0.0);
    if ( _calc_gas_swell )
    {
      if ( _allow_central_swelling || (*_zone_old)[qp] == 3 || (*_zone_old)[qp] == 4 )
      {
        calcP1Swelling( _burnup[qp],     P1Strain,    dP1Strain_dT );
        calcP1Swelling( _burnup_old[qp], P1StrainOld, dP1Strain_dTOld );
        if ( _P1_swelling)
          (*_P1_swelling)[qp] = P1Strain;
      }
    }

    // P2 swelling
    Real P2Strain(0.0);
    Real P2StrainOld(0.0);
    Real dP2Strain_dT(0.0);
    Real dP2Strain_dTOld(0.0);
    if ( _calc_gas_swell )
    {
      if ( _allow_central_swelling || (*_zone_old)[qp] == 3 || (*_zone_old)[qp] == 4 )
      {
        calcP2Swelling( _burnup[qp],     _temperature[qp],     (*_zone)[qp],     (*_T2)[qp],     P2Strain,    dP2Strain_dT );
        calcP2Swelling( _burnup_old[qp], _temperature_old[qp], (*_zone_old)[qp], (*_T2_old)[qp], P2StrainOld, dP2Strain_dTOld );
        if ( _P2_swelling)
          (*_P2_swelling)[qp] = P2Strain;
      }
    }

    // P3 swelling
    Real P3Strain(0.0);
    Real P3StrainOld(0.0);
    Real dP3Strain_dT(0.0);
    Real dP3Strain_dTOld(0.0);
    if ( _calc_gas_swell )
    {
      if ( _allow_central_swelling || (*_zone_old)[qp] == 3 || (*_zone_old)[qp] == 4 )
      {
        calcP3Swelling( _temperature[qp],     (*_T3)[qp],     _burnup[qp],     P3Strain,    dP3Strain_dT );
        calcP3Swelling( _temperature_old[qp], (*_T3_old)[qp], _burnup_old[qp], P3StrainOld, dP3Strain_dTOld );
        if ( _P3_swelling)
          (*_P3_swelling)[qp] = P3Strain;
      }
    }

    if ( _t_step == 1 )
    {
      P1StrainOld = 0;
      P2StrainOld = 0;
      P3StrainOld = 0;
      dP1Strain_dTOld = 0;
      dP2Strain_dTOld = 0;
      dP3Strain_dTOld = 0;
    }

    // Densification
    Real dStrainOld(0);
    Real dStrain(0);
    Real ddStrain_dTOld(0);
    Real ddStrain_dT(0);
    densification( _total_densification, _burnup_constant, _burnup_old[qp], dStrainOld, ddStrain_dTOld );
    densification( _total_densification, _burnup_constant, _burnup[qp],     dStrain,    ddStrain_dT );
    if (_densification && _densification_old)
    {
      (*_densification)[qp] = dStrain;
    }

    // Volumetric strain increments
    const Real oneThird = 1./3.;
    const Real VStrain = std::pow((ssStrain +    P1Strain +    P2Strain +    P3Strain +    dStrain +    1)*v0OverVOld, oneThird) - \
                         std::pow((ssStrainOld + P1StrainOld + P2StrainOld + P3StrainOld + dStrainOld + 1)*v0OverVOld, oneThird);
    const Real dVStrain_dT = (dssStrain_dT - dssStrain_dTOld) + \
                             (dP1Strain_dT - dP1Strain_dTOld) + \
                             (dP2Strain_dT - dP2Strain_dTOld) + \
                             (dP3Strain_dT - dP3Strain_dTOld) + \
                             (ddStrain_dT  - ddStrain_dTOld);

    strain_increment.addDiag( -VStrain );
    dstrain_increment_dT.addDiag( -dVStrain_dT/3 * v0OverVOld );
    // std::cout << _t_step << std::endl;
    // std::cout << "P1_incr " << P1Strain - P1StrainOld << " P2_incr " << P2Strain - P2StrainOld << " P3_incr " << P3Strain - P3StrainOld << std::endl;
    // std::cout << "P1_swell " << P1Strain << " P2_swell " << P2Strain << " P3_swell " << P3Strain << std::endl;
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
  
  fract_volumetric = solid_swelling_factor * burnup;
  dfract_volumetric_dtemp = 0.;
}

//////////////////////////////////////////////////////////////////////////////////////

void
VSwellingUC::calcP1Swelling( const Real burnup,
                             Real & fract_volumetric,
                             Real & dfract_volumetric_dtemp )
{
  // Swelling calculated from Blank, "Nonoxide Ceramic Nuclear Fuels," (1994) pg 318
  // Swelling contribution broken into P1 and P2 bubble contributions
  //
  // Zones 1 and 2 contribute no swelling.
  // P1 in Zones 3 & 4:
  //
  // mu1 = S1 * (F- F01)  [%]
  //   S1 = 0.032 [% per a/o]
  //   F01 = 1.5 [a/o]
  //   F = Burnup [a/o]
  
  const Real F = burnup * 100; // turns burnup from [FIMA] to [a/o]
  const Real S1( 0.032 ); // [% per a/o]
  const Real F01( 1.5 ); // [a/o]
  
  fract_volumetric = ( S1 * (F - F01) ) / 100.0;
  dfract_volumetric_dtemp = 0;

  if ( fract_volumetric < 0 )
    fract_volumetric = 0;
}

void
VSwellingUC::calcP2Swelling( const Real burnup,
                             const Real temp,
                             const Real zone,
                             const Real T2,
                             Real & fract_volumetric,
                             Real & dfract_volumetric_dtemp)
{
  // Swelling calculated from Blank, "Nonoxide Ceramic Nuclear Fuels," (1994) pg 318
  // mu2+ = (S2+) ( F - F02) [%]
  // mu2 = (mu2+) - K2 ( F - F02 ) ( T2* - T ) [%] 
  //   S2+ = 0.613 [% per a/o]
  //   F02 = 0.83 [a/o]
  //   T2* is the critical temperature calculated in ZonalUC
  //   F = burnup [a/o]
  // 
  //   in zone 4: K2 = 0.0018
  //   in zone 3: K2 = 0.014

  if ( burnup > 0.0083 )
  {
    const Real F = burnup * 100; // turns burnup from [FIMA] to [a/o]
    const Real S2( 0.613 ); // [% per a/o]
    const Real F02( 0.83 ); // [a/o]
    const Real mu = S2 * ( F - F02 );

    Real K2;
    if ( zone == 4 )
      K2 = 0.0018;
    else
      K2 = 0.014;

    fract_volumetric = ( mu - K2 * ( F - F02 ) * ( T2 - temp ) ) / 100.0;
    dfract_volumetric_dtemp = ( K2 * ( F - F02 ) ) / 100.0;
    
    if ( fract_volumetric < 0)
      fract_volumetric = 0;
    if ( dfract_volumetric_dtemp < 0 )
      dfract_volumetric_dtemp = 0;
  }

  // std::cout << "T2: " << T2 << " temp: " << temp << " fract: " << fract_volumetric << " dfract: " << dfract_volumetric_dtemp << std::endl;
}

void
VSwellingUC::calcP3Swelling( const Real temp,
                             const Real T3,
                             const Real burnup,
                             Real & fract_volumetric,
                             Real & dfract_volumetric_dtemp )
{
// Swelling calculated from Blank, "Nonoxide Ceramic Nuclear Fuels," (1994) pg 318
  if ( burnup > 0.01 )
  {
    const Real b( 0.5 ); // [% per a/o]
    
    Real m;
    if ( temp < T3 )
      m = 0.001;
    else
      m = 0.025;

    fract_volumetric = ( b - m * ( T3 - temp) ) / 100.0;
    dfract_volumetric_dtemp = m / 100.0;

    if ( fract_volumetric < 0 )
      fract_volumetric = 0;
    if ( dfract_volumetric_dtemp < 0 )
      dfract_volumetric_dtemp = 0;

    // std::cout << "T3: " << T3 << " temp: " << temp << " fract: " << fract_volumetric << " dfract: " << dfract_volumetric_dtemp << std::endl;
  }
}


//////////////////////////////////////////////////////////////////////////////////////

// FIXME: add densification for UC
void
VSwellingUC::densification( const Real total_densification,
                            const Real burnup_constant,
                            const Real burnup,
                            Real & fract_volumetric,
                            Real & dfract_volumetric_dtemp )
{
  // Taken from W. Dienst, JNM 124 (1984) 153-158
  // densification porosity: p = p_total * (1 - exp( -burnup / 0.6 at% ) )
  
  fract_volumetric = - total_densification * ( 1 - exp( - burnup / burnup_constant ) );
  dfract_volumetric_dtemp = 0;
}
