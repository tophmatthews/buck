#ifndef BUCKAPP_H
#define BUCKAPP_H

#include "MooseApp.h"

class BuckApp;

template<>
InputParameters validParams<BuckApp>();

class BuckApp : public MooseApp
{
public:
  BuckApp(InputParameters parameters);
  virtual ~BuckApp();

  virtual std::string header() const;

  static void registerApps();
  static void registerObjects(Factory & factory);
  // static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);

};

#endif /* BUCKAPP_H */
