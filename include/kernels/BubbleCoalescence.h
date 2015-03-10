#ifndef BUBBLECOALESCENCE_H
#define BUBBLECOALESCENCE_H

#include "BubbleNucleation.h"

class BubbleCoalescence;

template<>
InputParameters validParams<BubbleCoalescence>();

class BubbleCoalescence : public BubbleNucleation
{
public:
  BubbleCoalescence(const std::string & name, InputParameters parameters);

protected:

  virtual Real calcLosses(bool jac);
  virtual Real calcGains(bool jac);

private:

};

#endif //BUBBLECOALESCENCE_H
