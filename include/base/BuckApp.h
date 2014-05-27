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

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* BUCKAPP_H */
