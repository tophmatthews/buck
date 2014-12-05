#include "VSwellingUr.h"

#include "SymmTensor.h"

/*////////////////////////////////
 *
 *note that this override of modifyStrain is for STRAIN RATE INCREMENTS
 *
 *//////////////////////////////

template<>
InputParameters validParams<VSwellingUr>()
{
  InputParameters params = validParams<VolumetricModel>();
  params.addCoupledVar("burnup", 0, "Coupled Burnup");
  params.addCoupledVar("temp", 0, "Coupled Temperature");
  params.addCoupledVar("porosity", 0.1, "Coupled porosity");
  params.addParam<PostprocessorName>("contact_pressure", 0, "Postpressor for contact_pressure average");
  params.addParam<Real>("total_densification",0.034, "The total fractional densification that can occur");
  params.addParam<Real>("burnup_constant",0.006, "Constant that divides burnup in Densifcation equation in units of FIMA");
  params.addParam<Real>("solid_factor", 0.417, "Factor multiplied against Burnup [FIMA] to determine swelling due to solid fission products");
  params.addParam<bool>("calculate_gas_swelling", true, "Flag to calculate gas swelling");
  params.addParam<bool>("save_solid_swell", false, "Should the solid swelling be saved in a material property");
  params.addParam<bool>("save_densification", false, "Should the densification be saved in a material property");

  return params;
}

VSwellingUr::VSwellingUr( const std::string & name, InputParameters parameters)
  :VolumetricModel( name, parameters ),

   _burnup(coupledValue("burnup")),
   _burnup_old(coupledValueOld("burnup")),
        
   _temperature(coupledValue("temp")),
   _porosity(coupledValue("porosity")),

   _contact_pressure(getPostprocessorValue("contact_pressure")),

   _total_densification(parameters.get<Real>("total_densification")),
   _burnup_constant(parameters.get<Real>("burnup_constant")),
   _solid_factor(parameters.get<Real>("solid_factor")),
   _calc_gas_swell(parameters.get<bool>("calculate_gas_swelling")),

   _solid_swelling(NULL),
   _gas_swelling(NULL),
   _densification(NULL),

   _solid_swelling_old(NULL),
   _gas_swelling_old(NULL),
   _densification_old(NULL)

{
  if (parameters.get<bool>("save_solid_swell"))
  {
    _solid_swelling = &declareProperty<Real>("solid_swell");
    _solid_swelling_old = &declarePropertyOld<Real>("solid_swell");
  }
  // if ( _calc_gas_swell )
  {
    _gas_swelling = &declareProperty<Real>("gas_swelling");
    _gas_swelling_old = &declarePropertyOld<Real>("gas_swelling");
  }
  if (parameters.get<bool>("save_densification"))
  {
    _densification = &declareProperty<Real>("densification");
    _densification_old = &declarePropertyOld<Real>("densification");
  }
}

////////////////////////////////////////////////////////////////////////

void
VSwellingUr::initStatefulProperties(unsigned n_points)
{
  for (unsigned qp(0); qp < n_points; ++qp)
  {
    if (_solid_swelling)
      (*_solid_swelling)[qp] = 0.;
    if (_solid_swelling_old)
      (*_solid_swelling_old)[qp] = 0.;

    if (_gas_swelling)
      (*_gas_swelling)[qp] = 0.;
    if (_gas_swelling_old)
      (*_gas_swelling_old)[qp] = 0.;

    if (_densification)
      (*_densification)[qp] = 0.;
    if (_densification_old)
      (*_densification_old)[qp] = 0.;
  }
}

