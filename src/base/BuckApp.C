#include "BuckApp.h"
#include "Moose.h"
#include "AppFactory.h"

// Modules
#include "ContactApp.h"
#include "HeatConductionApp.h"
#include "LinearElasticityApp.h"
#include "MiscApp.h"
#include "SolidMechanicsApp.h"

#include "FoxApp.h"
#include "FoxSyntax.h"

#include "BisonApp.h"
#include "BisonSyntax.h"

// Materials

#include "ThermalUC.h"
#include "CreepUC.h"
//#include "DensificationUC.h"
//#include "MechUC.h"
//#include "SifgrsUC.h"
#include "VSwellingUC.h"


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
  
  ContactApp::registerObjects(_factory);
  HeatConductionApp::registerObjects(_factory);
  LinearElasticityApp::registerObjects(_factory);
  MiscApp::registerObjects(_factory);
  SolidMechanicsApp::registerObjects(_factory);
  FoxApp::registerObjects(_factory);
  BisonApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  BuckApp::associateSyntax(_syntax, _action_factory);
  
  SolidMechanicsApp::associateSyntax(_syntax, _action_factory);
  ContactApp::associateSyntax(_syntax, _action_factory);
  HeatConductionApp::associateSyntax(_syntax, _action_factory);
  MiscApp::associateSyntax(_syntax, _action_factory);
  Fox::associateSyntax(_syntax, _action_factory);
  Bison::associateSyntax(_syntax, _action_factory);
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
  registerMaterial(ThermalUC);
  registerMaterial(CreepUC);
  registerMaterial(VSwellingUC);
}

void
BuckApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
}
