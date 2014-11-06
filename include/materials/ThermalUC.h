#ifndef THERMALUC_H
#define THERMALUC_H

#include "Material.h"


//Forward Declarations
class ThermalUC;

template<>
InputParameters validParams<ThermalUC>();

/**
 * Temperature and burnup dependent thermal properties of uranium carbide
 */
class ThermalUC : public Material
{
public:
  ThermalUC(const std::string & name,
             InputParameters parameters);

protected:
  virtual void computeProperties();
  virtual void initQpStatefulProperties();

  virtual Real computeSpecificHeat(const Real temp);
  virtual void computeUnirradThCond(const Real temp, Real & cond0, Real & cond0_dt);
  virtual Real computePor(const Real porosity);

  VariableValue  & _temp;
  VariableGradient & _grad_temp;
  VariableValue  & _burnup;
  VariableValue & _porosity;

  MaterialProperty<Real> & _thermal_conductivity;
  MaterialProperty<Real> & _thermal_conductivity_dT;
  MaterialProperty<Real> & _specific_heat;

};

#endif //THERMALUC_H
