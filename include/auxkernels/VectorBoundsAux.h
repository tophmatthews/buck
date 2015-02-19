#ifndef VECTORBOUNDSAUX_H
#define VECTORBOUNDSAUX_H

#include "AuxKernel.h"


//Forward Declarations
class VectorBoundsAux;

template<>
InputParameters validParams<VectorBoundsAux>();

/**
 * Fills in the "bounds vectors" to provide an upper and lower bound for the variable that is coupled in.
 * Doesn't actually calculate an auxiliary value although it must take an auxiliary variable as input.
 *
 * This MUST be run on a Nodal Auxiliary Variable!
 */
class VectorBoundsAux : public AuxKernel
{
public:

  /**
   * Factory constructor.
   */
  VectorBoundsAux(const std::string & name, InputParameters parameters);

protected:
  /**
   * Will store both the upper and lower bound into their respective vectors.
   */
  virtual Real computeValue();

  NumericVector<Number> & _upper_vector;
  NumericVector<Number> & _lower_vector;
  std::vector<VariableName> _bounded_variable_id;
  bool _upper_valid;
  bool _lower_valid;
  Real _upper;
  Real _lower;

  int _N;

  std::vector<unsigned int> _vars;
};

#endif //VECTORBOUNDSAUX_H
