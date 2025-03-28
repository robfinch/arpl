#ifndef _CGBLDEC_H
#define _CGBLDEC_H

// ============================================================================
//        __
//   \\__/ o\    (C) 2012-2025  Robert Finch, Waterloo
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
/*      global ParseSpecifierarations     */
#define MOT   0
#define STD   1
#define TABLE888	888
#define RAPTOR64	64
#define W65C816     816
#define FISA64      164
#define DSD7        7
#define isThor		(gCpu==THOR)
#define isTable888	(gCpu==TABLE888)
#define isRaptor64	(gCpu==RAPTOR64)
#define is816       (gCpu==W65C816)
#define isFISA64    (gCpu==FISA64)
#define isDSD7      (gCpu==DSD7)
#define isRiscv     (gCpu==RISCV)
#define isBigfoot   (gCpu==BIGFOOT)
#define isI386      (gCpu==I386)
//#define DOTRACE	1
#ifdef DOTRACE
#define TRACE(x)	x
#else
#define TRACE(x)
#endif

extern char comment_char;
extern const char* imm_char;
extern Compiler compiler;
extern int pass;
extern int maxPn;
extern int hook_predreg;
extern int gCpu;
extern int regGP;
extern int regGP1;
extern int regGP2;
extern int regGOT;
extern int regTP;
extern int regSP;
extern int regFP;
extern int regAFP;
extern int regLR;
extern int regXLR;
extern int regXHSP;
extern int regTS;
extern int regPC;
extern int regCLP;
extern int regPP;
extern int regZero;
extern int regAsm;
extern int regFirstTemp;
extern int regLastTemp;
extern int regXoffs;
extern int regFirstRegvar;
extern int regLastRegvar;
extern int regFirstArg;
extern int regLastArg;
extern int pregGP;
extern int pregTP;
extern int pregSP;
extern int pregFP;
extern int pregLR;
extern int pregXLR;
extern int pregPC;
extern int pregCLP;
extern int pregPP;
extern int pregZero;
extern int pregFirstTemp;
extern int pregLastTemp;
extern int pregFirstRegvar;
extern int pregLastRegvar;
extern int pregFirstArg;
extern int pregLastArg;
extern int regRS;
extern int regCS;
extern int opt_vreg;
extern int farcode;
extern int wcharSupport;
extern int verbose;
extern int use_gp;
extern int use_iprel;
extern int address_bits;
extern std::ifstream *ifs;
extern txtoStream ofs;
extern txtoStream lfs;
extern txtoStream dfs;
extern int mangledNames;
extern int maxVL;
extern MachineReg regs[3072];
extern MachineReg vregs[3072];
extern int nregs;
extern int max_reg_in_use;
extern int syntax;

extern int64_t rodata_base_address;
extern int64_t bss_base_address;
extern int64_t data_base_address;

extern CSet string_exclude;

/*
extern FILE             *input,
                        *list,
                        *output;
*/
extern FILE *outputG;
extern int incldepth;
extern int              lineno;
extern int              nextlabel;
extern int              lastch;
extern int              lastst;
extern char             lastkw[128];
extern char             laststr[MAX_STLP1];
extern int laststrlen;
extern int64_t	ival;
extern double           rval;
extern Float128			rval128;
extern Int128  ival128;
extern Posit64 pval64;
extern char float_precision;
extern char int_precision;
extern int parseEsc;
//extern FloatTriple      FAC1,FAC2;

extern TABLE            gsyms[257],
                        lsyms;
extern TABLE            tagtable;
extern Symbol              *lasthead;
extern struct slit      *strtab;
extern Float128		    *quadtab;
extern int              lc_auto;
extern int				lc_thread;
extern Statement     *bodyptr;       /* parse tree for function */
extern int              global_flag;
extern TABLE            defsyms;
extern CSet *save_mask;      /* register save mask */
extern CSet *fpsave_mask;
extern CSet* psave_mask;
extern int				bsave_mask;
extern int uctran_off;
extern int isKernel;
extern int isPascal;
extern int isOscall;
extern int isInterrupt;
extern int isTask;
extern int isNocall;
extern bool isRegister;
extern int asmblock;
extern int optimize;
extern int opt_noregs;
extern int opt_nopeep;
extern int opt_noexpr;
extern int opt_nocgo;
extern int opt_size;
extern int opt_loop_invariant;
extern int opt_rv5;
extern int mixedSource;
extern Function *currentFn;
extern Symbol* currentSym;
extern int iflevel;
extern int foreverlevel;
extern int looplevel;
extern int loopexit;
extern short int loop_active;
extern int stmtdepth;
extern int regmask;
extern int bregmask;
extern Statement *currentStmt;
extern bool dogen;

