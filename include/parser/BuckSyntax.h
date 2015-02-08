#ifndef BUCKSYNTAX_H
#define BUCKSYNTAX_H

#include "Factory.h"
#include "Syntax.h"
#include "ActionFactory.h"

namespace Buck
{
  void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
}

#endif // BUCKSYNTAX_H
