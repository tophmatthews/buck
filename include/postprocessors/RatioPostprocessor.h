#ifndef RATIOPOSTPROCESSOR_H
#define RATIOPOSTPROCESSOR_H

#include "GeneralPostprocessor.h"

class RatioPostprocessor;

template<>
InputParameters validParams<RatioPostprocessor>();

/**
 * Computes the Ratio between two postprocessors
 *
 * result = value1 - value2
 */
class RatioPostprocessor : public GeneralPostprocessor
{
public:
  RatioPostprocessor(const std::string & name, InputParameters parameters);
  virtual ~RatioPostprocessor();

  virtual void initialize();
  virtual void execute();
  virtual PostprocessorValue getValue();
  virtual void threadJoin(const UserObject & uo);

protected:
  const PostprocessorValue & _top;
  const PostprocessorValue & _bottom;
};


#endif /* RATIoPOSTPROCESSOR_H */
