#ifndef CREEPUC
#define CREEPUC

#include "SolidModel.h"

// Forward declarations
class CreepUC;

template<>
InputParameters validParams<CreepUC>();

/*
 * Combined steady-state thermal and irradiation creep for UC:
 *  edot = A1*Fdot*sigma + A2*sigma^2.44*exp(-Q/T)
 *
 *   where:
 *     edot  = creep strain rate (1/s),
 *     sigma = Mises stress (Pa),
 *     Fdot  = volumetric fission rate (fissions/m**3-s),
 *     T     = temperature (K),
 *     Q     = activation energy (J/mol),
 *     A1&A2 = material constants
 * 
 *  Ref: T. Preusser Nuclear Technology 57 1982
 */

class CreepUC : public SolidModel
{
public:
  CreepUC( const std::string & name,
            InputParameters params );

protected:

  const Real _relative_tolerance;
  const Real _absolute_tolerance;
  const unsigned int _max_its;
  const bool _output_iteration_info;
  const PostprocessorValue * _output;

  VariableValue  & _fission_rate;

  MaterialProperty<SymmTensor> & _creep_strain;
  MaterialProperty<SymmTensor> & _creep_strain_old;

  const Real _a1;
  const Real _a2;

  const Real _q;

  /// Compute the stress (sigma += deltaSigma)
  virtual void computeStress();
};

#endif //CREEPUC
