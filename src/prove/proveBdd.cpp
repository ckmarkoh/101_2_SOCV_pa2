/****************************************************************************
  FileName     [ proveBdd.cpp ]
  PackageName  [ prove ]
  Synopsis     [ For BDD-based verification ]
  Author       [ ]
  Copyright    [ Copyleft(c) 2010 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include "v3NtkUtil.h"
#include "v3Msg.h"
#include "bddMgrV.h"

void
BddMgrV::buildPInitialState()
{
   // TODO : remember to set _initState
   // Set Initial State to All Zero
   _initState=BddNodeV::_one;
  V3Ntk* const ntk = v3Handler.getCurHandler()->getNtk();
  for(unsigned i = 0, n = ntk->getLatchSize(); i < n; ++i) {
    const V3NetId& nId = ntk->getLatch(i);
    BddNodeV b=bddMgrV->getBddNodeV(nId.id);
	_initState &=(~b); 
     }

}

void
BddMgrV::buildPTransRelation()
{
   // TODO : remember to set _tr, _tri
}

void
BddMgrV::buildPImage( int level )
{
   // TODO : remember to add _reachStates and set _isFixed
   // Note:: _reachStates record the set of reachable states
}

void 
BddMgrV::runPCheckProperty( const string &name, BddNodeV monitor )
{
   // TODO : prove the correctness of AG(~monitor)
}
