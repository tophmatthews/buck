#ifndef POSITIVEDAMPER_H
#define POSITIVEDAMPER_H

#include "Damper.h"

class PositiveDamper : public Damper
{
public:
  PositiveDamper(std::string name, InputParameters parameters);
  virtual ~PositiveDamper() {}

protected:
  virtual Real computeQpDamping();

  Real _damping;
};

template<>
InputParameters validParams<PositiveDamper>();

#endif //POSITIVEDAMPER_H
