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

  const int _N;
  const Real _omega;
  const Real _a;
  const bool _cluster_diffusion;

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

  MaterialProperty<std::vector<std::vector<Real> > > & _rx_rates;
  MaterialProperty<std::vector<Real> > & _cluster_diffusivities;

  MaterialProperty<Real> & _atomic_diffusivity;

  bool _initialized;

  std::vector<std::vector<Real> > _rx_coeffs;
};

#endif //HOMNUCLEATIONMATERIAL_H