extern TYP stdvoid;
extern TYP stdbit;
extern TYP stddecimal;
extern TYP stdint;
extern TYP stduint;
extern TYP stdlong;
extern TYP stdulong;
extern TYP stdshort;
extern TYP stdushort;
extern TYP stdchar;
extern TYP stduchar;
extern TYP stdichar;
extern TYP stdiuchar;
extern TYP stdbyte;
extern TYP stdubyte;
extern TYP stdstring;
extern TYP stdistring;
extern TYP stdastring;
extern TYP stddbl;
extern TYP stdtriple;
extern TYP stdflt;
extern TYP stdhalf;
extern TYP stdsingle;
extern TYP stddouble;
extern TYP stdposit;
extern TYP stdposit32;
extern TYP stdposit16;
extern TYP stdfunc;
extern TYP stdexception;
extern TYP stdconst;
extern TYP stdquad;
extern TYP stdvector;
extern TYP stdvectormask;
extern TYP stdenum;
extern TYP stdptr;

extern std::string *declid;
extern Compiler compiler;
extern int64_t genst_cumulative;

extern int throwlab;
extern int breaklab;
extern int contlab;
extern int retlab;
extern int lastsph;
extern char *semaphores[20];
extern unsigned int ArgRegCount;
extern bool isFuncBody;
extern bool isUnion;
extern int isUnsigned;
extern int isSigned;
extern int isVolatile;
extern int isVirtual;
extern bool isInline;
extern int isIO;
extern int isConst;
extern bool isRegister;
extern bool isAuto;
extern bool isFuncBody;
extern bool isFuncPtr;
extern char *stkname;
extern int64_t sp_init;
extern bool DoesContextSave;
extern int nparms;
extern int funcdecl;
extern Stringx names[20];
extern int isStructDecl;
extern int worstAlignment;
extern Map map;
extern int optimized;
extern short int typ_sp;
#ifdef STARK
extern starkCPU cpu;
extern starkCodeGenerator cg;
extern starkStatementGenerator sg;
#endif
#ifdef LB650
extern LB650CPU cpu;
extern LB650CodeGenerator cg;
extern LB650StatementGenerator sg;
#endif
#ifdef QUPLS
extern QuplsCPU cpu;
extern QuplsCodeGenerator cg;
extern QuplsStatementGenerator sg;
#endif
#ifdef QUPLS40
extern CPU cpu;
extern QuplsCodeGenerator cg;
extern QuplsStatementGenerator sg;
#endif
#ifdef THOR
extern CPU cpu;
extern ThorCodeGenerator cg;
extern ThorStatementGenerator sg;
#endif
#ifdef RISCV
extern CPU cpu;
extern RiscvCodeGenerator cg;
extern RiscvStatementGenerator sg;
#endif
#ifdef BIGFOOT
extern BigfootCPU cpu;
extern BigfootCodeGenerator cg;
extern BigfootStatementGenerator sg;
#endif
#ifdef I386
extern i386CPU cpu;
extern i386CodeGenerator cg;
extern i386StatementGenerator sg;
#endif
extern int DataLabels[65535];
extern ENODE* gNameRefNode;
extern TYP* typ_vector[100];
extern short int brace_level;

// Analyze.c
extern short int csendx;
extern int equalnode(ENODE *node1, ENODE *node2);
extern int bsort(CSE **list);
// CMain.c
extern void closefiles();

extern void error(int n);
extern void needpunc(enum e_sym p,int);
// Memmgt.c
extern void *allocx(int);
extern char *xalloc(int);
extern Symbol *allocSYM();
extern TYP *allocTYP();
extern Operand *allocOperand();
extern ENODE *allocEnode();
extern CSE *allocCSE();
extern void ReleaseGlobalMemory();
extern void ReleaseLocalMemory();

// NextToken.c
extern void initsym();
extern void NextToken(int typ=0);
extern int getch();
extern int my_isspace(char c);
extern void getbase(int64_t);
extern void SkipSpaces();

