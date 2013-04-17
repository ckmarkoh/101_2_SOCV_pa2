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

/*int totallevel=0;
unsigned inbddsize=0;
unsigned dffbddsize=0;
*/


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
/*    for(unsigned j=0 ; j<module->getIoSize(CKT_IN);j++){
            CktCell* tempio=module->getIoCell(CKT_IN,j);
            const unsigned width = tempio->getOutPin()->getBusWidth();
            for(unsigned k=0; k<width;k++){
                inbddsize++;
            }
        }
        for(unsigned j=0 ; j<module->getDffSize();j++){
            CktCell* tempdff=module->getSeqCell(j);
            const unsigned width = tempdff->getOutPin()->getBusWidth();
            for(unsigned k=0; k<width;k++){
                dffbddsize++;
            }
        }
    totallevel=inbddsize+dffbddsize*2;
*/
	/*
	_initState.exist(18);
	cout<<_initState<<endl;

	_initState.exist(17);
	cout<<_initState<<endl;

	_initState.exist(13);
	cout<<_initState<<endl;

	_initState.exist(1);
	cout<<_initState<<endl;
	*/
}

void
BddMgrV::buildPTransRelation()
{
   // TODO : remember to set _tr, _tri
   // NOTE : _tri is the BDD with PI, Current State, and Next State Variables
   //        _tr  is the BDD with Current and Next State Variables
   //       (PI Variables are Existential Quantified from _tri)
 
	_tri=BddNodeV::_one;


	V3NtkHandler* const ntkhandler=v3Handler.getCurHandler();
	V3Ntk* const ntk = v3Handler.getCurHandler()->getNtk();
	for(unsigned i = 0, n = ntk->getLatchSize(); i < n; i++) {
		const V3NetId& nId = ntk->getLatch(i);
		BddNodeV b=bddMgrV->getBddNodeV(ntkhandler->getNetNameOrFormedWithId(nId)+"_ns");
		const V3NetId& nId2 = ntk->getInputNetId(nId,0);
		BddNodeV b2=bddMgrV->getBddNodeV(nId2.id);
		if(b2()==0){
		//	cout<<"haven't build:"<<nId2.id<<endl;
			ntk->buildBdd(nId2);
			b2=bddMgrV->getBddNodeV(nId2.id);
		}
		_tri &= ~(b ^ b2); 
	}
	_tr=_tri;
	/*
    CktModule* module = VLDesign.getFltModule(); assert (module);
//  unsigned getIoSize() const;i
    _tr=BddNodeV::_one;
//  BddNodeV _tr2=BddNodeV::_one;
    _tri=BddNodeV::_one;
    for(unsigned i2=0; i2<module->getDffSize();i2++){
        CktCell* tempdff=module->getSeqCell(i2);
        string inPinName = tempdff->getInPin(0)->getOutPin()->getCell()->getOutPinName();
        CktCell* tempdffin=tempdff->getInPin(0)->getOutPin()->getCell();
//      cout<<"inPinName "<<tempdffin->getOutPinName()<<" celltype "<<tempdffin->getCellTypeString() <<endl;
        const unsigned width2 = tempdffin->getOutPin()->getBusWidth();
        for(unsigned j2=0; j2<width2;j2++)
        {
            BddNodeV tempin=getBddNodeV(getBitName(tempdffin->getOutPinName(),j2));
            BddNodeV tempns=getBddNodeV(getNSName(getBitName(tempdff->getOutPinName(),j2)));
            BddNodeV temp2=(~(tempin^tempns));
            _tri &= temp2;//(~(tempin^tempns));

        }
    }*/
  	unsigned inbddsize=ntk->getInoutSize()+ntk->getInputSize(); 
	_tr=_tri;
       for(unsigned j=1 ; j<=inbddsize; j++ ){
                _tr=_tr.exist(j);
        }
    //_tr=tri.exist(1)  

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
