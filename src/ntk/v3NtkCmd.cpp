/****************************************************************************
  FileName     [ v3NtkCmd.cpp ]
  PackageName  [ v3/src/ntk ]
  Synopsis     [ Ntk Commands ]
  Author       [ Cheng-Yin Wu ]
  Copyright    [ Copyright(c) 2012 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef V3_NTK_CMD_C
#define V3_NTK_CMD_C

#include "v3Msg.h"
#include "v3NtkCmd.h"
#include "v3StrUtil.h"
#include "v3FileUtil.h"
#include "v3NtkInput.h"
#include "v3NtkParser.h"
#include "v3NtkWriter.h"
#include "v3NtkSimplify.h"

#include <iomanip>

bool initNtkCmd() {
   // Set 1'b0 and [0:0] 0 to V3BVXId and V3BusId, respectively
   const V3BVXId zeroId = V3BvNtk::hashV3ConstBitVec("1'b0"); assert (!zeroId);
   const V3BusId boolId = V3BvNtk::hashV3BusId(0, 0); assert (!boolId);
   // Reset Verbosity Settings
   V3NtkHandler::resetVerbositySettings();
   return (
         // Network Input Commands
         v3CmdMgr->regCmd("REAd Rtl",           3, 1, new V3ReadRTLCmd        ) &&
         v3CmdMgr->regCmd("REAd Aig",           3, 1, new V3ReadAIGCmd        ) &&
         v3CmdMgr->regCmd("REAd Btor",          3, 1, new V3ReadBTORCmd       ) &&
         // Network Output Commands
         v3CmdMgr->regCmd("WRIte Rtl",          3, 1, new V3WriteRTLCmd       ) &&
         v3CmdMgr->regCmd("WRIte Aig",          3, 1, new V3WriteAIGCmd       ) &&
         v3CmdMgr->regCmd("WRIte Btor",         3, 1, new V3WriteBTORCmd      ) &&
         // Network Report Commands
         v3CmdMgr->regCmd("PRInt NTk",          3, 2, new V3PrintNtkCmd       ) &&
         v3CmdMgr->regCmd("PLOt NTk",           3, 2, new V3PlotNtkCmd        ) &&
         // Network Hierarchy Manipulation Commands
         v3CmdMgr->regCmd("@CD",                3,    new V3SetNtkCmd         ) &&
         v3CmdMgr->regCmd("@LS",                3,    new V3ListNtkCmd        ) &&
         v3CmdMgr->regCmd("@PWD",               4,    new V3WorkNtkCmd        ) &&
         // Network Simplification Commands
         v3CmdMgr->regCmd("STRash NTk",         3, 2, new V3StrashNtkCmd      ) &&
         v3CmdMgr->regCmd("REDuce NTk",         3, 2, new V3ReduceNtkCmd      ) &&
         v3CmdMgr->regCmd("REWrite NTk",        3, 2, new V3RewriteNtkCmd     ) &&
         v3CmdMgr->regCmd("NETinfo",            3,    new V3NetInfoCmd) // MODIFICATION FOR SoCV BDD
   );
}

//----------------------------------------------------------------------
// REAd Rtl <(string fileName)> [-Filelist]
//----------------------------------------------------------------------
V3CmdExecStatus
V3ReadRTLCmd::exec(const string& option) {
   vector<string> options;
   V3CmdExec::lexOptions(option, options);
   
   bool fileList = false;
   string fileName = "";

   size_t n = options.size();
   for (size_t i = 0; i < n; ++i) {
      const string& token = options[i];
      if (v3StrNCmp("-Filelist", token, 2) == 0) {
         if (fileList) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else fileList = true;
      }
      else if (fileName == "") fileName = token;
      else return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
   }

   if (fileName == "") return V3CmdExec::errorOption(CMD_OPT_MISSING, "<(string fileName)>");

   V3NtkInput* inputHandler = V3NtkFromQuteRTL(fileName.c_str(), fileList);
   
   if (!inputHandler) Msg(MSG_ERR) << "Parse Failed !!" << endl;
   else v3Handler.pushAndSetCurHandler(inputHandler);
   return CMD_EXEC_DONE;
}

void
V3ReadRTLCmd::usage() const {
   Msg(MSG_IFO) << "Usage: REAd Rtl <(string fileName)> [-Filelist]" << endl;
}

void
V3ReadRTLCmd::help() const {
   Msg(MSG_IFO) << setw(20) << left << "REAd Rtl: " << "Read RTL (Verilog) Design." << endl;
}

//----------------------------------------------------------------------
// REAd Aig <(string fileName)> [-Symbol]
//----------------------------------------------------------------------
V3CmdExecStatus
V3ReadAIGCmd::exec(const string& option) {
   vector<string> options;
   V3CmdExec::lexOptions(option, options);
   
   bool symbol = false;
   string fileName = "";

   size_t n = options.size();
   for (size_t i = 0; i < n; ++i) {
      const string& token = options[i];
      if (v3StrNCmp("-Symbol", token, 2) == 0) {
         if (symbol) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else symbol = true;
      }
      else if (fileName == "") fileName = token;
      else return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
   }

   if (fileName == "") return V3CmdExec::errorOption(CMD_OPT_MISSING, "<(string fileName)>");

   V3NtkInput* inputHandler = V3AigParser(fileName.c_str(), symbol);
   
   if (!inputHandler) Msg(MSG_ERR) << "Parse Failed !!" << endl;
   else v3Handler.pushAndSetCurHandler(inputHandler);

   return CMD_EXEC_DONE;
}

void
V3ReadAIGCmd::usage() const {
   Msg(MSG_IFO) << "Usage: REAd Aig <(string fileName)> [-Symbol]" << endl;
}

void
V3ReadAIGCmd::help() const {
   Msg(MSG_IFO) << setw(20) << left << "REAd Aig: " << "Read AIGER Design." << endl;
}

//----------------------------------------------------------------------
// REAd Btor <(string fileName)> [-Symbol]
//----------------------------------------------------------------------
V3CmdExecStatus
V3ReadBTORCmd::exec(const string& option) {
   vector<string> options;
   V3CmdExec::lexOptions(option, options);
   
   bool symbol = false;
   string fileName = "";

   size_t n = options.size();
   for (size_t i = 0; i < n; ++i) {
      const string& token = options[i];
      if (v3StrNCmp("-Symbol", token, 2) == 0) {
         if (symbol) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else symbol = true;
      }
      else if (fileName == "") fileName = token;
      else return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
   }

   if (fileName == "") return V3CmdExec::errorOption(CMD_OPT_MISSING, "<(string fileName)>");

   V3NtkInput* inputHandler = V3BtorParser(fileName.c_str(), symbol);
   
   if (!inputHandler) Msg(MSG_ERR) << "Parse Failed !!" << endl;
   else v3Handler.pushAndSetCurHandler(inputHandler);
   return CMD_EXEC_DONE;
}

void
V3ReadBTORCmd::usage() const {
   Msg(MSG_IFO) << "Usage: REAd BTOR <(string fileName)> [-Symbol]" << endl;
}

void
V3ReadBTORCmd::help() const {
   Msg(MSG_IFO) << setw(20) << left << "REAd Btor: " << "Read BTOR Design." << endl;
}

//----------------------------------------------------------------------
// WRIte Rtl <(string fileName)> [-Symbol] [-Initial]
//----------------------------------------------------------------------
V3CmdExecStatus
V3WriteRTLCmd::exec(const string& option) {
   vector<string> options;
   V3CmdExec::lexOptions(option, options);
   
   bool symbol = false, initial = false;
   string fileName = "";

   size_t n = options.size();
   for (size_t i = 0; i < n; ++i) {
      const string& token = options[i];
      if (v3StrNCmp("-Symbol", token, 2) == 0) {
         if (symbol) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else symbol = true;
      }
      else if (v3StrNCmp("-Initial", token, 2) == 0) {
         if (initial) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else initial = true;
      }
      else if (fileName == "") fileName = token;
      else return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
   }

   if (fileName == "") return V3CmdExec::errorOption(CMD_OPT_MISSING, "<(string fileName)>");

   V3NtkHandler* const handler = v3Handler.getCurHandler();
   if (handler) V3RTLWriter(handler, fileName.c_str(), symbol, initial);
   else Msg(MSG_ERR) << "Empty Ntk !!" << endl;
   return CMD_EXEC_DONE;
}

void
V3WriteRTLCmd::usage() const {
   Msg(MSG_IFO) << "Usage: WRIte Rtl <(string fileName)> [-Symbol]" << endl;
}

void
V3WriteRTLCmd::help() const {
   Msg(MSG_IFO) << setw(20) << left << "WRIte Rtl: " << "Write RTL (Verilog) Network." << endl;
}

//----------------------------------------------------------------------
// WRIte Aig <(string fileName)> [-Symbol]
//----------------------------------------------------------------------
V3CmdExecStatus
V3WriteAIGCmd::exec(const string& option) {
   vector<string> options;
   V3CmdExec::lexOptions(option, options);
   
   bool symbol = false;
   string fileName = "";

   size_t n = options.size();
   for (size_t i = 0; i < n; ++i) {
      const string& token = options[i];
      if (v3StrNCmp("-Symbol", token, 2) == 0) {
         if (symbol) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else symbol = true;
      }
      else if (fileName == "") fileName = token;
      else return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
   }

   if (fileName == "") return V3CmdExec::errorOption(CMD_OPT_MISSING, "<(string fileName)>");

   V3NtkHandler* const handler = v3Handler.getCurHandler();
   if (handler) {
      if (!dynamic_cast<const V3BvNtk*>(handler->getNtk())) V3AigWriter(handler, fileName.c_str(), symbol);
      else Msg(MSG_ERR) << "Current Network is NOT an AIG Ntk !!" << endl;
   }
   else Msg(MSG_ERR) << "Empty Ntk !!" << endl;
   return CMD_EXEC_DONE;
}

void
V3WriteAIGCmd::usage() const {
   Msg(MSG_IFO) << "Usage: WRIte Aig <(string fileName)> [-Symbol]" << endl;
}

void
V3WriteAIGCmd::help() const {
   Msg(MSG_IFO) << setw(20) << left << "WRIte Aig: " << "Write AIGER Network." << endl;
}

//----------------------------------------------------------------------
// WRIte Btor <(string fileName)> [-Symbol]
//----------------------------------------------------------------------
V3CmdExecStatus
V3WriteBTORCmd::exec(const string& option) {
   vector<string> options;
   V3CmdExec::lexOptions(option, options);
   
   bool symbol = false;
   string fileName = "";

   size_t n = options.size();
   for (size_t i = 0; i < n; ++i) {
      const string& token = options[i];
      if (v3StrNCmp("-Symbol", token, 2) == 0) {
         if (symbol) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else symbol = true;
      }
      else if (fileName == "") fileName = token;
      else return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
   }

   if (fileName == "") return V3CmdExec::errorOption(CMD_OPT_MISSING, "<(string fileName)>");

   V3NtkHandler* const handler = v3Handler.getCurHandler();
   if (handler) {
      if (dynamic_cast<const V3BvNtk*>(handler->getNtk())) V3BtorWriter(handler, fileName.c_str(), symbol);
      else Msg(MSG_ERR) << "Current Network is NOT a BV Ntk !!" << endl;
   }
   else Msg(MSG_ERR) << "Empty Ntk !!" << endl;
   return CMD_EXEC_DONE;
}

void
V3WriteBTORCmd::usage() const {
   Msg(MSG_IFO) << "Usage: WRIte Btor <(string fileName)> [-Symbol]" << endl;
}

void
V3WriteBTORCmd::help() const {
   Msg(MSG_IFO) << setw(20) << left << "WRIte Btor: " << "Write BTOR Network." << endl;
}

//----------------------------------------------------------------------
// PRInt NTk [| -Summary | -Primary | -Verbose | -Netlist]
//----------------------------------------------------------------------
V3CmdExecStatus
V3PrintNtkCmd::exec(const string& option) {
   vector<string> options;
   V3CmdExec::lexOptions(option, options);

   bool summary = false, primary = false, verbose = false, netlist = false;

   size_t n = options.size();
   for (size_t i = 0; i < n; ++i) {
      const string& token = options[i];
      if (v3StrNCmp("-Summary", token, 2) == 0) {
         if (summary || primary || verbose || netlist) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else summary = true;
      }
      else if (v3StrNCmp("-Primary", token, 2) == 0) {
         if (summary || primary || verbose || netlist) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else primary = true;
      }
      else if (v3StrNCmp("-Verbose", token, 2) == 0) {
         if (summary || primary || verbose || netlist) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else verbose = true;
      }
      else if (v3StrNCmp("-Netlist", token, 2) == 0) {
         if (summary || primary || verbose || netlist) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else netlist = true;
      }
      else return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
   }

   if (!(summary || primary || verbose || netlist)) summary = true;

   V3NtkHandler* const handler = v3Handler.getCurHandler();
   if (handler) {
      if (verbose) handler->printVerbose();
      else if (primary) handler->printPrimary();
      else if (netlist) handler->printNetlist();
      else handler->printSummary();
   }
   else Msg(MSG_ERR) << "Empty Ntk !!" << endl;
   return CMD_EXEC_DONE;
}

void
V3PrintNtkCmd::usage() const {
   Msg(MSG_IFO) << "Usage: PRInt NTk [| -Summary | -Primary | -Verbose | -Netlist]" << endl;
}

void
V3PrintNtkCmd::help() const {
   Msg(MSG_IFO) << setw(20) << left << "PRInt NTk: " << "Print Network Information." << endl;
}

//----------------------------------------------------------------------
// PLOt NTk [| -DOT | -PNG | -PS] <(string fileName)>
//          <(-Level | -Depth) (unsigned size)> [-Monochrome]
//----------------------------------------------------------------------
V3CmdExecStatus
V3PlotNtkCmd::exec(const string& option) {
   vector<string> options;
   V3CmdExec::lexOptions(option, options);

   bool dot = false, png = false, ps = false;
   bool level = false, depth = false, inSize = false;
   bool monochrome = false;
   string fileName = ""; int size = 0;

   size_t n = options.size();
   for (size_t i = 0; i < n; ++i) {
      const string& token = options[i];
      if (v3StrNCmp("-DOT", token, 4) == 0) {
         if (inSize) return V3CmdExec::errorOption(CMD_OPT_MISSING, "(unsigned size)");
         else if (dot || png || ps) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else dot = true;
      }
      else if (v3StrNCmp("-PNG", token, 4) == 0) {
         if (inSize) return V3CmdExec::errorOption(CMD_OPT_MISSING, "(unsigned size)");
         else if (dot || png || ps) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else png = true;
      }
      else if (v3StrNCmp("-PS", token, 3) == 0) {
         if (inSize) return V3CmdExec::errorOption(CMD_OPT_MISSING, "(unsigned size)");
         else if (dot || png || ps) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else ps = true;
      }
      else if (v3StrNCmp("-Level", token, 2) == 0) {
         if (inSize) return V3CmdExec::errorOption(CMD_OPT_MISSING, "(unsigned size)");
         else if (level || depth) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else level = inSize = true;
      }
      else if (v3StrNCmp("-Depth", token, 2) == 0) {
         if (inSize) return V3CmdExec::errorOption(CMD_OPT_MISSING, "(unsigned size)");
         else if (level || depth) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else depth = inSize = true;
      }
      else if (v3StrNCmp("-Monochrome", token, 2) == 0) {
         if (inSize) return V3CmdExec::errorOption(CMD_OPT_MISSING, "(unsigned size)");
         else monochrome = true;
      }
      else if (inSize) {
         if (!v3Str2Int(token, size)) return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, "(unsigned size)");
         if (size <= 0) return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, "(unsigned size)");
         inSize = false;
      }
      else if (fileName == "") fileName = token;
      else return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
   }

   if ("" == fileName) return V3CmdExec::errorOption(CMD_OPT_MISSING, "(string fileName)");
   if (!size) return V3CmdExec::errorOption(CMD_OPT_MISSING, "(unsigned size)");
   if (!(dot || png || ps)) dot = true;

   V3NtkHandler* const handler = v3Handler.getCurHandler();
   if (handler) {
      // Set Outputs
      V3NetVec outputNets; outputNets.reserve(handler->getNtk()->getOutputSize());
      for (uint32_t i = 0; i < handler->getNtk()->getOutputSize(); ++i) 
         outputNets.push_back(handler->getNtk()->getOutput(i));
      if (level) V3PlotNtkByLevel(handler, fileName.c_str(), size, outputNets, monochrome);
      else V3PlotNtkByDepth(handler, fileName.c_str(), size, outputNets, monochrome);
      if (png) dotToPng(fileName);
      else if (ps) dotToPs(fileName);
   }
   else Msg(MSG_ERR) << "Empty Ntk !!" << endl;
   return CMD_EXEC_DONE;
}

void
V3PlotNtkCmd::usage() const {
   Msg(MSG_IFO) << "Usage: PLOt NTk [| -DOT | -PNG | -PS] <(string fileName)>" << endl;
   Msg(MSG_IFO) << "                <(-Level | -Depth) (unsigned size)> [-Monochrome]" << endl;
}

void
V3PlotNtkCmd::help() const {
   Msg(MSG_IFO) << setw(20) << left << "PLOt NTk: " << "Plot Network Structure." << endl;
}

//----------------------------------------------------------------------
// @CD [/ | - | . | .. | (Path_Format)]
//----------------------------------------------------------------------
V3CmdExecStatus
V3SetNtkCmd::exec(const string& option) {
   vector<string> options;
   V3CmdExec::lexOptions(option, options);

   bool cdRoot = false, cdLast = false, cdCur = false, cdPrev = false;
   string path = "";

   size_t n = options.size();
   for (size_t i = 0; i < n; ++i) {
      const string& token = options[i];
      if ("/" == token) {
         if (cdRoot || cdLast || cdCur || cdPrev || path.size()) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else cdRoot = true;
      }
      else if ("-" == token) {
         if (cdRoot || cdLast || cdCur || cdPrev || path.size()) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else cdLast = true;
      }
      else if ("." == token) {
         if (cdRoot || cdLast || cdCur || cdPrev || path.size()) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else cdCur = true;
      }
      else if (".." == token) {
         if (cdRoot || cdLast || cdCur || cdPrev || path.size()) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else cdPrev = true;
      }
      else if (!path.size()) path = token;
      else return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
   }

   V3NtkHandler* const handler = v3Handler.getCurHandler();
   if (handler) {
      if (cdRoot) v3Handler.setRootHandler();  // Change to Root Ntk
      else if (cdLast) v3Handler.setLastHandler();  // Change to Last Ntk
      else if (cdPrev) v3Handler.setPrevHandler();  // Change to Topper Ntk  (Stall if current Ntk is the Base Ntk)
      else if (path.size()) v3Handler.setCurHandlerFromPath(path);  // Change to Ntk with specified path
      else if (!cdCur) v3Handler.setBaseHandler();  // Change to Base (Top) Ntk
   }
   else Msg(MSG_ERR) << "Empty Ntk !!" << endl;
   return CMD_EXEC_DONE;
}

void
V3SetNtkCmd::usage() const {
   Msg(MSG_IFO) << "Usage: @CD [/ | .. | (Path_Format)]" << endl;
   Msg(MSG_IFO) << "       Path_Format : [/(unsigned ntkID)][/(unsigned subModuleIndex)]*" << endl;
   Msg(MSG_IFO) << "       e.g. @CD 0/1 will change Ntk to be the second submodule of the first submodule." << endl;
   Msg(MSG_IFO) << "       e.g. @CD /5/1 will change Ntk to be the second submodule of Ntk with id = 5." << endl;
}

void
V3SetNtkCmd::help() const {
   Msg(MSG_IFO) << setw(20) << left << "@CD: " << "Change Current Network." << endl;
}

//----------------------------------------------------------------------
// @LS [-Recursive [(unsigned level)]]
//----------------------------------------------------------------------
V3CmdExecStatus
V3ListNtkCmd::exec(const string& option) {
   vector<string> options;
   V3CmdExec::lexOptions(option, options);

   bool recur = false, recurON = false;
   uint32_t level = 0;

   size_t n = options.size();
   for (size_t i = 0; i < n; ++i) {
      const string& token = options[i];
      if (v3StrNCmp("-Recursive", token, 2) == 0) {
         if (recur) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else recur = recurON = true;
      }
      else if (recurON) {
         int temp;
         if (!v3Str2Int(token, temp)) return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, "(unsigned level)");
         if (temp <= 0) return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, "(unsigned level)");
         level = (uint32_t)temp; --level; recurON = false;
      }
      else return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
   }

   V3NtkHandler* const handler = v3Handler.getCurHandler();
   if (handler) v3Handler.printRecurHierarchy(handler, level, 0);
   else Msg(MSG_ERR) << "Empty Ntk !!" << endl;
   return CMD_EXEC_DONE;
}

void
V3ListNtkCmd::usage() const {
   Msg(MSG_IFO) << "Usage: @LS [-Recursive [(unsigned level)]]" << endl;
}

void
V3ListNtkCmd::help() const {
   Msg(MSG_IFO) << setw(20) << left << "@LS: " << "List the Hierarchy of Current Network." << endl;
}

//----------------------------------------------------------------------
// @PWD [| -Base | -Root ]
//----------------------------------------------------------------------
V3CmdExecStatus
V3WorkNtkCmd::exec(const string& option) {
   vector<string> options;
   V3CmdExec::lexOptions(option, options);

   bool base = false, root = false;

   size_t n = options.size();
   for (size_t i = 0; i < n; ++i) {
      const string& token = options[i];
      if (v3StrNCmp("-Base", token, 2) == 0) {
         if (root || base) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else base = true;
      }
      else if (v3StrNCmp("-Root", token, 2) == 0) {
         if (root || base) return V3CmdExec::errorOption(CMD_OPT_EXTRA, token);
         else root = true;
      }
      else return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
   }

   if (!(base || root)) base = true;

   V3NtkHandler* const handler = v3Handler.getCurHandler();
   if (handler) {
      if (base) v3Handler.printNtkInHierarchy();
      else v3Handler.printNtkInAncestry();
   }
   else Msg(MSG_ERR) << "Empty Ntk !!" << endl;
   return CMD_EXEC_DONE;
}

void
V3WorkNtkCmd::usage() const {
   Msg(MSG_IFO) << "Usage: @PWD" << endl;
}

void
V3WorkNtkCmd::help() const {
   Msg(MSG_IFO) << setw(20) << left << "@PWD: " << "Print the Path of Current Network." << endl;
}

//----------------------------------------------------------------------
// STRash NTk
//----------------------------------------------------------------------
V3CmdExecStatus
V3StrashNtkCmd::exec(const string& option) {
   vector<string> options;
   V3CmdExec::lexOptions(option, options);

   size_t n = options.size();
   if (n) return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);

   V3NtkHandler* const handler = v3Handler.getCurHandler();
   if (handler) {
      V3NtkHandler::setExternalVerbosity(); V3NtkHandler::setStrash(true);
      v3Handler.pushAndSetCurHandler(new V3NtkSimplify(handler));
      V3NtkHandler::setInternalVerbosity();
   }
   else Msg(MSG_ERR) << "Empty Ntk !!" << endl;
   return CMD_EXEC_DONE;
}

void
V3StrashNtkCmd::usage() const {
   Msg(MSG_IFO) << "Usage: STRash NTk" << endl;
}

void
V3StrashNtkCmd::help() const {
   Msg(MSG_IFO) << setw(20) << left << "STRash NTk: " << "Perform Structural Hashing on Current Network." << endl;
}

//----------------------------------------------------------------------
// REDuce NTk
//----------------------------------------------------------------------
V3CmdExecStatus
V3ReduceNtkCmd::exec(const string& option) {
   vector<string> options;
   V3CmdExec::lexOptions(option, options);

   size_t n = options.size();
   if (n) return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);

   V3NtkHandler* const handler = v3Handler.getCurHandler();
   if (handler) {
      V3NtkHandler::setExternalVerbosity(); V3NtkHandler::setReduce(true);
      v3Handler.pushAndSetCurHandler(new V3NtkSimplify(handler));
      V3NtkHandler::setInternalVerbosity();
   }
   else Msg(MSG_ERR) << "Empty Ntk !!" << endl;
   return CMD_EXEC_DONE;
}

void
V3ReduceNtkCmd::usage() const {
   Msg(MSG_IFO) << "Usage: REDuce NTk" << endl;
}

void
V3ReduceNtkCmd::help() const {
   Msg(MSG_IFO) << setw(20) << left << "REDuce NTk: " << "Perform Reduction on Current Network." << endl;
}

//----------------------------------------------------------------------
// REWrite NTk
//----------------------------------------------------------------------
V3CmdExecStatus
V3RewriteNtkCmd::exec(const string& option) {
   vector<string> options;
   V3CmdExec::lexOptions(option, options);

   size_t n = options.size();
   if (n) return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);

   V3NtkHandler* const handler = v3Handler.getCurHandler();
   if (handler) {
      V3NtkHandler::setExternalVerbosity(); V3NtkHandler::setRewrite(true);
      v3Handler.pushAndSetCurHandler(new V3NtkSimplify(handler));
      V3NtkHandler::setInternalVerbosity();
   }
   else Msg(MSG_ERR) << "Empty Ntk !!" << endl;
   return CMD_EXEC_DONE;
}

void
V3RewriteNtkCmd::usage() const {
   Msg(MSG_IFO) << "Usage: REWrite NTk" << endl;
}

void
V3RewriteNtkCmd::help() const {
   Msg(MSG_IFO) << setw(20) << left << "REWrite NTk: " << "Perform Rewriting Rules on Current Network." << endl;
}

/* MODIFICATION FOR SoCV BDD */
//----------------------------------------------------------------------
//    NETinfo < -Netid <netId> | -Output <outputIndex> > <unsigned level>
//----------------------------------------------------------------------
V3CmdExecStatus
V3NetInfoCmd::exec(const string& option)
{
  V3NtkHandler* const handler = v3Handler.getCurHandler();

  if (!handler) {
    Msg(MSG_WAR) << "Design does not exist !!!" << endl;
    return CMD_EXEC_ERROR;
  }

  vector<string> options;
  V3CmdExec::lexOptions(option, options);
  if( options.size() < 3 )
    return V3CmdExec::errorOption(CMD_OPT_MISSING, "");
  if( options.size() > 3)
    return V3CmdExec::errorOption(CMD_OPT_EXTRA, options[3]);

  V3Ntk* const ntk = handler->getNtk();
  bool isNet = false;
  bool isOutput = false;
  V3NetId netId;
  int num = 0;
  int level = 0;

  if (v3StrNCmp("-Netid", options[0], 2) == 0) isNet = true;
  else if(v3StrNCmp("-Output", options[0], 2) == 0) isOutput = true;
  else return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
  if(!v3Str2Int(options[1], num) || (num < 0))
    return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
  if(isNet) {
    if((unsigned)num >= ntk->getNetSize()) {
      Msg(MSG_ERR) << "Net with Id " << num << " does NOT Exist in Current Ntk !!" << endl;
      return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
    }
    netId = V3NetId::makeNetId(num);
  } else if(isOutput) {
    if((unsigned)num >= ntk->getOutputSize()) {
      Msg(MSG_ERR) << "Output with Index " << num << " does NOT Exist in Current Ntk !!" << endl;
      return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
    }
    netId = ntk->getOutput(num);
  }
  if(!v3Str2Int(options[2], level) || (level < 0))
    return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);

  ntk->newMiscData();
  ntk->printFaninRec(handler, netId, 0, level);

  return CMD_EXEC_DONE;
}

void
V3NetInfoCmd::usage() const
{
  Msg(MSG_IFO) << "Usage: NETinfo < -Netid <netId> | -Output <outputIndex> > <unsigned level>" << endl;
}

void
V3NetInfoCmd::help() const
{
  cout << setw(20) << left << "NETINFO:"
       << "provide the information of a net" << endl;
}
/* END OF MODIFICATION */

#endif
