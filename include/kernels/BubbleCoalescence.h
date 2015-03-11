#ifndef BUBBLECOALESCENCE_H
#define BUBBLECOALESCENCE_H

#include "BubbleBase.h"

class BubbleCoalescence;

template<>
InputParameters validParams<BubbleCoalescence>();

class BubbleCoalescence : public BubbleBase
{
public:
  BubbleCoalescence(const std::string & name, InputParameters parameters);

protected:

  virtual void calcLosses(Real & losses, const bool jac);
  virtual void calcGains(Real & gains, const bool jac);

private:

	MaterialProperty<std::vector<Real> > & _Dg;

};

#endif //BUBBLECOALESCENCE_H
