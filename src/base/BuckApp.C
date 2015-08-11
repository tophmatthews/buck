#include "BuckApp.h"
#include "Moose.h"
#include "AppFactory.h"

// Buck
#include "BuckSyntax.h"
#include "BuckRevision.h"

// AuxKernels
#include "EquilibriumRadiusAux.h"

// dampers
#include "PositiveDamper.h"

// Kernels
#include "VariableScaledSource.h"
#include "BasicDiffusion.h"
#include "BubbleBase.h"
#include "BubbleNucleation.h"
#include "BubbleFFNucleation.h"
#include "BubbleGrowth.h"
#include "BubbleKnockout.h"

// Materials
#include "GasAtomDiffusivity.h"

// Postprocessors
#include "GrainBoundaryGasFlux.h"
#include "SumOfPostprocessors.h"
#include "MaterialXeBubbleTester.h"
#include "BoundedElementAverage.h"
#include "SwellingPostprocessor.h"
#include "PostprocessorTerminator.h"
#include "C1LossPostprocessor.h"
#include "GainRatePostprocessor.h"
#include "KnockoutRatePostprocessor.h"

template<>
InputParameters validParams<BuckApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

BuckApp::BuckApp(InputParameters parameters) :
    MooseApp(parameters)
{
  srand(processor_id());

  Moose::registerObjects(_factory);
  BuckApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  Buck::associateSyntax(_syntax, _action_factory);
}

BuckApp::~BuckApp()
{
}

extern "C" void BuckApp__registerApps() { BuckApp::registerApps(); }
void
BuckApp::registerApps()
{
  registerApp(BuckApp);
}

void
BuckApp::registerObjects(Factory & factory)
{
  registerAux(EquilibriumRadiusAux);

  registerDamper(PositiveDamper);

  registerKernel(VariableScaledSource);
  registerKernel(BasicDiffusion);
  registerKernel(BubbleBase);
  registerKernel(BubbleNucleation);
  registerKernel(BubbleFFNucleation);
  registerKernel(BubbleGrowth);
  registerKernel(BubbleKnockout);

  registerMaterial(GasAtomDiffusivity);

  registerPostprocessor(GrainBoundaryGasFlux);
  registerPostprocessor(SumOfPostprocessors);
  registerPostprocessor(MaterialXeBubbleTester);
  registerPostprocessor(BoundedElementAverage);
  registerPostprocessor(SwellingPostprocessor);
  registerPostprocessor(PostprocessorTerminator);
  registerPostprocessor(C1LossPostprocessor);
  registerPostprocessor(GainRatePostprocessor);
  registerPostprocessor(KnockoutRatePostprocessor);
}

void
BuckApp::runInputFile()
{
  printHeader();
  MooseApp::runInputFile();
}

void
BuckApp::printHeader()
{
  Moose::out  << "\n"
              << "\n"
              << "     ______  _     _  ______ _    _      \n"
              << "    (____  \\| |   | |/ _____) |  / )    \n"
              << "     ____)  | |   | | /     | | / /      \n"
              << "    |  __  (| |   | | |     | |< <       \n"
              << "    | |__)  | |___| | \\_____| | \\ \\   \n"
              << "    |______/ \\______|\\______)_|  \\_)  \n"

              << "\n"
              << "\n"
              << "      Bubble and Cluster Kinetics \n"
              << "        Oregon State University   \n"
              << "             Corvallis, OR        \n"
              << "\n"
              << "\n";

  Moose::out << "Input file:   " << _input_filename << "\n"
             << "Input units:  micrometer, gram, second, kelvin, mole\n"
             << "\n"
             << "BUCK version: " << BUCK_REVISION << std::endl
             << std::endl;
}
