// ============================================================================
//        __
//   \\__/ o\    (C) 2012-2024  Robert Finch, Waterloo
//    \  __ /    All rights reserved.
//     \/_//     robfinch<remove>@finitron.ca
//       ||
//
// CC64 - 'C' derived language compiler
//  - 64 bit CPU
//
// This source file is free software: you can redistribute it and/or modify 
// it under the terms of the GNU Lesser General Public License as published 
// by the Free Software Foundation, either version 3 of the License, or     
// (at your option) any later version.                                      
//                                                                          
// This source file is distributed in the hope that it will be useful,      
// but WITHOUT ANY WARRANTY; without even the implied warranty of           
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            
// GNU General Public License for more details.                             
//                                                                          
// You should have received a copy of the GNU General Public License        
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    
//                                                                          
// ============================================================================
//
#include "stdafx.h"
/*      global definitions      */

int syntax;
char comment_char = ';';
const char* imm_char = "$";
Compiler compiler;
int pass;
int maxPn = 15;
int gCpu = 5;
#ifdef LB650
LB650CPU cpu;
int regTS = 52;
int regPC = 34;
int regSP = 31;
int regFP = 30;
int regAFP = 31;      // alternate frame pointer
int regLR = 41;
int regXLR = 28;
int regXHSP = 27;
int regGP = 29;
int regGP1 = 60;
int regGP2 = 51;
int regGOT = 45;
int regTP = 52;
int regCLP = 25;                // class pointer
int regPP = 56;					// program pointer
int regZero = 0;
int regXoffs = 55;
int regAsm = 58;
int pregSP = 31;
int pregFP = 30;
int pregLR = 41;
int pregXLR = 28;
int pregGP = 29;
int pregTP = 27;
int pregCLP = 25;                // class pointer
int pregPP = 24;					// program pointer
int pregZero = 0;
int pregFirstTemp = 9;
int pregLastTemp = 17;
int pregFirstRegvar = 18;
int pregLastRegvar = 26;
int pregFirstArg = 1;
int pregLastArg = 8;
int regRS = 46;
int regCS = 47;
int farcode = 0;
int wcharSupport = 1;
int verbose = 0;
int use_gp = 1;
int use_iprel = 0;
int address_bits = 32;
int maxVL = 64;
int nregs = 128;

#endif
#ifdef QUPLS
QuplsCPU cpu;
int regTS = 52;
int regPC = 34;
int regSP = 31;
int regFP = 30;
int regAFP = 31;      // alternate frame pointer
int regLR = 41;
int regXLR = 28;
int regXHSP = 27;
int regGP = 29;
int regGP1 = 60;
int regGP2 = 51;
int regGOT = 45;
int regTP = 52;
int regCLP = 25;                // class pointer
int regPP = 56;					// program pointer
int regZero = 0;
int regXoffs = 55;
int regAsm = 58;
int pregSP = 31;
int pregFP = 30;
int pregLR = 41;
int pregXLR = 28;
int pregGP = 29;
int pregTP = 27;
int pregCLP = 25;                // class pointer
int pregPP = 24;					// program pointer
int pregZero = 0;
int pregFirstTemp = 9;
int pregLastTemp = 17;
int pregFirstRegvar = 18;
int pregLastRegvar = 26;
int pregFirstArg = 1;
int pregLastArg = 8;
int regRS = 46;
int regCS = 47;
int farcode = 0;
int wcharSupport = 1;
int verbose = 0;
int use_gp = 1;
int use_iprel = 0;
int address_bits = 32;
int maxVL = 64;
int nregs = 128;

#endif
#ifdef QUPLS40
CPU cpu;
int regTS = 52;
int regPC = 34;
int regSP = 15;
int regFP = 14;
int regAFP = 31;      // alternate frame pointer
int regLR = 16;
int regXLR = 28;
int regXHSP = 27;
int regGP = 13;
int regGP1 = 13;
int regGP2 = 13;
int regGOT = 45;
int regTP = 52;
int regCLP = 25;                // class pointer
int regPP = 56;					// program pointer
int regZero = 0;
int regXoffs = 55;
int regAsm = 58;
int pregSP = 15;
int pregFP = 14;
int pregLR = 16;
int pregXLR = 28;
int pregGP = 13;
int pregTP = 27;
int pregCLP = 25;                // class pointer
int pregPP = 24;					// program pointer
int pregZero = 0;
int pregFirstTemp = 4;
int pregLastTemp = 7;
int pregFirstRegvar = 8;
int pregLastRegvar = 12;
int pregFirstArg = 1;
int pregLastArg = 3;
int regRS = 46;
int regCS = 47;
int farcode = 0;
int wcharSupport = 1;
int verbose = 0;
int use_gp = 1;
int use_iprel = 0;
int address_bits = 32;
int maxVL = 64;
int nregs = 128;

