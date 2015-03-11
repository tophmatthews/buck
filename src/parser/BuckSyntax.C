#include "BuckSyntax.h"
#include "ActionFactory.h"

// Actions
#include "BubblesConcVarsAction.h"
#include "BubblesConcMomentVarsAction.h"

#include "BubblesRadAuxVarsAction.h"

#include "BubblesConcTimeKernelAction.h"
#include "BubblesConcMomentTimeKernelAction.h"

#include "BubblesPPAction.h"

#include "CoalescenceKernelsAction.h"

// #include "NucleationKernelsAction.h"


// #include "KnockoutKernelsAction.h"

// #include "ClustersBoundsAction.h"
// #include "ClustersBoundsVarsAction.h"

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

  syntax.registerActionSyntax("BubblesConcVarsAction", "Bubbles/Conc/", "add_variable");
  syntax.registerActionSyntax("BubblesConcVarsAction", "Bubbles/Conc/", "add_ic");
  registerAction(BubblesConcVarsAction, "add_variable");
  registerAction(BubblesConcVarsAction, "add_ic");

  syntax.registerActionSyntax("BubblesConcMomentVarsAction", "Bubbles/Moment/", "add_variable");
  registerAction(BubblesConcMomentVarsAction, "add_variable");

  syntax.registerActionSyntax("BubblesRadAuxVarsAction", "Bubbles/Rad/", "add_aux_variable");
  registerAction(BubblesRadAuxVarsAction, "add_aux_variable");

  syntax.registerActionSyntax("BubblesConcTimeKernelAction", "Bubbles/Conc/", "add_kernel");
  registerAction(BubblesConcTimeKernelAction, "add_kernel");

  syntax.registerActionSyntax("BubblesConcMomentTimeKernelAction", "Bubbles/Moment/", "add_kernel");
  registerAction(BubblesConcMomentTimeKernelAction, "add_kernel");

  syntax.registerActionSyntax("BubblesPPAction", "Bubbles/PPs/", "add_postprocessor");
  registerAction(BubblesPPAction, "add_postprocessor");

  syntax.registerActionSyntax("CoalescenceKernelsAction", "Bubbles/Coalescence/", "add_kernel");
  registerAction(CoalescenceKernelsAction, "add_kernel");

  // syntax.registerActionSyntax("NucleationKernelsAction", "Clusters/Nucleation/", "add_kernel");
  // registerAction(NucleationKernelsAction, "add_kernel");





  // syntax.registerActionSyntax("AddBoundsVectorsAction", "Clusters/Bounds/");
  // syntax.registerActionSyntax("ClustersBoundsAction", "Clusters/Bounds/", "add_aux_kernel");
  // registerAction(ClustersBoundsAction, "add_aux_kernel");

  // syntax.registerActionSyntax("ClustersBoundsVarsAction", "Clusters/Bounds/", "add_aux_variable");
  // registerAction(ClustersBoundsVarsAction, "add_aux_variable");

  // syntax.registerActionSyntax("KnockoutKernelsAction", "Clusters/Knockout/", "add_kernel");
  // registerAction(KnockoutKernelsAction, "add_kernel");

  ////////////////////////////////////////////////////////////////////////////////////////////
}

}
