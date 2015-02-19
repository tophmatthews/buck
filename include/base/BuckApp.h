#ifndef BUCKAPP_H
#define BUCKAPP_H

#include "MooseApp.h"

class BuckApp;

template<>
InputParameters validParams<BuckApp>();

class BuckApp : public MooseApp
{
public:
  BuckApp(const std::string & name, InputParameters parameters);
  virtual ~BuckApp();

  virtual void runInputFile();

  static void registerApps();
  static void registerObjects(Factory & factory);
  // static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);

protected:
  void printHeader();

};

#endif /* BUCKAPP_H */
