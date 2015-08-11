#ifndef BubblesRadAuxKernelAction_H
#define BubblesRadAuxKernelAction_H

#include "BubblesActionBase.h"

class BubblesRadAuxKernelAction;

template<>
InputParameters validParams<BubblesRadAuxKernelAction>();

class BubblesRadAuxKernelAction : public BubblesActionBase
{
public:
  BubblesRadAuxKernelAction(InputParameters params);

  virtual void act();

private:
	std::vector<SubdomainName> _blocks;
};

#endif // BubblesRadAuxKernelAction_H
