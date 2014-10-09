#ifndef BUBBLEATOMICDENSITYAUXUC_H
#define BUBBLEATOMICDENSITYAUXUC_H

#include "AuxKernel.h"
#include "BubbleAtomicDensityUO.h"

/**
 * Coupled auxiliary value
 */
class BubbleAtomicDensityAuxUC : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  BubbleAtomicDensityAuxUC(const std::string & name, InputParameters parameters);

  virtual ~BubbleAtomicDensityAuxUC() {}

protected:
  virtual Real computeValue();

private:
  const Real _radius;
  const BubbleAtomicDensityUO & _arho_UO;

  bool _has_temp;
  VariableValue & _temp;
};

template<>
InputParameters validParams<BubbleAtomicDensityAuxUC>();

#endif //BUBBLEATOMICDENSITYUC_H
