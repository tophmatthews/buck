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
  void setRonchiData( Real temp , std::vector<Real>& rad, std::vector<Real>& arho) const;
  Real calcRonchi( Real temp, Real radius) const;
  Real calcAtomicDensity( Real temp, Real radius ) const;


protected:

  MooseEnum _model;
  Real _gamma;

  //bool & _initialized;

};

template<>
InputParameters validParams<BubbleAtomicDensityUO>();

#endif //BUBBLEATOMICDENSITYUO_H
