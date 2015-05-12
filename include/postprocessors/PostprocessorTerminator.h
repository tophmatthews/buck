#ifndef POSTPROCESSORTERMINATOR_H
#define POSTPROCESSORTERMINATOR_H

#include "GeneralPostprocessor.h"

class PostprocessorTerminator;

template<>
InputParameters validParams<PostprocessorTerminator>();

class PostprocessorTerminator : public GeneralPostprocessor
{
public:
  PostprocessorTerminator(const std::string & name, InputParameters parameters);
  virtual void initialize() {};
  virtual void execute();
  virtual Real getValue() { return 0; }
  virtual void threadJoin(const UserObject &) {};

protected:
	const Real _threshold;
  const PostprocessorValue & _value;
};

#endif
