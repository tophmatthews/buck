#ifndef HOMNUCLEATIONMATERIAL_H
#define HOMNUCLEATIONMATERIAL_H

#include "Material.h"

class HomNucleationMaterial;

template<>
InputParameters validParams<HomNucleationMaterial>();

class HomNucleationMaterial : public Material
{
public:
  HomNucleationMaterial(const std::string & name,
                         InputParameters parameters);

protected:
  virtual void computeProperties();
  virtual void initialize();

  VariableValue  & _temp;

  const Real _omega;
  const Real _a;
  const Real _D0;
  const Real _Q;
  const Real _k;

  std::vector<Real> _diffusivity_multipliers;

  std::vector<Real> _c1_rx_coeffs;
  std::vector<Real> _c2_rx_coeffs;
  std::vector<Real> _c3_rx_coeffs;
  std::vector<Real> _c4_rx_coeffs;
  std::vector<Real> _c5_rx_coeffs;
  std::vector<Real> _c6_rx_coeffs;
  std::vector<Real> _c7_rx_coeffs;
  std::vector<Real> _c8_rx_coeffs;
  std::vector<Real> _c9_rx_coeffs;

  std::vector<std::vector<Real> > _rx_coeffs;

  MaterialProperty<std::vector<Real> > & _diffusivities;
  MaterialProperty<std::vector<std::vector<Real> > > & _rx_rates;

  bool _initialized;
  int _N;
};

#endif //HOMNUCLEATIONMATERIAL_H
