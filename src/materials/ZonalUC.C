#include "ZonalUC.h"
#include "MooseEnum.h"
#include "math.h"

template<>
InputParameters validParams<ZonalUC>()
{
  InputParameters params = validParams<Material>();
  
  params.addParam<Real>("frac_yield", 0.25, "Fraction of fissions that result in a fission gas atom");
  params.addParam<Real>("nitrogen_fraction", 0, "Stochiometric fraction of Nitrogen");
  params.addParam<bool>("high_oxygen", true, "Sets high oxygen fuel content:true or false");
  params.addParam<Real>("zone3_width", 100, "Sets temperautre width of Zone 3");
  params.addParam<Real>("frac_rel_zone1", 0.70, "Sets fission gas release fraction from Zone 1");
  params.addParam<Real>("frac_rel_zone3", 0.15, "Sets fission gas release fraction from Zone 3");
  params.addParam<Real>("frac_rel_zone4", 0.10, "Sets fission gas release fraction from Zone 4");
  params.addParam<bool>("testing", false, "Turns on/off testing output");

  params.addCoupledVar("temp", 0, "Coupled Temperature");
  params.addCoupledVar("fission_rate",0, "Coupled Fission Rate");
  params.addCoupledVar("burnup", 0, "Coupled Burnup");
  
  return params;
}

ZonalUC::ZonalUC(const std::string & name, InputParameters parameters) :
  Material(name, parameters),
        
  _frac_yield(getParam<Real>("frac_yield")),
  _nitrogen_fraction(getParam<Real>("nitrogen_fraction")),
  _oxygen_level(getParam<bool>("high_oxygen")),
  _zone3_width(getParam<Real>("zone3_width")),
  _frac_rel_zone1(getParam<Real>("frac_rel_zone1")),
  _frac_rel_zone3(getParam<Real>("frac_rel_zone3")),
  _frac_rel_zone4(getParam<Real>("frac_rel_zone4")),

  _fission_rate(coupledValue("fission_rate")),  // fission rate [1/(m**3*s)]
  _fission_rate_old(coupledValueOld("fission_rate")),

  _temp(coupledValue("temp")),  // temperature [K]
  _temp_old(coupledValueOld("temp")),
 
  _burnup(coupledValue("burnup")),  // local burnup [FIMA]
  _burnup_old(coupledValueOld("burnup")),

  _T2(declareProperty<Real>("T2")),

  _zone(declareProperty<Real>("zone")),
  _gas_gen(declareProperty<Real>("gas_gen")),  // volume concentration of gas produced [mol/m**3]
  _gas_rel(declareProperty<Real>("gas_rel")),   // volume concentration of gas released [mol/m**3]

  _zone_old(declarePropertyOld<Real>("zone")),
  _gas_gen_old(declarePropertyOld<Real>("gas_gen")),
  _gas_rel_old(declarePropertyOld<Real>("gas_rel")),

  // constants
  _R(8.314), // Ideal Gas constant [J/mol/K]
  _avogadros_num(6.022e+23),

  _testing(getParam<bool>("testing"))
{
  if ( !isCoupled("fission_rate") )
    mooseError("From ZonalUC: coupled fission_rate needed");

  if ( !isCoupled("temp") )
    mooseError("From ZonalUC: coupled temp needed");

  if ( !isCoupled("burnup") )
    mooseError("From ZonalUC: coupled Burnup needed");
}

void
ZonalUC::calcT2(unsigned int qp)
{
  // Calculation of the critical temperature T2*
  // Equation goes by T2 = a / ( R*ln(F/b))
  // T = temperature [K]
  // R = Ideal gas constant 8.314 [k/mol/K]
  // F = Burnup [FIMA]
  // a,b = constants

  Real a;
  Real b;
  Real F = _burnup[qp] * 100.0; // [FIMA] to [a/o]

  Real n = _nitrogen_fraction;
  Real n2 = n * n;
  Real n3 = n * n2;

  a = -514 * n + 1.45e5;
  b = 1.36e-9 * n3 - 4.29e-8 * n2 + 7.15e-7 * n + 2.00e-6;

  _T2[qp] = a / _R / log( F / b );

  if ( _testing )
    std::cout << "Temp: " << _temp[qp] << " burnup: " << F << " a: " << a << " b: " << b << " T2: " << _T2[qp] << std::endl;
}

