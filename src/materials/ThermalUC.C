#include "ThermalUC.h"

template<>
InputParameters validParams<ThermalUC>()
{
  InputParameters params = validParams<Material>();

  params.addCoupledVar("temp", "Coupled Temperature");
  params.addCoupledVar("burnup", "Coupled Burnup Rate");
  params.addCoupledVar("porosity", "Coupled Porosity");

  params.addParam<Real>("initial_porosity", 0.0, "The initial porosity");
  return params;
}

ThermalUC::ThermalUC(const std::string & name,
                       InputParameters parameters) :
  Material(name, parameters),

  _has_temp(isCoupled("temp")),
  _temp(_has_temp ? coupledValue("temp") : _zero),
  _grad_temp(_has_temp ? coupledGradient("temp") : _grad_zero),

  _has_burnup(isCoupled("burnup")),
  _burnup(_has_burnup ? coupledValue("burnup") : _zero),

  _has_porosity(isCoupled("porosity")),
  _porosity(_has_porosity ? coupledValue("porosity") : _zero),       
        
  _thermal_conductivity(declareProperty<Real>("thermal_conductivity")),
  _thermal_conductivity_dT(declareProperty<Real>("thermal_conductivity_dT")),
  _specific_heat(declareProperty<Real>("specific_heat")),

  _initial_porosity(getParam<Real>("initial_porosity"))
{}

Real
ThermalUC::computeSpecificHeat(const Real temp)
{
  // Ref: T. Preusser Nuclear Technology 57 1982 pg. 352
  // valid for 298 to 2780
    
  Real a =  50.98;
  Real b =  2.572e-2;
  Real c = -1.868e-5;
  Real d =  5.716e-9;
  Real e = -6.187e+5;
  
  Real T = temp;
  Real T2 = T * T;
  Real T3 = T2 * T;

  Real cpm = a + b*T + c*T2 + d*T3 + e/T2; // [J/mol/K]

  // Specific heat with molecular weight conversion
  return cpm/0.25; // [J/kg/K]
}


void
ThermalUC::computeUnirradThCond(const Real temp,
                                 Real & cond0,
                                 Real & cond0_dt)
{
    //   Ref: T. Preusser Nuclear Technology 57 1982 pg. 351
    
  Real thc;
  Real a = 20;
  Real b = 1.3e-3;
  if( temp < 773 )
  {
    cond0 = a;
    cond0_dt = 0;
  }
  else
  {
    cond0 = a + b * ( temp - 773 );
    cond0_dt = b;
  }
}

Real
ThermalUC::computePor( const Real porosity )
{
  //Compute correction for porosity. ref: Preusser Nuc Tech 57 1982
  return (1. - porosity) / (1. + porosity);
}

void
ThermalUC::computeProperties()
{
  for(_qp=0; _qp<_qrule->n_points(); ++_qp)
  {
    Real temp1 = _temp[_qp];
    if (temp1 < 0)
    {
      if (n_processors()>1)
      {
        std::stringstream msg;
        msg << "WARNING:  In ThermalUO2:  negative temperature!\n"
            << "\tResetting to zero.\n"
            << "\t_qp: " << _qp << "\n"
            << "\ttemp: " << _temp[_qp] << "\n"
            << "\telem: " << _current_elem->id() << "\n"
            << "\tproc: " << processor_id() << "\n";
        mooseWarning( msg.str() );
        temp1 = 0;
      }
      else
      {
        std::stringstream errorMsg;
        errorMsg << "\nThermalUO2: Negative temperature in element: "<<_current_elem->id();
        std::cout<<errorMsg.str()<<std::endl<<std::endl;
        throw MooseException();
      }
    }

    //Calculate unirradiated thermal conductivity
    Real cond0, cond0_dT;
    computeUnirradThCond(temp1, cond0, cond0_dT);

    // Correction for porosity
    Real f_por = computePor( _porosity[_qp] );

    // std::cerr << "cond0 fd fp fb fr " << cond0 << "  " << fd << "  " << fp << "  "
    //          << fb << "  " << fr  <<std::endl;

    // Conductivity of irradiated UC: W/(m K)
    _thermal_conductivity[_qp] = cond0 * f_por;
    _thermal_conductivity_dT[_qp] = cond0_dT * f_por;

    // Specific heat: J/kg/K
    _specific_heat[_qp] = computeSpecificHeat(temp1);

    //   std::cerr << "k rho c " << _thermal_conductivity[_qp] << "  "
    //          << _specific_heat[_qp] <<std::endl;

  }
}

void
ThermalUC::initQpStatefulProperties()
{
  _thermal_conductivity[_qp] = 0;
  _thermal_conductivity_dT[_qp] = 0;
  _specific_heat[_qp] = 0;
}