#endif
#ifdef THOR
CPU cpu;
int regTS = 52;
int regPC = 53;
int regSP = 62;
int regFP = 61;
int regAFP = 31;      // alternate frame pointer
int regLR = 56;
int regXLR = 28;
int regXHSP = 27;
int regGP = 60;
int regGP1 = 59;
int regGOT = 45;
int regTP = 52;
int regCLP = 25;                // class pointer
int regPP = 56;					// program pointer
int regZero = 0;
int regXoffs = 55;
int regAsm = 58;
int pregSP = 30;
int pregFP = 29;
int pregLR = 1;
int pregXLR = 28;
int pregGP = 28;
int pregTP = 27;
int pregCLP = 25;                // class pointer
int pregPP = 24;					// program pointer
int pregZero = 0;
int pregFirstTemp = 4;
int pregLastTemp = 9;
int pregFirstRegvar = 11;
int pregLastRegvar = 17;
int pregFirstArg = 18;
int pregLastArg = 22;
int regRS = 46;
int regCS = 47;
int farcode = 0;
int wcharSupport = 1;
int verbose = 0;
int use_gp = 1;
int address_bits = 32;
int maxVL = 64;
int nregs = 128;

int cpu.sizeOfWord = 16;
int sizeOfFP = 16;
int cpu.sizeOfFPS = 4;
int cpu.sizeOfFPD = 8;
int cpu.sizeOfFPT = 12;
int cpu.sizeOfFPQ = 16;
int cpu.sizeOfPtr = 16;
int cpu.sizeOfInt = 16;
int cpu.sizeOfDecimal = 16;
int cpu.sizeOfPosit = 8;

#endif
#ifdef RISCV
CPU cpu;
int regTS = 52;         // so the symbol is defined
int regSP = 2;
int regFP = 8;
int regLR = 1;
int regGP = 3;
int regGP1 = 27;
int regTP = 4;
int regCLP = -1;                // class pointer
int regPP = -1;					// program pointer
int regZero = 0;
int regXoffs = -1;
int regAsm = -1;
int wcharSupport = 1;
int verbose = 0;
int use_gp = 1;
int address_bits = 32;
int nregs = 32;

int sizeOfFP = 8;

int use_iprel = 0;
#endif

#ifdef BIGFOOT
BigfootCPU cpu;
int regTS = 52;
int regPC = 34;
int regSP = 31;
int regFP = 30;
int regAFP = 31;      // alternate frame pointer
int regLR = 41;
int regXLR = 28;
int regXHSP = 27;
int regGP = 29;
int regGP1 = 60;
int regGP2 = 51;
int regGOT = 45;
int regTP = 52;
int regCLP = 25;                // class pointer
int regPP = 56;					// program pointer
int regZero = 0;
int regXoffs = 55;
int regAsm = 58;
int regFirstTemp = 9;
int pregSP = 31;
int pregFP = 30;
int pregLR = 41;
int pregXLR = 28;
int pregGP = 29;
int pregTP = 27;
int pregCLP = 25;                // class pointer
int pregPP = 24;					// program pointer
int pregZero = 0;
int pregFirstTemp = 9;
int pregLastTemp = 17;
int pregFirstRegvar = 18;
int pregLastRegvar = 26;
int pregFirstArg = 1;
int pregLastArg = 8;
int regRS = 46;
int regCS = 47;
int farcode = 0;
int wcharSupport = 1;
int verbose = 0;
int use_gp = 1;
int use_iprel = 0;
int address_bits = 32;
int maxVL = 64;
int nregs = 128;

#endif