void
ZonalUC::initQpStatefulProperties()
{
    _gas_gen[_qp] = 0.;
    _gas_rel[_qp] = 0.;
    _zone[_qp] = 4;
    _T2[_qp] = 0;

    _gas_gen_old[_qp] = 0.;
    _gas_rel_old[_qp] = 0.;
    _zone_old[_qp] = 4;
}

void
ZonalUC::computeProperties()
{
  if( _t_step == 0 || _t <= 0. )
  {
    for (unsigned int qp=0; qp<_qrule->n_points(); ++qp)
    {
      _gas_gen[qp] = 0.;
      _gas_rel[qp] = 0.;
      _zone[qp] = 4.;
      _T2[qp] = 1.0e4;

      _gas_gen_old[qp] = 0.;
      _gas_rel_old[qp] = 0.;
      _zone_old[qp] = 4;

      _fission_rate_old[qp] = _fission_rate[qp];
    }
    return;
  }

  for(unsigned int qp=0; qp<_qrule->n_points(); ++qp)
  {
    if (_temp[qp] < 0.0)
    {
      if (n_processors()>1)
      {
        std::stringstream errorMsg;
        errorMsg << "ForMas: Negative temperature:"
                 << "\nElement:     " <<_current_elem->id()
                 << "\nqp:          " <<qp
                 << "\nTemperature: " << _temp[qp];
        mooseError(errorMsg.str());
      }
      else
      {
        std::stringstream errorMsg;
        errorMsg << "\nForMas: Negative temperature in element: "<<_current_elem->id();
        Moose::out<<errorMsg.str()<<std::endl<<std::endl;
        throw MooseException();
      }
    }

    // Generate fission gas
    const Real yield = _frac_yield / _avogadros_num;  // yield of fission gas (Xe + Kr) [mol]
    const Real gas_gen_rate     = _fission_rate[qp]     * yield;
    const Real gas_gen_rate_old = _fission_rate_old[qp] * yield;
    const Real av_gas_gen_rate = ( gas_gen_rate + gas_gen_rate_old ) * 0.5;  // average gas generation rate in the time step [mol/(m**3*s)]
    const Real dgas_gen = av_gas_gen_rate * _dt;

    _gas_gen[qp] = _gas_gen_old[qp] + dgas_gen;

    if ( _testing )
      std::cout << "qp: " << qp << " gas_gen: " << _gas_gen[qp] << " dgas_gen: " << dgas_gen << " old: " << _gas_gen_old[qp] << std::endl;


    if ( _gas_gen[qp] < 0. )
    {
      _gas_gen[qp] = 0.;
      std::cout << "BAAAAAADDDDDD" << std::endl;
    }

    if ( _burnup[qp] < 0.001 ) // if burnup is less than threshold, then produce but don't release gas
    {
      _gas_rel[qp] = 0;
    }
    else
    {
      
      calcT2(qp);

      // Ensure zone isn't something it shouldn't be
      if ( _zone[qp] != 4 && _zone[qp] != 3 && _zone[qp] != 1 )
      {
        std::stringstream errorMsg;
        errorMsg << "\nZonalUC: Bad Zone designation: "<<_zone[qp];
        Moose::out<<errorMsg.str()<<std::endl<<std::endl;
        throw MooseException();
      }

      if ( _zone[qp] == 4)
      {
        if ( _temp[qp] > _T2[qp] )
          _zone[qp] = 3;
      }

      if ( _zone[qp] == 3)
      {
        if ( _temp[qp] > _T2[qp] + _zone3_width )
          _zone[qp] = 1;
      }

      Real frac_rel;
      if (_zone[qp] == 4)
        frac_rel = _frac_rel_zone4;
      if (_zone[qp] == 3)
        frac_rel = _frac_rel_zone3;
      if (_zone[qp] == 1)
        frac_rel = _frac_rel_zone1;

      Real frac_rel_old;
      if (_zone_old[qp] == 4)
        frac_rel_old = _frac_rel_zone4;
      if (_zone_old[qp] == 3)
        frac_rel_old = _frac_rel_zone3;
      if (_zone_old[qp] == 1)
        frac_rel_old = _frac_rel_zone1;

      Real frac_rel_avg = ( frac_rel + frac_rel_old ) * 0.5;

      if ( _testing )
        std::cout << "zone: " << _zone[qp] << " frac_rel_avg: " << frac_rel_avg << std::endl;

      _gas_rel[qp] = _gas_rel_old[qp] + dgas_gen * frac_rel_avg;
    }
  }
}
