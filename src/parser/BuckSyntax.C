#include "BuckSyntax.h"
#include "ActionFactory.h"

// Actions
#include "ClustersVarsAction.h"
#include "ClustersTimeKernelAction.h"

#include "NucleationKernelsAction.h"
#include "GrowthKernelsAction.h"
#include "ClustersPPAction.h"
#include "KnockoutKernelsAction.h"

#include "ClustersBoundsAction.h"
#include "ClustersBoundsVarsAction.h"

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

  syntax.registerActionSyntax("ClustersVarsAction", "Clusters", "add_variable");
  registerAction(ClustersVarsAction, "add_variable");

  syntax.registerActionSyntax("ClustersTimeKernelAction", "Clusters", "add_kernel");
  registerAction(ClustersTimeKernelAction, "add_kernel");

  syntax.registerActionSyntax("NucleationKernelsAction", "Clusters/Nucleation/", "add_kernel");
  registerAction(NucleationKernelsAction, "add_kernel");

  syntax.registerActionSyntax("GrowthKernelsAction", "Clusters/Growth/", "add_kernel");
  registerAction(GrowthKernelsAction, "add_kernel");

  syntax.registerActionSyntax("ClustersPPAction", "Clusters/PPs/", "add_postprocessor");
  registerAction(ClustersPPAction, "add_postprocessor");

  syntax.registerActionSyntax("AddBoundsVectorsAction", "Clusters/Bounds/");
  syntax.registerActionSyntax("ClustersBoundsAction", "Clusters/Bounds/", "add_aux_kernel");
  registerAction(ClustersBoundsAction, "add_aux_kernel");

  syntax.registerActionSyntax("ClustersBoundsVarsAction", "Clusters/Bounds/", "add_aux_variable");
  registerAction(ClustersBoundsVarsAction, "add_aux_variable");

  syntax.registerActionSyntax("KnockoutKernelsAction", "Clusters/Knockout/", "add_kernel");
  registerAction(KnockoutKernelsAction, "add_kernel");

  ////////////////////////////////////////////////////////////////////////////////////////////
}

}
