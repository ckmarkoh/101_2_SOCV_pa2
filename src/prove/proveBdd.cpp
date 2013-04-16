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
 /*   CktModule* module = VLDesign.getFltModule(); assert (module);
    for(unsigned i=0; i<module->getDffSize();i++){
        CktCell* tempdff=module->getSeqCell(i);
        const unsigned width = tempdff->getOutPin()->getBusWidth();
        for(unsigned j=0; j<width ; j++){
//          BddNodeV temp=getBddNodeV(getNSName(getBitName(tempdff->getOutPinName(),j)));
            BddNodeV temp=getBddNodeV(getBitName(tempdff->getOutPinName(),j));
            _initState &= (~temp);
        }   
   }*/   
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
