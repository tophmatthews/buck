#ifndef URFGR_H
#define URFGR_H

#include "Material.h"

//Forward Declarations
class UrFGR;

template<>
InputParameters validParams<UrFGR>();

/**
 * Simple material with constant properties.
 */
class UrFGR : public Material
{
public:
  UrFGR(const std::string & name,
                         InputParameters parameters);

protected:
  virtual void initQpStatefulProperties();
  virtual void computeProperties();

  const Real _frac_yield;

  VariableValue & _fission_rate;
  VariableValue & _burnup;

  VariableValue & _fission_rate_old;  
  VariableValue & _burnup_old;

  MaterialProperty<Real> & _gas_gen;
  MaterialProperty<Real> & _gas_rel;

  MaterialProperty<Real> & _gas_gen_old;
  MaterialProperty<Real> & _gas_rel_old;

  PostprocessorValue & _center_temp;


  const Real _R;
  const Real _avogadros_num;

  bool _testing;
};

#endif //URFGR_H
