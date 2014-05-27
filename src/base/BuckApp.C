#include "BuckApp.h"
#include "Moose.h"
#include "AppFactory.h"

template<>
InputParameters validParams<BuckApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

BuckApp::BuckApp(const std::string & name, InputParameters parameters) :
    MooseApp(name, parameters)
{
  srand(processor_id());

  Moose::registerObjects(_factory);
  BuckApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  BuckApp::associateSyntax(_syntax, _action_factory);
}

BuckApp::~BuckApp()
{
}

void
BuckApp::registerApps()
{
  registerApp(BuckApp);
}

void
BuckApp::registerObjects(Factory & factory)
{
}

void
BuckApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
}
