#ifndef POSITIVEDAMPER_H
#define POSITIVEDAMPER_H

#include "Damper.h"

class PositiveDamper;

template<>
InputParameters validParams<PositiveDamper>();

class PositiveDamper : public Damper
{
public:
  PositiveDamper(std::string name, InputParameters parameters);

  virtual ~PositiveDamper();

protected:
  virtual Real computeQpDamping();

  Real _damping;
};

#endif //POSITIVEDAMPER_H
