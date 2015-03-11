#include "BuckSyntax.h"
#include "ActionFactory.h"

// Actions
#include "BubblesConcVarsAction.h"
#include "BubblesConcTimeKernelAction.h"

#include "BubblesConcMomentVarsAction.h"
#include "BubblesConcMomentTimeKernelAction.h"

#include "BubblesRadAuxVarsAction.h"
#include "BubblesRadAuxKernelAction.h"

#include "BubblesPPAction.h"

#include "CoalescenceKernelsAction.h"

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

    syntax.registerActionSyntax("BubblesConcMomentVarsAction", "Bubbles/Moment/", "add_variable");
    syntax.registerActionSyntax("BubblesConcMomentTimeKernelAction", "Bubbles/Moment/", "add_kernel");
    registerAction(BubblesConcMomentVarsAction, "add_variable");
    registerAction(BubblesConcMomentTimeKernelAction, "add_kernel");

    syntax.registerActionSyntax("BubblesPPAction", "Bubbles/PPs/", "add_postprocessor");
    registerAction(BubblesPPAction, "add_postprocessor");

    syntax.registerActionSyntax("CoalescenceKernelsAction", "Bubbles/Coalescence/", "add_kernel");
    registerAction(CoalescenceKernelsAction, "add_kernel");

    syntax.registerActionSyntax("BubblesRadAuxVarsAction", "Bubbles/Rad/", "add_aux_variable");
    syntax.registerActionSyntax("BubblesRadAuxKernelAction", "Bubbles/Rad/Eq/", "add_aux_kernel");
    registerAction(BubblesRadAuxVarsAction, "add_aux_variable");
    registerAction(BubblesRadAuxKernelAction, "add_aux_kernel");
  }

} //end namespace