#ifdef I386
i386CPU cpu;
int regTS = 52;
int regPC = 34;
int regSP = 31;
int regFP = 30;
int regAFP = 31;      // alternate frame pointer
int regLR = 41;
int regXLR = 28;
int regXHSP = 27;
int regGP = 29;
int regGP1 = 60;
int regGP2 = 51;
int regGOT = 45;
int regTP = 52;
int regCLP = 25;                // class pointer
int regPP = 56;					// program pointer
int regZero = 0;
int regXoffs = 55;
int regAsm = 58;
int pregSP = 31;
int pregFP = 30;
int pregLR = 41;
int pregXLR = 28;
int pregGP = 29;
int pregTP = 27;
int pregCLP = 25;                // class pointer
int pregPP = 24;					// program pointer
int pregZero = 0;
int pregFirstTemp = 9;
int pregLastTemp = 17;
int pregFirstRegvar = 18;
int pregLastRegvar = 26;
int pregFirstArg = 1;
int pregLastArg = 8;
int regRS = 46;
int regCS = 47;
int farcode = 0;
int wcharSupport = 1;
int verbose = 0;
int use_gp = 1;
int use_iprel = 0;
int address_bits = 32;
int maxVL = 64;
int nregs = 128;

#endif

std::ifstream *ifs;
txtoStream ofs;
txtoStream lfs;
txtoStream dfs;
txtoStream irfs;

/*
FILE            *input = 0,
                *list = 0,
                *output = 0;*/
FILE			*outputG = 0;
int incldepth = 0;
int             lineno = 0;
int             nextlabel = 1;
int             lastch = 0;
int             lastst = 0;
char            lastkw[128] = "";
char            laststr[MAX_STRLEN + 1] = "";
int64_t			ival = 0;
double          rval = 0.0;
Float128		rval128;
Int128 ival128;
Posit64 pval64;
char float_precision = 'd';
char int_precision = ' ';
//FloatTriple     FAC1,FAC2;
//FLOAT           rval = {0,0,0,0,0,0};
int parseEsc = TRUE;

TABLE           gsyms[257];// = {0,0},
int DataLabels[65535];
	           
Symbol             *lasthead = (Symbol *)NULL;
Float128		*quadtab = nullptr;
struct slit     *strtab = (struct slit *)NULL;
int             lc_auto = 0;
int				lc_thread = 0;
Statement    *bodyptr = 0;
int             global_flag = 1;
TABLE           defsyms;
CSet *save_mask = nullptr;          /* register save mask */
CSet *fpsave_mask = nullptr;
CSet* psave_mask = nullptr;
TYP             tp_int, tp_econst;
bool dogen = true;
int isKernel = FALSE;
int isPascal = TRUE;
int isOscall = FALSE;
int isInterrupt = FALSE;
int isTask = FALSE;
int isNocall = FALSE;
int optimize = TRUE;
int opt_noregs = FALSE;
int opt_nopeep;
int opt_noexpr = FALSE;
int opt_nocgo = FALSE;
int opt_size = FALSE;
int opt_vreg = FALSE;
int opt_loop_invariant = TRUE;
int mixedSource = FALSE;
Function *currentFn = nullptr;
int callsFn = FALSE;
int stmtdepth = 0;

int seg_aligned[20];
char nmspace[20][100];
int bsave_mask;
short int loop_active;
ENODE* gNameRefNode;
TABLE* gCurrentTbl;
std::map<int, std::string*> DataLabelMap;
CSet string_exclude;

#ifdef LB650
LB650CodeGenerator cg;
LB650StatementGenerator sg;
#endif
#ifdef QUPLS
QuplsCodeGenerator cg;
QuplsStatementGenerator sg;
#endif
#ifdef QUPLS40
QuplsCodeGenerator cg;
QuplsStatementGenerator sg;
#endif
#ifdef THOR
ThorCodeGenerator cg;
ThorStatementGenerator sg;
#endif
#ifdef RISCV
RiscvCodeGenerator cg;
RiscvStatementGenerator sg;
#endif
#ifdef BIGFOOT
BigfootCodeGenerator cg;
BigfootStatementGenerator sg;
#endif
#ifdef I386
i386CodeGenerator cg;
i386StatementGenerator sg;
#endif
