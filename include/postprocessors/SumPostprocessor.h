#ifndef SUMPOSTPROCESSOR_H
#define SUMPOSTPROCESSOR_H

#include "DifferencePostprocessor.h"

class SumPostprocessor;

template<>
InputParameters validParams<SumPostprocessor>();


class SumPostprocessor : public DifferencePostprocessor
{
public:
  SumPostprocessor(const std::string & name, InputParameters parameters);
  virtual PostprocessorValue getValue();
};

#endif /* SUMPOSTPROCESSOR_H */
