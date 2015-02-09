#include "BuckSyntax.h"
#include "ActionFactory.h"

// Actions
#include "NucleationKernelsAction.h"
#include "NucleationVarsAction.h"

#include "GrowthKernelsAction.h"
#include "GrowthVarsAction.h"


namespace Buck
{

void
associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  /**
   * Note: the optional third parameter is used to differentiate which action_name is
   * satisfied based on the syntax encountered for classes which are registered
   * to satisfy more than one action_name
   */
  syntax.registerActionSyntax("NucleationKernelsAction", "Nucleation/*", "add_kernel");
  syntax.registerActionSyntax("NucleationVarsAction",    "Nucleation/*", "add_variable");

  registerAction(NucleationKernelsAction, "add_kernel");
  registerAction(NucleationVarsAction,    "add_variable");

  syntax.registerActionSyntax("GrowthKernelsAction", "Growth/*", "add_kernel");
  syntax.registerActionSyntax("GrowthVarsAction",    "Growth/*", "add_variable");

  registerAction(GrowthKernelsAction, "add_kernel");
  registerAction(GrowthVarsAction,    "add_variable");
}

}
