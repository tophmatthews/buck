#ifndef BUBBLESUC_H
#define BUBBLESUC_H

#include "Material.h"

//Forward Declarations
class BubblesUC;

template<>
InputParameters validParams<BubblesUC>();

/**
 * Simple material with constant properties.
 */
class BubblesUC : public Material
{
public:
  BubblesUC(const std::string & name,
                         InputParameters parameters);

protected:
  virtual void initQpStatefulProperties();
  virtual void computeProperties();

  const Real _initial_P0_porosity; // Fabriction porosity
  const Real _initial_P1_porosity; // small fission gas bubbles 1-30 nm
  const Real _initial_P2_porosity; // large fission gas bubbbles 30-500 nm
  const Real _initial_P3_porosity; // grain face bubbles

  MaterialProperty<Real> & _P0_porosity;
  MaterialProperty<Real> & _P1_porosity;
  MaterialProperty<Real> & _P2_porosity;
  MaterialProperty<Real> & _P3_porosity;
  
};

#endif //BUBBLESUC_H
