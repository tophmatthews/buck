#ifndef SUMOFPOSTPROCESSORS_H
#define SUMOFPOSTPROCESSORS_H

#include "GeneralPostprocessor.h"

//Forward Declarations
class SumOfPostprocessors;

template<>
InputParameters validParams<SumOfPostprocessors>();

class SumOfPostprocessors :
  public GeneralPostprocessor
{
public:

  SumOfPostprocessors(const std::string & name, InputParameters parameters);

  virtual void initialize() {};
  virtual void execute() {};
  virtual void threadJoin(const UserObject &) {};
  virtual Real getValue();

protected:
  std::vector<const PostprocessorValue *> _postprocessor_values;
};

#endif
