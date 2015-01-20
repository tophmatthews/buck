#ifndef HOMNUCLEATIONMATERIAL_H
#define HOMNUCLEATIONMATERIAL_H

#include "Material.h"

//Forward Declarations
class HomNucleationMaterial;

template<>
InputParameters validParams<HomNucleationMaterial>();

/**
 * Simple material with constant properties.
 */
class HomNucleationMaterial : public Material
{
public:
  HomNucleationMaterial(const std::string & name,
                         InputParameters parameters);

protected:
  virtual void computeProperties();
  virtual void initialize();

  MaterialProperty<std::vector<Real> > & _coeffs;

  std::vector<Real> _input;
  int _num;
  bool _initialized;
  
};

#endif //HOMNUCLEATIONMATERIAL_H
