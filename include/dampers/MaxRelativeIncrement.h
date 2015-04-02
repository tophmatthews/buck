#ifndef MAXRELATIVEINCREMENT_H
#define MAXRELATIVEINCREMENT_H

#include "Damper.h"

class MaxRelativeIncrement;

template<>
InputParameters validParams<MaxRelativeIncrement>();

class MaxRelativeIncrement : public Damper
{
public:
  MaxRelativeIncrement(std::string name, InputParameters parameters);

  virtual ~MaxRelativeIncrement();

protected:
  virtual Real computeQpDamping();

  Real _max_relative;
};

#endif //MAXRELATIVEINCREMENT_H
