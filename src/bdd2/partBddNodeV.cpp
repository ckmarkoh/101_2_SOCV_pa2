/****************************************************************************
  FileName     [ partBddNodeV.cpp ]
  PackageName  [ ]
  Synopsis     [ Define BDD Node member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-2009 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <sstream>
#include <fstream>
#include <cassert>
#include "bddNodeV.h"
#include "bddMgrV.h"

BddNodeV
BddNodeV::exist(unsigned l) const
{
   // TODO: return existentially quantified BddNodeV on this BddNodeV
   return BddNodeV::_one;
}

const void* BddNodeV::socv_func_1(vector<void*> arg_vec) const
{
   return NULL;
}

const void* BddNodeV::socv_func_2(vector<void*> arg_vec) const
{
  return NULL;
}

const void* BddNodeV::socv_func_3(vector<void*> arg_vec) const
{
  return NULL;
}

const void* BddNodeV::socv_func_4(vector<void*> arg_vec) const
{
  return NULL;
}
