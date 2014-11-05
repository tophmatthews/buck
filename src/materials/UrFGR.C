#include "UrFGR.h"
#include "MooseEnum.h"
#include "math.h"

template<>
InputParameters validParams<UrFGR>()
{
  InputParameters params = validParams<Material>();
  
  params.addParam<Real>("frac_yield", 0.25, "Fraction of fissions that result in a fission gas atom");

  params.addParam<bool>("testing", false, "Turns on/off testing output");

  params.addCoupledVar("temp", 0, "Coupled Temperature");
  params.addCoupledVar("fission_rate",0, "Coupled Fission Rate");
  params.addCoupledVar("burnup", 0, "Coupled Burnup");

  params.addParam<PostprocessorName>("center_temp", 0, "Postpressor for center temperature");

  return params;
}

UrFGR::UrFGR(const std::string & name, InputParameters parameters) :
  Material(name, parameters),
        
  _frac_yield(getParam<Real>("frac_yield")),

  _fission_rate(coupledValue("fission_rate")),  // fission rate [1/(m**3*s)]
  _temp(coupledValue("temp")),  // temperature [K]
  _burnup(coupledValue("burnup")),  // local burnup [FIMA]

  _fission_rate_old(coupledValueOld("fission_rate")),
  _temp_old(coupledValueOld("temp")),
  _burnup_old(coupledValueOld("burnup")),

  _gas_gen(declareProperty<Real>("gas_gen")),  // volume concentration of gas produced [mol/m**3]
  _gas_rel(declareProperty<Real>("gas_rel")),   // volume concentration of gas released [mol/m**3]

  _gas_gen_old(declarePropertyOld<Real>("gas_gen")),
  _gas_rel_old(declarePropertyOld<Real>("gas_rel")),

  _center_temp(getPostprocessorValue("center_temp")),

  // constants
  _R(8.314), // Ideal Gas constant [J/mol/K]
  _avogadros_num(6.022e+23),

  _testing(getParam<bool>("testing"))
{}



void
UrFGR::initQpStatefulProperties()
{
  _gas_gen[_qp] = 0.;
  _gas_rel[_qp] = 0.;

  _gas_gen_old[_qp] = 0.;
  _gas_rel_old[_qp] = 0.;
}

void
UrFGR::computeProperties()
{
  if( _t_step == 0 || _t <= 0. )
  {
    for (unsigned int qp=0; qp<_qrule->n_points(); ++qp)
    {
      _gas_gen[qp] = 0.;
      _gas_rel[qp] = 0.;

      _gas_gen_old[qp] = 0.;
      _gas_rel_old[qp] = 0.;
    }
    return;
  }

  for(unsigned int qp=0; qp<_qrule->n_points(); ++qp)
  {
    if (_temp[qp] < 0.0)
      mooseError("UrFGR: Negative temperature in element.");

    // Generate fission gas
    const Real yield = _frac_yield / _avogadros_num;  // yield of fission gas (Xe + Kr) [mol]
    const Real gas_gen_rate     = _fission_rate[qp]     * yield;
    Real old_fsnrate = _fission_rate_old[qp];
    if( _t_step == 1)
      old_fsnrate = _fission_rate[qp];

    const Real gas_gen_rate_old = old_fsnrate * yield;
    const Real av_gas_gen_rate = ( gas_gen_rate + gas_gen_rate_old ) * 0.5;  // average gas generation rate in the time step [mol/(m**3*s)]
    const Real dgas_gen = av_gas_gen_rate * _dt;

    _gas_gen[qp] = _gas_gen_old[qp] + dgas_gen;

    if ( _testing )
      std::cout << "qp: " << qp << " gas_gen: " << _gas_gen[qp] << " dgas_gen: " << dgas_gen << " old: " << _gas_gen_old[qp] << std::endl;

    if ( _gas_gen[qp] < 0. )
      _gas_gen[qp] = 0.;

    // compute release rate
    // The following model is taken from Preusser, JNM vol 57 (1982)

    const Real cen_temp_cel = _center_temp - 273;
    Real burnup_threshold( 2.0 );
    if ( cen_temp_cel > 2325 )
      burnup_threshold = 0;
    else if ( cen_temp_cel > 1455 )
      burnup_threshold = -0.0023 * cen_temp_cel + 5.3504;

    const Real bu_percent = _burnup[qp] * 100;

    Real corr_burnup = ( 1 - std::exp( -1.5 * ( bu_percent - burnup_threshold ) ) );
    if ( corr_burnup < 0)
      corr_burnup = 0;

    Real gas_rel_rate(0.0);
    if ( cen_temp_cel < 1000 )
      gas_rel_rate = 0.0;
    else if ( cen_temp_cel > 2070 )
      gas_rel_rate = 0.741918 * std::log( 0.7675 * cen_temp_cel ) - 4.968477;
    else
      gas_rel_rate = 0.000467 * cen_temp_cel - 0.467;

    bool add;
    Real avg_ratio = _gas_rel_old[qp] / _gas_gen_old[qp];
    if ( _gas_gen_old[qp] == 0 )
      avg_ratio = 0;
    if ( gas_rel_rate > avg_ratio )
    {
      _gas_rel[qp] = gas_rel_rate * corr_burnup * _gas_gen[qp];
      add = false;
    }
    else
    {
      _gas_rel[qp] = _gas_rel_old[qp] + gas_rel_rate * dgas_gen;
      add = true;
    }
    if ( _testing )
    {
      std::cout << " burnup_thres: " << burnup_threshold << " bu_corr: " << corr_burnup<< " cen_temp_cel: " << cen_temp_cel << std::endl;
      std::cout <<  "burnup: " << bu_percent << " gas_rel_rate: " << gas_rel_rate << " rel/gen "  << avg_ratio << " add: " << add << std::endl;
    }

    if ( _gas_rel[qp] < _gas_rel_old[qp])
      mooseError("Fission gas is being lost: _gas_rel[qp] < _gas_rel_old[qp]");
  }
}
