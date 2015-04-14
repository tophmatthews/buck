#include "BuckSyntax.h"
#include "ActionFactory.h"

// Actions
#include "BubblesConcVarsAction.h"
#include "BubblesConcTimeKernelAction.h"

#include "BubblesRadAuxVarsAction.h"
#include "BubblesRadAuxKernelAction.h"

#include "BubblesPostprocessorsAction.h"

#include "BubblesCoalescenceKernelsAction.h"
#include "BubblesGrowthKernelsAction.h"
#include "BubblesNucleationKernelsAction.h"
#include "BubblesKnockoutKernelsAction.h"

#include "BubblesBoundsAction.h"

#include "BubblesDampersAction.h"

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
    syntax.registerActionSyntax("BubblesConcTimeKernelAction", "Bubbles/Conc/", "add_kernel");
    registerAction(BubblesConcVarsAction, "add_variable");
    registerAction(BubblesConcVarsAction, "add_ic");
    registerAction(BubblesConcTimeKernelAction, "add_kernel");

    syntax.registerActionSyntax("BubblesPostprocessorsAction", "Bubbles/PPs/", "add_postprocessor");
    registerAction(BubblesPostprocessorsAction, "add_postprocessor");

    syntax.registerActionSyntax("BubblesGrowthKernelsAction", "Bubbles/Growth/", "add_kernel");
    registerAction(BubblesGrowthKernelsAction, "add_kernel");

    syntax.registerActionSyntax("BubblesKnockoutKernelsAction", "Bubbles/Knockout/", "add_kernel");
    registerAction(BubblesKnockoutKernelsAction, "add_kernel");

    syntax.registerActionSyntax("BubblesCoalescenceKernelsAction", "Bubbles/Coalescence/", "add_kernel");
    registerAction(BubblesCoalescenceKernelsAction, "add_kernel");

    syntax.registerActionSyntax("BubblesNucleationKernelsAction", "Bubbles/Nucleation/", "add_kernel");
    registerAction(BubblesNucleationKernelsAction, "add_kernel");

    syntax.registerActionSyntax("BubblesDampersAction", "Bubbles/Dampers/", "add_damper");
    registerAction(BubblesDampersAction, "add_damper");

    syntax.registerActionSyntax("BubblesRadAuxVarsAction", "Bubbles/Rad/", "add_aux_variable");
    syntax.registerActionSyntax("BubblesRadAuxKernelAction", "Bubbles/Rad/Eq/", "add_aux_kernel");
    registerAction(BubblesRadAuxVarsAction, "add_aux_variable");
    registerAction(BubblesRadAuxKernelAction, "add_aux_kernel");

    syntax.registerActionSyntax("AddBoundsVectorsAction", "Bubbles/Bounds/");
    syntax.registerActionSyntax("BubblesBoundsAction", "Bubbles/Bounds/", "add_aux_kernel");
    syntax.registerActionSyntax("BubblesBoundsAction", "Bubbles/Bounds/", "add_aux_variable");
    registerAction(BubblesBoundsAction, "add_aux_kernel");
    registerAction(BubblesBoundsAction, "add_aux_variable");
  }

} //end namespace