void
VSwellingUr::modifyStrain(const unsigned int qp,
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

    if ( _solid_swelling )
    {
      (*_solid_swelling)[qp] = ssStrain;
    }
    
    Real gsStrainRate(0);
    Real dgsStrainRate_dT(0);
    calcGasSwelling( _burnup[qp], _burnup_old[qp], _temperature[qp], _porosity[qp], _contact_pressure, gsStrainRate, dgsStrainRate_dT );
    if ( _calc_gas_swell )
    {
      (*_gas_swelling)[qp] =  (*_gas_swelling_old)[qp] + gsStrainRate;
    }

    // Densification
    Real dStrainOld(0);
    Real dStrain(0);
    Real ddStrain_dTOld(0);
    Real ddStrain_dT(0);
    densification( _total_densification, _burnup_constant, _burnup_old[qp], dStrainOld, ddStrain_dTOld );
    densification( _total_densification, _burnup_constant, _burnup[qp],     dStrain,    ddStrain_dT );
    if ( _densification )
    {
      (*_densification)[qp] = dStrain;
    }

    // Volumetric strain increments
    const Real oneThird = 1./3.;
    const Real VStrain = std::pow((ssStrain + (*_gas_swelling)[qp] / v0OverVOld + dStrain + 1)*v0OverVOld, oneThird) - \
                         std::pow((ssStrainOld + (*_gas_swelling_old)[qp] / v0OverVOld + dStrainOld + 1)*v0OverVOld, oneThird);
    const Real dVStrain_dT = (dssStrain_dT - dssStrain_dTOld) + dgsStrainRate_dT + (ddStrain_dT  - ddStrain_dTOld);

    strain_increment.addDiag( -VStrain );
    dstrain_increment_dT.addDiag( -dVStrain_dT/3 * v0OverVOld );
    // std::cout << "step: " << _t_step << " strain: " << gsStrainRate << " v0OverVOld: " << v0OverVOld << " contact_pressure: " << _contact_pressure << std::endl;
  }
}

//////////////////////////////////////////////////////////////////////////////////////

void
VSwellingUr::solidSwelling( const Real solid_swelling_factor,
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
VSwellingUr::calcGasSwelling( const Real burnup,
                              const Real burnup_old,
                              const Real temp,
                              const Real por,
                              const Real cont_pres,
                              Real & fract_volumetric,
                              Real & dfract_volumetric_dtemp )
{
  const Real c_gs( 1.528 );
  const Real d1( 12.95 );
  const Real d2( -0.0281 );
  const Real d3( 1.52e-5 );
  const Real BU0( .0112 ); // [FIMA] from 1 MWd/kg = .00112 FIMA
  const Real a( 2.0 );
  const Real b( 0.1 );
  const Real pc0( 1e6 ); //[Pa]

  const Real limit( 3.653 ); // volume swelling rate maximum, [1/FIMA]

  // Porosity correction
  Real corr_por = std::exp( - (por - 0.04));
  if ( corr_por > 1.0 )
    corr_por = 1.0;

  // Contact stress correction
  Real corr_stress = std::exp( - cont_pres / pc0 * b );
  if ( corr_stress > 1 )
    corr_stress = 1;

  // Burnup correction
  Real corr_burnup = burnup / BU0 - a;
  if ( corr_burnup < 0.0 )
    corr_burnup = 0.0;

  // Temperature correction
  Real corr_temp = 0.0;
  Real dcorr_temp = 0.0;
  if ( temp > 973.0 )
  {
    corr_temp = d1 + d2 * temp + d3 * temp * temp;
    dcorr_temp = d2 + 2.0 * d3 * temp;
  }

  fract_volumetric = ( c_gs + corr_temp * corr_burnup ) * corr_por * corr_stress;
  dfract_volumetric_dtemp = dcorr_temp * corr_burnup * corr_por * corr_stress;

  if ( fract_volumetric > limit )
  {
    fract_volumetric = limit;
    dfract_volumetric_dtemp = 0; // I think?
  }
  if ( fract_volumetric < 0 )
  {
    fract_volumetric = 0;
    dfract_volumetric_dtemp = 0;
  }
    
  const Real burnup_inc = burnup - burnup_old;
  fract_volumetric *= burnup_inc;
  dfract_volumetric_dtemp *= burnup_inc;
}

//////////////////////////////////////////////////////////////////////////////////////

void
VSwellingUr::densification( const Real total_densification,
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