// Symbol.c
extern Symbol *gsearch(std::string na);
extern Symbol *search(std::string na,TABLE *thead);
extern void insert(Symbol* sp, TABLE *table);

extern char *my_strdup(char *);
// Decl.c
extern int imax(int i, int j);
extern TYP *maketype(int bt, int siz);
extern void initstack();
extern int getline(int listflag);
extern void compile();

// Init.c
extern void doinit(Symbol *sp, bool glbls);
// Func.c
extern Symbol *makeint(char *);
extern void funcbody(Symbol *sp);
// Expr.c
extern Symbol *makeStructPtr(std::string name);
extern ENODE *makenode(int nt, ENODE *v1, ENODE *v2);
extern ENODE *makeinode(int nt, int64_t v1);
extern ENODE *makesnode(int nt, std::string *v1, std::string *v2, int64_t i);
extern TYP *nameref(ENODE **node,int);
extern Operand *GenerateExpression(ENODE *node, int flags, int size);
extern int GetNaturalSize(ENODE *node);
extern TYP *asnop(ENODE **node);
extern TYP *NonCommaExpression(ENODE **);
// Optimize.c
extern void opt_const(ENODE **node);
// GenerateStatement.c
//extern void GenerateFunction(Statement *stmt);
extern void GenerateIntoff(Statement *stmt);
extern void GenerateInton(Statement *stmt);
extern void GenerateStop(Statement *stmt);
extern void gen_regrestore();
extern Operand *make_direct(int i);
extern Operand *makereg(int r);
extern Operand *makevreg(int r);
extern Operand *makefpreg(int t);
extern Operand *makebreg(int r);
extern Operand *makepred(int r);
extern int popcnt(int64_t m);
// Peepgen.c
extern void MarkRemove(OCODE *ip);
extern int PeepCount(OCODE *);
extern void flush_peep();
extern void GenerateZeradic(int op);
extern void GenerateMonadic(int op, int len, Operand *ap1);
extern void GenerateMonadicNT(int op, int len, Operand *ap1);
extern OCODE* GenerateDiadic(int op, int len, Operand *ap1, Operand *ap2);
extern void GenerateDiadicNT(int op, int len, Operand *ap1, Operand *ap2);
extern OCODE* GenerateTriadic(int op, int len, Operand *ap1, Operand *ap2, Operand *ap3);
extern void Generate4adic(int op, int len, Operand *ap1, Operand *ap2, Operand *ap3, Operand *ap4);
// Gencode.c
extern Operand *MakeDataLabel(int lab);
extern Operand *MakeCodeLabel(int lab);
extern Operand *MakeImmediate(int64_t i);
extern Operand *MakeIndirect(int i);
extern Operand *MakeDirect(ENODE *node);
extern void swap_nodes(ENODE *node);

// IdentifyKeyword.c
extern int IdentifyKeyword();
// Preproc.c
extern int preprocess();
// CodeGenerator.c
extern char *GetNamespace();
extern char nmspace[20][100];
extern void MakeLegalOperand(Operand *ap,int flags, int size);
// List.c
extern void ListTable(TABLE *t, int i);

extern void GenerateFunction(Symbol *sym);
extern void GenerateReturn(Statement *stmt);

extern Operand *GenerateBitfieldDereference(ENODE *node, int flags, int size);
extern Operand *GenerateBitfieldAssign(ENODE *node, int flags, int size);
extern void GenerateBitfieldInsert(Operand *ap1, Operand *ap2, int offset, int width);

// err.c
extern void fatal(char *str);

extern int max_reg_alloc_ptr;
extern int max_stack_use;
extern BasicBlock *basicBlocks[10000];
extern BasicBlock *sortedBlocks[10000];
extern Forest forest;
extern IGraph iGraph;
extern Instruction opl[410];
extern BasicBlock *LastBlock;
extern Instruction *GetInsn(int);
extern char inpline[100000];
extern bool ExpressionHasReference;
extern Object *_GarbageList;
extern TABLE* gCurrentTbl;
extern int window_pos;

extern txtoStream irfs;
extern CSet* ru, * rru;
extern OCODE* gHeadif;
extern std::map<int, std::string*> DataLabelMap;

extern int seg_aligned[20];

#endif
