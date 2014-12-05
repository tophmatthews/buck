#ifndef CREEPUC_H
#define CREEPUC_H

#include "SolidModel.h"

// Forward declarations
class BurnupFunction;

/**
 * Combined steady-state thermal and irradiation creep for UO2:
 *  edot = (A1+A2*Fdot)/((A3+D)*G**2))*(sigma)*exp(-Q1/(RT)) +
 *         (A4/(A6+D))*sigma**4.5*exp(-Q2/(RT)) +
 *         A7*Fdot*exp(-Q3/(RT))
 *   where:
 *     edot = creep strain rate (1/s),
 *     sigma = Mises stress (Pa),
 *     Fdot = volumetric fission rate (fissions/m**3-s),
 *     T = temperature (K),
 *     Q = activation energy (J/mol),
 *     G = grain size (microns)
 *     R = universal gas constant (J/mol-K), and
 *     A1-A7 =  material constants
 */

class CreepUC : public SolidModel
{
public:
  CreepUC( const std::string & name,
            InputParameters params );

protected:

  virtual void createElasticityTensor();
  virtual bool updateElasticityTensor(SymmElasticityTensor & tensor);

  const Real _relative_tolerance;
  const Real _absolute_tolerance;
  const unsigned int _max_its;
  const bool _output_iteration_info;
  const PostprocessorValue * _output;

  MaterialProperty<SymmTensor> & _creep_strain;
  MaterialProperty<SymmTensor> & _creep_strain_old;

  const bool _calc_elastic_modulus;
  const bool _model_creep;

  VariableValue  & _fission_rate;
  const VariableValue & _porosity;

  const Real _a1;
  const Real _a2;
  const Real _q;

  /// Compute the stress (sigma += deltaSigma)
  virtual void computeStress();

private:

};

template<>
InputParameters validParams<CreepUC>();

#endif //CREEPUC_H
