#ifndef BUBBLEATOMICDENSITYUO_H
#define BUBBLEATOMICDENSITYUO_H

#include "GeneralUserObject.h"

class BubbleAtomicDensityUO : public GeneralUserObject
{
public:

  BubbleAtomicDensityUO(const std::string & name, InputParameters parameters);

  virtual ~BubbleAtomicDensityUO(){}

  virtual void initialSetup() {}

  virtual void residualSetup() {}

  virtual void timestepSetup() {}

  virtual void execute() {}

  virtual void initialize() {}
  virtual void finalize() {}

  Real calcVDW( Real temp, Real radius) const;
  Real calcRonchi( Real temp, Real radius) const;
  Real calcAtomicDensity( Real temp, Real radius ) const;


protected:

  MooseEnum _model;
  Real _gamma;

  std::vector<Real> _volume;
  std::vector<Real> _radius;

  void getData( Real temp );
  //bool & _initialized;

};

template<>
InputParameters validParams<BubbleAtomicDensityUO>();

#endif //BUBBLEATOMICDENSITYUO_H
