#include "BuckApp.h"
#include "MooseInit.h"
#include "Moose.h"
#include "MooseApp.h"
#include "AppFactory.h"

// Create a performance log
PerfLog Moose::perf_log("Buck");

// Begin the main program.
int main(int argc, char *argv[])
{
  // Initialize MPI, solvers and MOOSE
  MooseInit init(argc, argv);

  // Register this application's MooseApp and any it depends on
  BuckApp::registerApps();

  // This creates dynamic memory that we're responsible for deleting
  MooseApp * app = AppFactory::createApp("BuckApp", argc, argv);

  // Terminate if unused parameters in input file
  app->setCheckUnusedFlag(true);

  // Execute the application
  app->run();

  // Free up the memory we created earlier
  delete app;

  return 0;
}
