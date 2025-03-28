#ifndef _TYPES_H
#define _TYPES_H

// ============================================================================
//        __
//   \\__/ o\    (C) 2012-2025 Robert Finch, Waterloo
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

class Operand;
class ENODE;
class CodeGenerator;
class Statement;
class StatementGenerator;
class BasicBlock;
class Instruction;
class Var;
class CSE;
class CSETable;
class Operand;
class Symbol;
class SymbolFactory;
class Function;
class OCODE;
class PeepList;
class Var;
class List;

extern void GenerateZeradic(int op);
extern void GenerateMonadic(int op, int len, Operand*);
extern OCODE* GenerateDiadic(int op, int len, Operand* ap1, Operand* ap2);
extern OCODE* GenerateTriadic(int op, int len, Operand* ap1, Operand* ap2, Operand* ap3);

typedef struct tagCase {
	bool first;
	bool done;
	int label;
	int64_t val;
	Statement* stmt;
} Case;

// 64 bytes
class Object
{
public:
	Object();
	int64_t magic;
	int size;
	//	__gc_skip skip1 {
	__int32 typenum;
	__int32 id;
	__int8 state;			// WHITE, GREY, or BLACK
	__int8 scavangeCount;
	__int8 owningMap;
	__int8 pad1;
	__int32 pad2;
	unsigned int usedInMap;
	//	};
	struct _tagObject* forwardingAddress;
	void (*finalizer)();
	int pad3;
};

class CompilerType
{
public:
	static CompilerType *alloc();
};

class MBlk
{
	static MBlk *first;
public:
	MBlk *next;
	static void ReleaseAll();
	static void *alloc(int sz);
};

struct slit {
    struct slit *next;
		struct slit *tail;
    int             label;
    char            *str;
		std::string sp;
		bool		isString;
		int8_t pass;
	char			*nmspace;
	Symbol* sym;
};

struct nlit {
	struct nlit* next;
	struct nlit* tail;
	int    label;
	int precision;
	Float128 f128;
	double f;
	Posit64 p;
	int typ;
	int8_t pass;
	char* nmspace;
};

struct scase {
	int label;
	int64_t val;
	int8_t pass;
};

struct clit {
  struct clit *next;
  int     label;
	int		num;
	int8_t pass;
  scase   *cases;
	char	*nmspace;
};

class ArplException
{
public:
	int errnum;
	int data;
	ArplException(int e, int d) { errnum = e; data = d; };
};


struct typ;
class Statement;

class TYP;
class TypeArray;

class Value : public CompilerType
{
public:
	Value* MakeNew();
	TYP* typ;
	union {
		int64_t i;
		uint64_t u;
		double f;
		uint16_t wa[8];
		char* s;
	} value;
	// The compiler does not support initialization of complex types in unions.
	Posit64 posit;
	Float128 f128;
	double f1, f2;
	std::string* sp;
	std::string* msp;
	std::string* udnm;			// undecorated name
};

class DerivedMethod
{
public:
  int typeno;
  DerivedMethod *next;
  std::string *name;
};

class MachineReg
{
public:
	int number;
	bool isConst;
	bool assigned;
	bool modified;
	bool sub;
	bool IsArg;
	bool IsColorable;
	bool isGP;
	bool isFP;
	bool isPosit;
	ENODE *offset;
	int64_t val;
	Int128 val128;
public:
	static bool IsCalleeSave(int regno);
	static bool IsArgReg(int regno);
	bool IsArgReg();
	bool IsPositReg();
	bool IsFloatReg();
	bool ContainsPositConst();
	static void MarkColorable();
};

class Factory : public CompilerType
{
};

// Class for representing tables. Small footprint.

class TABLE {
public:
	Symbol* headp, * tailp;
	Symbol* basep;
	Symbol* ownerp;
	static Symbol *match[100];
	static int matchno;
	TABLE();
	static void CopySymbolTable(TABLE *dst, TABLE *src);
	Symbol* insert(Symbol* sp);
	Symbol *Find(std::string na,bool opt);
	Symbol* Find(std::string na, bool opt, e_bt bt);
	int Find(std::string na);
	int Find(std::string na,__int16,TypeArray *typearray, bool exact);
	int FindRising(std::string na);
	Symbol** GetParameters();
	void Clear() { headp = nullptr; tailp = nullptr; basep = nullptr; };
	void CopyTo(TABLE *dst) {
		dst->headp = headp;
		dst->tailp = tailp;
		dst->basep = basep;
		dst->ownerp = ownerp;
	};
	void AddTo(TABLE* dst);
	void MoveTo(TABLE *dst) {
		CopyTo(dst);
		Clear();
	};
};

class PeepList
{
public:
	OCODE *head;
	OCODE *tail;
public:
	void Add(OCODE *cd);
	int Count(OCODE *pos);
	bool HasCall(OCODE *pos);
	static OCODE *FindLabel(int64_t i);
	void InsertBefore(OCODE *an, OCODE *cd);
	void InsertAfter(OCODE *an, OCODE *cd);
	void MarkAllKeep();
	void MarkAllKeep2();
	void RemoveCompilerHints();
	void RemoveCompilerHints2();
	void Remove(OCODE *ip);
	void Remove();
	void Remove2();
	void RemoveLinkUnlink();
	void RemoveGPLoad();
	void RemoveRegsave();
	void RemoveEnterLeave();
	void flush(txtoStream& tfs);
	void SetLabelReference();
	void EliminateUnreferencedLabels();
	OCODE* FindTarget(OCODE *ip, int reg, OCODE* eip = nullptr);
	bool UsesOnlyArgRegs() const;

	void Dump(char *msg);
	void Dump(const char* msg) {
		Dump((char*)msg);
	};
	BasicBlock *Blockize();
	int CountSPReferences();
	int CountBPReferences();
	int CountGPReferences();
	void RemoveStackAlloc();
	void RemoveStackCode();
	void RemoveReturnBlock();

	// Optimizations
	void OptTempRegs();
	void OptInstructions();
	void OptBranchToNext();
	void OptIncrBranch();
	void OptDoubleTargetRemoval();
	void OptConstReg();
	void OptLoopInvariants(OCODE *loophead);
	void OptIf(OCODE* headif);

	// Color Graphing
	void SetAllUncolored();
	void RemoveMoves();

	void loadHex(txtiStream& ifs);
	void storeHex(txtoStream& ofs);
};

// Class holding information about functions or methods. These fields were part
// of the Symbol class at one point, but moved to their own class to conserve
// storage space. Many symbols are not functions.

class Function
{
public:
	unsigned short int number;
	bool valid;
	bool IsPrototype;
	bool IsTask;
	bool IsInterrupt;
	bool DoesContextSave;
	bool IsNocall;							// has no calling conventions
	bool IsPascal;
	bool IsLeaf;
	bool IsFar;
	bool DoesThrow;
	bool doesJAL;
	bool UsesNew;
	bool UsesPredicate;					// deprecated
	bool IsVirtual;
	bool IsInline;
	bool IsUnknown;
	bool UsesTemps;							// uses temporary registers
	bool UsesStackParms;
	bool hasSPReferences;
	bool hasBPReferences;				// frame pointer references
	bool hasGPReferences;				// global pointer references
	bool has_rodata;
	bool has_data;
	bool has_bss;
	bool has_return_block;
	bool didRemoveReturnBlock;
	bool retGenerated;
	bool alloced;
	bool hasAutonew;
	bool alstk;									// stack space was allocated with link
	bool hasParameters;
	bool hasDefaultCatch;				// programmer coded a default catch
	bool IsCoroutine;
	bool Islocal;
	bool UsesLoopCounter;
	short inline_threshold;
	short depth;
	uint16_t NumRegisterVars;
	__int8 NumParms;						// 256 max parameters
	__int8 NumFixedAutoParms;
	__int8 numa;								// number of stack parameters (autos)
	int64_t stkspace;						// stack space used by function
	int64_t sp_init;						// initial SP for interrupt functions
	int64_t argbot;
	int64_t tempbot;
	int64_t regvarbot;
	int64_t arg_space;					// space used by pushed arguments.
	TABLE proto;								// Table holding protoype information
	TABLE params;
	Statement* prolog;					// Function prolog
	Statement* epilog;
	Statement* body;
	uint64_t stksize;
	CSETable *csetbl;
	Symbol *sym;								// Associated Symbol data
	Symbol *parms;					    // List of parameters associated with symbol
	Symbol *nextparm;
	DerivedMethod *derivitives;	
	int64_t int_save_mask;
	int8_t operating_mode;
	CSet *mask, *rmask;					// Register saved/restored masks
	CSet *fpmask, *fprmask;
	CSet* pmask, * prmask;
	CSet *vmask, *vrmask;
	BasicBlock *RootBlock;
	BasicBlock *LastBlock;
	BasicBlock *ReturnBlock;
	Var *varlist;
	PeepList pl;							// under construction
	OCODE* pEnter;						// place of ENTER instruction
	OCODE *spAdjust;					// place where sp adjustment takes place
	OCODE *rcode;
	int64_t defCatchLabel;
	int64_t tryCount;
	OCODE* defCatchLabelPatchPoint;
	char* outbuf;
public:
	Function();
	void RemoveDuplicates();
	int64_t GetTempBot() { return (tempbot); };
	int64_t GetTempTop() { return (argbot); };
	void CheckParameterListMatch(Function *s1, Function *s2);
	bool CheckSignatureMatch(Function *a, Function *b) const;
	TypeArray *GetParameterTypes();
	TypeArray *GetProtoTypes();
	void PrintParameterTypes();
	std::string *BuildSignature(int opt = 0);
	Function *FindExactMatch(int mm);
	static Function *FindExactMatch(int mm, std::string name, int rettype, TypeArray *typearray);
	bool HasRegisterParameters();
	bool ProtoTypesMatch(Function *sym);
	bool ProtoTypesMatch(TypeArray *typearray);
	bool ParameterTypesMatch(Function *sym);
	bool ParameterTypesMatch(TypeArray *typearray);
	int BPLAssignReg(Symbol* sp1, int reg, bool* noParmOffset);
	void BuildParameterList(int *num, int*numa, int* ellipos);
	void AddParameters(Symbol *list);
	void AddProto(Symbol *list);
	void AddProto(TypeArray *);
	void AddDerived();
	void DoFuncptrAssign(Function *);

	void CheckForUndefinedLabels();
	void Summary(Statement *);
	Statement *ParseBody();
	void Init();
	int Parse(bool local);
	void InsertMethod();
	void InsertAuto(Symbol* var);

	void SaveGPRegisterVars();
	void SaveFPRegisterVars();
	void SavePositRegisterVars();
	void SaveRegisterVars();
	void SaveRegisterArguments();
	int RestoreGPRegisterVars();
	int RestoreFPRegisterVars();
	int RestorePositRegisterVars();
	void RestoreRegisterVars();
	void RestoreRegisterArguments();
	void SaveTemporaries(int *sp, int *fsp, int* psp, int* vsp);
	void RestoreTemporaries(int sp, int fsp, int psp, int vsp);

	void UnlinkStack(int64_t amt);

	// Optimization
	void PeepOpt();
	void FlushPeep(txtoStream& tfs) { pl.flush(tfs); };

	// Code generation
	Operand *MakeDataLabel(int lab, int ndxreg);
	Operand *MakeCodeLabel(int lab);
	Operand *MakeStringAsNameConst(char *s, e_sg seg);
	Operand *MakeString(char *s);
	Operand *MakeImmediate(int64_t i);
	Operand *MakeIndirect(int i);
	Operand *MakeIndexed(int64_t o, int i);
	Operand *MakeDoubleIndexed(int i, int j, int scale);
	Operand *MakeDirect(ENODE *node);
	Operand *MakeIndexed(ENODE *node, int rg);
	void PatchEnter(CSet* rmask);

	void GenLoad(Operand *ap3, Operand *ap1, int ssize, int size);
	int64_t SizeofReturnBlock();
	OCODE* SetupReturnBlock();
	void GenerateLocalFunctions();
	void GenerateCoroutineData();
	void GenerateCoroutineEntry();
	void GenerateCoroutineExit();
	void GenerateName(bool force);
	void GenerateBody(bool force_inline);
	void Generate();
	void GenerateDefaultCatch();
	int CountBss(Statement* stmt);
	void DumpBss(Statement* stmt);

	void CreateVars();
	void ComputeLiveVars();
	void DumpLiveVars();

	void storeHex(txtoStream& ofs);
private:
	void StackGPRs();
};

// Class representing compiler symbols.

class Symbol : public CompilerType {
public:
	static int acnt;
	static Symbol* alloc();
	static Symbol* initsym;		// symbol being initialized.
	static int initlvl;
public:
	bool valid;
	int number;
	short depth;
	int id;
	int parent;
	Symbol* parentp;
	int next;
	Symbol* nextp;
	std::string *name;
	std::string *shortname;
	std::string *mangledName;
	char nameext[4];
	char *realname;
	char *stkname;
	e_sc storage_class;
	e_sg segment;
	unsigned int IsInline : 1;
	unsigned int pos : 4;			// position of the symbol (param, auto or return type)
	// Function attributes
	Function *fi;
	// Auto's are handled by compound statements
	TABLE lsyms;              // local symbols (goto labels)
	bool IsParameter;
	bool IsRegister;
	bool IsAuto;
	bool isConst;
	bool IsKernel;
	bool IsPrivate;
	bool IsUndefined;					// undefined function
	bool IsExternal;
	bool ctor;
	bool dtor;
	ENODE *initexp;
	__int16 reg;
	ENODE* defval;	// default value
	int16_t parmno;	// parameter number
	union {
        int64_t i;
        uint64_t u;
        double f;
        uint16_t wa[8];
        char *s;
    } value;
	ENODE* enode;
	Posit64 p;
	Float128 f128;
	TYP *tp;
  Statement *stmt;
	std::streampos storage_pos;
	std::streampos storage_endpos;
	std::string bss_string;
	std::string data_string;

	Function* MakeFunction(int symnum, bool isPascal);
	bool IsTypedef();
	static Symbol *Copy(Symbol *src);
	Symbol *Find(std::string name);
	int FindNextExactMatch(int startpos, TypeArray *);
	Symbol *FindRisingMatch(bool ignore = false);
	Symbol* FindInUnion(std::string nme);
	static Symbol* GetTemp();
	std::string *GetNameHash();
	std::string* GetFullName();
	static std::string* GetFullNameByFunc(std::string nm);
	std::string *BuildSignature(int opt);
	static Symbol *GetPtr(int n);
	Symbol *GetParentPtr();
	void SetName(std::string nm) {
		if (name)
			delete name;
    name = new std::string(nm);
		if (mangledName == nullptr)
			mangledName = new std::string(nm);
	};
	void SetNext(int nxt) { next = nxt; };
	int GetNext() { return next; };
	Symbol *GetNextPtr();
	int GetIndex();
	void SetType(TYP *t) { 
		if (t == (TYP *)0x500000005) {
			printf("Press key\n");
			getchar();
	}
	else
		tp = t;
} ;
	void SetStorageOffset(TYP *head, int nbytes, int al, int ilc, int ztype);
	int64_t AdjustNbytes(int64_t nbytes, int al, int ztype);
	int64_t Initialize(txtoStream& tfs, ENODE* pnode, TYP* tp2, int opt);
	int64_t InitializeArray(txtoStream& tfs, ENODE*, TYP*);
	int64_t InitializeCharArray(txtoStream& tfs, ENODE*, TYP*);
	int64_t InitializeStruct(txtoStream& tfs, ENODE*, TYP*);
	int64_t InitializePointerToStruct(txtoStream& tfs, ENODE*, TYP*);
	int64_t InitializeUnion(txtoStream& tfs, ENODE*, TYP*);
	int64_t InitializePointerToUnion(txtoStream& tfs, ENODE*, TYP*);
	int64_t GenerateT(txtoStream& tfs, ENODE* node, TYP* tp);
	void storeHex(txtoStream& ofs);
};

class SymbolFactory : public CompilerType
{
public:
	static Symbol* Make(std::string name, TYP* tp, Symbol* parent, int depth, e_sc storage_class);
};

// Class representing compiler types.

class TYP {
public:
  e_bt type;
	__int16 typeno;			// number of the type
	bool val_flag;       /* is it a value type */
	bool isArray;
	bool isUnsigned;
	bool isShort;
	bool isVolatile;
	bool isIO;
	bool isResv;
	bool isBits;
	bool isDecimal;
	bool unknown_size;	// true if the array size is unknown
	__int16 precision;			// precision of the numeric in bits
	__int16 lane_precision;	// precision for one lane of a vector type
	ENODE* bit_width;
	ENODE* bit_offset;
	int8_t		ven;			// vector element number
	int64_t size;
	int64_t align;
	int64_t struct_offset;
	int8_t dimen;
	int64_t numele;					// number of elements in array / vector length
	TABLE lst;
	TYP* btpp;
	std::string* rd_cache;	// cache-ability specifiers
	std::string* wr_cache;

	static TYP *GetPtr(int n);
	int64_t GetIndex();
	int GetHash();
	static int64_t GetSize(int num);
	int64_t GetElementSize();
	static int GetBasicType(int num);
	std::string *sname;
	unsigned int alignment;
	static TYP *Make(e_bt bt, int64_t siz);
	static TYP *Copy(TYP *src);
	bool IsScalar();
	static bool IsScalar(e_sym kw);
	bool IsCharType() const {
		if (this == nullptr)
			return (false);
		return (type == bt_char || type == bt_uchar || type == bt_ichar || type == bt_iuchar);
	};
	bool IsFloatType() const { 
		if (this == nullptr)
			return (false);
		return (type==bt_quad || type==bt_float || type==bt_double || type==bt_single || type==bt_half); };
	static const bool IsFloatType(int t) {
		return (t == bt_quad || t == bt_float || t == bt_double || t == bt_single || t == bt_half);
	};
	bool IsPositType() const {
		if (this == nullptr)
			return (false);
		return (type == bt_posit); };
	int IsIntrinsicType()
	{
		return  
			type == bt_byte || type == bt_char || type == bt_short || type == bt_int || type == bt_void ||
			type == bt_ubyte || type == bt_uchar || type == bt_ushort || type == bt_uint || type == bt_void ||
			type == bt_long || type == bt_float || type == bt_double || type == bt_quad || type == bt_ulong ||
			type == bt_enum || type == bt_struct || type == bt_union || type == bt_bool ||
			type == bt_ichar || type == bt_iuchar || type == bt_decimal ||
			type == bt_unsigned || type == bt_exception
			;
	};
	static int IsIntrinsicType(e_bt st)
	{
		return
			st == bt_byte || st == bt_char || st == bt_short || st == bt_int || st == bt_void ||
			st == bt_ubyte || st == bt_uchar || st == bt_ushort || st == bt_uint || st == bt_void ||
			st == bt_long || st == bt_float || st == bt_double || st == bt_quad || st == bt_ulong ||
			st == bt_enum || st == bt_struct || st == bt_union || st == bt_bool ||
			st == bt_ichar || st == bt_iuchar || st == bt_decimal ||
			st == bt_unsigned || st == bt_exception || st==bt_bitfield
			;
	};
	bool IsIntegralType() {
		return
			type == bt_byte || type == bt_char || type == bt_short || type == bt_int ||
			type == bt_ubyte || type == bt_uchar || type == bt_ushort || type == bt_uint ||
			type == bt_long || type == bt_ulong ||
			type == bt_enum || type == bt_bool ||
			type == bt_ichar || type == bt_iuchar ||
			type == bt_unsigned || type == bt_exception || type == bt_bitfield
			;
	};
	bool IsFunc() const { if (this == nullptr) return (false); return (type == bt_func || type == bt_ifunc); };
	bool IsVectorType() const { if (this == nullptr) return (false);  return (type == bt_vector); };
	bool IsUnion() const { if (this == nullptr) return (false); return (type == bt_union); };
	bool IsStructType() const { if (this == nullptr) return false; return (type == bt_struct || type == bt_class || type == bt_union); };
	bool IsArrayType() const { return (type == bt_array); };
	bool IsAggregateType() const { if (this == nullptr) return (false);  return (IsStructType() | isArray | IsArrayType()); };
	static bool IsSameType(TYP *a, TYP *b, bool exact);
	static bool IsSameStructType(TYP* a, TYP* b);
	static bool IsSameUnionType(TYP* a, TYP* b);
	void put_ty();

	int64_t Alignment();
	int64_t walignment();
	int64_t roundAlignment();
	int64_t roundSize();

	ENODE *BuildEnodeTree();

	// Initialization
	int64_t GenerateT(txtoStream& tfs, ENODE *node);
	int64_t InitializeArray(txtoStream& tfs, int64_t sz, Symbol* symi);
	int64_t InitializeStruct(txtoStream& tfs, ENODE*, Symbol* symi);
	int64_t InitializeUnion(txtoStream& tfs, Symbol* symi, ENODE* node);
	int64_t Initialize(txtoStream& tfs, int64_t val, Symbol* symi);
	int64_t Initialize(txtoStream& tfs, ENODE* node, TYP *, int opt, Symbol* symi);

	// Serialization
	char* ToString(int ndx = 0);
	void storeHex(txtoStream& ofs);

	// GC support
	bool FindPointer();
	bool FindPointerInStruct();
	bool IsSkippable();
};

class TypeArray
{
public:
	e_bt types[40];
	__int16 preg[40];
	int length;
	TypeArray();
	void Add(e_bt tp, __int16 regno);
	void Add(TYP *tp, __int16 regno);
	bool IsEmpty();
	bool IsEqual(TypeArray *);
	bool IsLong(e_bt);
	bool IsShort(e_bt);
	bool IsInt(e_bt);
	bool IsChar(e_bt);
	bool IsByte(e_bt);
	bool IsIntType(e_bt);
	void Clear();
	TypeArray *Alloc();
	void Print(txtoStream *);
	void Print();
	std::string *BuildSignature();
};

class ENODE {
public:
	static int segcount[16];
	static CSet initializedSet;
public:
	int number;										// number of this node for reference
	int64_t order;								// list ordering for initializers
	enum e_node nodetype;
	enum e_node new_nodetype;			// nodetype replaced by optimization

	int etype;
	TYP* etypep;
	int64_t esize;
	TYP* tp;
	Symbol* sym;									// pointer to symbol referenced by node
	bool constflag;								// the node contains a constant value
	unsigned int segment : 4;
	unsigned int predreg : 6;
	bool isVolatile;
	bool isIO;
	bool isUnsigned;
	bool isCheckExpr;
	bool isPascal;
	bool isAutonew;
	bool isNeg;
	bool argref;									// argument reference
	ENODE* vmask;
	ENODE* bit_width;
	ENODE* bit_offset;
	__int8 scale;
	short int rg;
	int64_t stack_offs;
	// The following could be in a value union
	// Under construction: use value class
//	Value val;
	// The value information is represented directly in the class for several
	// classes for convenience in referencing.
	int64_t i;
	int64_t i_lhs;					// i of node being assign to
	int64_t i_rhs;					// i of node being assigned from
	double f;
	double f1, f2;
	Int128 i128;
	Float128 f128;
	Posit64 posit;
	std::string* sp;
	std::string* msp;
	std::string* udnm;			// undecorated name
	void* ctor;
	void* dtor;
	ENODE* p[4];
	ENODE* pfl;			// postfix list
	int8_t mask;						// vector mask register
	e_sc sc;				// storage class

	ENODE* Clone();

	bool IsLValue();
	void SetType(TYP* t) { 
		if (t == (TYP*)1)
			printf("hello");
		if (this) { tp = t; if (t) etype = t->type; } };
	bool IsPtr() { return (etype == bt_pointer || etype == bt_struct || etype == bt_union || etype == bt_class || nodetype == en_addrof); };
	bool IsFloatType() {
		if (nodetype == en_addrof || nodetype == en_autofcon)
			return (false);
		if (etype == bt_double || etype == bt_quad || etype == bt_float)
			return (true);
		if (etype == bt_vector)
			return (tp->btpp->IsFloatType());
		return (false);
	};
	bool IsPositType() {
		return (nodetype == en_addrof || nodetype == en_autopcon) ? false : (etype == bt_posit);
	};
	bool IsLogic() {
		return (nodetype == en_lor 
			|| nodetype == en_land 
			|| nodetype == en_land_safe
			|| nodetype==en_or
			|| nodetype==en_lor_safe
			|| nodetype==en_and);
	}
	bool IsVectorType() {
		return (etype == bt_vector); };
	bool IsAutocon() { return (nodetype == en_autocon || nodetype == en_autofcon || nodetype == en_autopcon || nodetype == en_autovcon || nodetype == en_classcon); };
	bool IsUnsignedType() { return (etype == bt_ubyte || etype == bt_uchar || etype == bt_ushort || etype == bt_ulong || etype == bt_pointer || nodetype==en_addrof || nodetype==en_autofcon || nodetype==en_autocon); };
	// ??? Use of this method is dubious
	bool IsRefType() {
		if (this) {
			// This hack to get exit() to work, which uses an array of function pointers.
			if (nodetype == en_ref) {
				if (tp)
					return (tp->type != bt_void && p[0]->nodetype != en_regvar);
				else
					return (true);
			}
			return (nodetype == en_ref || etype == bt_struct || etype == bt_union || etype == bt_class);
		}
		else return (false);
	};
	bool IsBitfield();
	static bool IsEqualOperand(Operand *a, Operand *b);
	char fsize();
	int64_t GetReferenceSize();
	int64_t GetNaturalSize();

	static bool IsSameType(ENODE *ep1, ENODE *ep2);
	static bool IsEqual(ENODE *a, ENODE *b, bool lit = false);
	bool HasAssignop();
	bool HasCall();

	// Parsing
	void AddToList(ENODE* ele);
	bool AssignTypeToList(TYP *);

	// Optimization
	CSE *OptInsertAutocon(int duse);
	CSE *OptInsertRef(int duse);
	void scanexpr(int duse);
	void repexpr();
	void update();

	// Code generation
	List* ReverseList(ENODE*);
	bool FindLoopVar(int64_t);

	Operand *MakeDataLabel(int lab, int ndxreg);
	Operand *MakeCodeLabel(int lab);
	Operand *MakeStringAsNameConst(char *s, e_sg seg);
	Operand *MakeString(char *s);
	Operand *MakeImmediate(int64_t i);
	Operand *MakeIndirect(int i);
	Operand *MakeIndexed(int64_t o, int i);
	Operand *MakeDoubleIndexed(int i, int j, int scale);
	Operand *MakeDirect(ENODE *node);
	Operand *MakeIndexed(ENODE *node, int rg);

	void GenerateHint(int num);
	void GenMemop(int op, Operand *ap1, Operand *ap2, int64_t ssize, int typ);
	void GenerateLoad(Operand *ap3, Operand *ap1, int64_t ssize, int64_t size);
	void GenStore(Operand *ap1, Operand *ap3, int64_t size);
	static void GenRedor(Operand *ap1, Operand *ap2);
	Operand *GenerateIndex(bool neg);
	Operand* GenerateRegRegIndex();
	Operand* GenerateImmExprIndex(Operand* ap1, bool neg);
	Operand* GenerateRegImmIndex(Operand* ap1, Operand* ap2, bool neg);
	Operand *GenerateSafeHook(int flags, int64_t size);
	Operand *GenerateShift(int flags, int64_t size, int op);
	Operand *GenerateMultiply(int flags, int64_t size, int op);
	Operand *GenerateDivMod(int flags, int64_t size, int op);
	Operand *GenerateUnary(int flags, int64_t size, int op);
	Operand *GenerateBinary(int flags, int64_t size, int op);
	Operand *GenerateAssignShift(int flags, int64_t size, int op);
	Operand *GenerateAssignAdd(int flags, int64_t size, int op);
	Operand *GenerateAssignLogic(int flags, int64_t size, int op);
	Operand *GenerateLand(int flags, int op, bool safe);
	Operand* GenerateBitfieldDereference(int flags, int64_t size, int opt);
	void GenerateBitfieldInsert(Operand* ap1, Operand* ap2, int offset, int width);
	void GenerateBitfieldInsert(Operand* ap1, Operand* ap2, Operand* offset, Operand* width);
	void GenerateBitfieldInsert(Operand* ap1, Operand* ap2, ENODE* offset, ENODE* width);
	Operand* GenerateBitfieldAssign(int flags, int64_t size);
	Operand* GenerateBitfieldAssignAdd(int flags, int64_t size, int op);
	Operand* GenerateBitfieldAssignLogic(int flags, int64_t size, int op);
	Operand* GenerateScaledIndexing(int flags, int64_t size, int rhs);

	// Serialization
	void store(txtoStream& ofs);
	void load(txtiStream& ifs);
	int load(char* bufptr);
	void storeHex(txtoStream& ofs);
	void loadHex(txtiStream& ifs);

	std::string* GetLabconLabel(int64_t ii);
	int64_t PutStructConst(txtoStream& ofs);
	void PutConstant(txtoStream& ofs, unsigned int lowhigh, unsigned int rshift, bool opt = false, int display_opt = 0);
	void PutConstantHex(txtoStream& ofs, unsigned int lowhigh, unsigned int rshift);
	static ENODE *GetConstantHex(std::ifstream& ifs);
	void GenerateHalf(txtoStream& tfs) {};
	void GenerateInt(txtoStream& tfs);
	void GenerateShort(txtoStream& tfs);
	void GenerateLong(txtoStream& tfs);

	// Utility
	void ResetSegmentCount() { ZeroMemory(&segcount, sizeof(segcount)); };
	void CountSegments();

	// Debugging
	std::string nodetypeStr();
	void Dump(int pn = 0);
	void DumpAggregate();
};

// Class to allow representing a set of expression nodes as a linear list
// rather than as a tree. Useful for initializations.

class List
{
public:
	static int64_t numele;
public:
	List() {
		nxt = nullptr;
		node = nullptr;
	};
	List(ENODE *nd) {
		nxt = nullptr;
		node = nd;
	};
	List* nxt;
	ENODE* node;
};

// Under construction
class INODE : public CompilerType
{
public:
	INODE* next;
	INODE* prev;
	INODE* inner;
	INODE* outer;
	int type;
	int64_t size;
	// value
	void* arry;
	int64_t i;
	double f;
	Float128 f128;
	Posit64 posit;
	std::string* str;
};

class ExpressionFactory : public Factory
{
public:
	ENODE* Makenode(int nt, ENODE* v1, ENODE* v2, ENODE* v3, ENODE* v4);
	ENODE* Makenode(int nt, ENODE* v1, ENODE* v2, ENODE* v3);
	ENODE* Makenode(int nt, ENODE* v1, ENODE* v2);
	ENODE* Makefqnode(int nt, Float128 v1);
	ENODE* Makefnode(int nt, double v1);
	ENODE* Makepnode(int nt, Posit64 v1);
	ENODE* Makenode();
	ENODE* MakePositNode(int nt, Posit64 v1);
};

class Expression : public CompilerType
{
private:
	int cnt;				// number of []
	ENODE* pep1;
	int numdimen;
	int64_t sa[10];	// size array - 10 dimensions max
	int64_t totsz;
public:
	bool isMember;
	TYP* head;
	TYP* tail;
	TYP* LHSType;
	int sizeof_flag;
	bool got_pa;
	int parsingAggregate;
	Statement* owning_stmt;
	Function* owning_func;
private:
	void SetRefType(ENODE** node);
	void GetSizesForArray(TYP* typ);
	ENODE* SetIntConstSize(TYP* tptr, int64_t val);
	ENODE *ParseArgumentList(ENODE *hidden, TypeArray *typearray, Symbol* symi);
	TYP* ParseCharConst(ENODE** node, int sz);
	TYP* ParseStringConst(ENODE** node, Symbol* sym);
	ENODE* ParseStringConstWithSizePrefix(ENODE** node, Symbol* sym);
	ENODE* ParseInlineStringConst(ENODE** node, Symbol* sym);
	TYP* ParseRealConst(ENODE** node);
	TYP* ParsePositConst(ENODE** node);
	ENODE* ParseAggregateConst(ENODE** node);
	ENODE* ParseAggregateStringConst(ENODE** node, Symbol* sym);
	TYP* ParseFloatMax(ENODE** node);
	ENODE* ParseThis(ENODE** node);
	void ParseAggregateHelper(ENODE** node, ENODE* cnode);
	bool ParseAggregateStruct(ENODE** node, ENODE* cnode, Symbol* symi, TYP* tp);
	void ParseAggregateArray(ENODE** node, ENODE* cnode, Symbol* symi, TYP* tp);
	TYP* ParseAggregate(ENODE** node, Symbol* typi, TYP* tp);
	bool ParseGenericCase(ENODE** node, TYP* tp1, Symbol* symi, int count, 
		short int* defcount, ENODE** ep_def, TYP** head, TYP** tp2, ENODE** ep4);
	TYP* ParseGenericSwitch(ENODE** node, Symbol* symi);
	TYP* ParseSwitchExpr(ENODE** node, Symbol* symi);
	TYP* ParseSwitch(ENODE** node, Symbol* symi);
	TYP* ParseGenericCase(ENODE** node, Symbol* symi, TYP* tp1);
	TYP* ParseGeneric(ENODE** node, Symbol* symi);
	TYP* ParseVm(ENODE** node, Symbol* symi, int vmn);
	ENODE* ParseTypenum();
	ENODE* ParseNew(bool autonew, Symbol* symi);
	ENODE* ParseDelete(Symbol* symi);
	ENODE* ParseAddressOf(Symbol* symi);
	ENODE* ParseMulf(Symbol* symi);
	ENODE* ParseBytndx(Symbol* symi);
	ENODE* ParseWydndx(Symbol* symi);
	ENODE* ParseBmap(Symbol* symi);
	ENODE* ParseSaveContext(Symbol* symi);
	ENODE* ParseSync(Symbol* symi);
	// Unary Expression Parsing
	TYP* ParseMinus(ENODE** node, Symbol* symi);
	TYP* ParseNot(ENODE** node, Symbol* symi);
	ENODE* ParseCom(Symbol* symi);
	TYP* ParseStar(ENODE** node, Symbol* symi);
	ENODE* ParseSizeof(Symbol* symi);
	ENODE* ParseAlignof(Symbol* symi);

	ENODE* ParseDotOperator(TYP* tp1, ENODE* parent, Symbol* symi);
	ENODE* ParsePointsTo(TYP* tp1, ENODE* ep1);
	ENODE* ParseOpenpa(TYP* tp1, ENODE* ep1, Symbol* symi);
	ENODE* ParseOpenbr(TYP*tp1, ENODE* ep1);
	ENODE* AdjustForBitArray(int pop, TYP* tp1, ENODE* ep1);
	ENODE* ParseBitfieldSpec(TYP* typ, ENODE* fldspec, ENODE* var);

	void ApplyVMask(ENODE* node, ENODE* mask);

	TYP* AddRef(ENODE** node, TYP* tp);

	TYP *ParsePrimaryExpression(ENODE **node, int got_pa, Symbol* symi);
	TYP *ParseCastExpression(ENODE **node, Symbol* symi);
	TYP *ParseMultOps(ENODE **node, Symbol* symi);
	TYP *ParseAddOps(ENODE **node, Symbol* symi);
	TYP *ParseShiftOps(ENODE **node, Symbol* symi);
	TYP *ParseRelationalOps(ENODE **node, Symbol* symi);
	TYP *ParseEqualOps(ENODE **node, Symbol* symi);
	TYP *ParseBitwiseAndOps(ENODE **node, Symbol* symi);
	TYP *ParseBitwiseXorOps(ENODE **node, Symbol* symi);
	TYP *ParseBitwiseOrOps(ENODE **node, Symbol* symi);
	TYP *ParseAndOps(ENODE **node, Symbol* symi);
	TYP *ParseSafeAndOps(ENODE **node, Symbol* symi);
	TYP *ParseOrOps(ENODE **node, Symbol* symi);
	TYP *ParseSafeOrOps(ENODE **node, Symbol* symi);
	TYP* ParseMux(ENODE** node, Symbol* symi);
	TYP *ParseConditionalOps(ENODE **node, Symbol* symi);
	TYP *ParseCommaOp(ENODE **node, Symbol* symi);
	ENODE* MakeNameNode(Symbol* sym);
	ENODE* MakeStaticNameNode(Symbol* sym);
	ENODE* MakeThreadNameNode(Symbol* sp);
	ENODE* MakeExternNameNode(Symbol* sp);
	ENODE* MakeConstNameNode(Symbol* sp);
	ENODE* MakeMemberNameNode(Symbol* sp);
	ENODE* MakeUnknownFunctionNameNode(std::string nm, TYP** tp, TypeArray* typearray, ENODE* args);
	TYP* RefBit(ENODE** node, TYP* tp);
	TYP* RefByte(ENODE** node, TYP* tp);
	TYP* RefUnsignedByte(ENODE** node, TYP* tp);
	TYP* RefChar(ENODE** node, TYP* tp);
	TYP* RefUnsignedChar(ENODE** node, TYP* tp);
	TYP* RefIChar(ENODE** node, TYP* tp);
	TYP* RefUnsignedIChar(ENODE** node, TYP* tp);
	TYP* RefEnum(ENODE** node, TYP* tp);
	TYP* RefShort(ENODE** node, TYP* tp);
	TYP* RefUnsignedShort(ENODE** node, TYP* tp);
	TYP* RefInt(ENODE** node, TYP* tp);
	TYP* RefUnsignedInt(ENODE** node, TYP* tp);
	TYP* RefLong(ENODE** node, TYP* tp);
	TYP* RefUnsignedLong(ENODE** node, TYP* tp);
	TYP* RefException(ENODE** node, TYP* tp);
	TYP* RefPointer(ENODE** node, TYP* tp);
	TYP* RefFloat(ENODE** node, TYP* tp);
	TYP* RefDouble(ENODE** node, TYP* tp);
	TYP* RefQuad(ENODE** node, TYP* tp);
	TYP* RefPosit(ENODE** node, TYP* tp);
	TYP* RefBitfield(ENODE** node, TYP* tp);
	TYP* RefVector(ENODE** node, TYP* tp);
	TYP* RefVectorMask(ENODE** node, TYP* tp);
	TYP* RefVoid(ENODE** node, TYP* tp);
	ENODE* FindLastMulu(ENODE*);
public:
	Expression();
	Expression(Statement* type) {
		owning_stmt = type;
	};
	ENODE* MakeGlobalNameNode(Symbol* sp);
	Symbol* gsearch2(std::string na, __int16 rettype, TypeArray* typearray, bool exact);
	TYP* ParseNameRef(ENODE** node, Symbol* symi, int nt = 1);
	TYP* ParseUnaryExpression(ENODE** node, int got_pa, Symbol* symi);
	TYP* CondAddRef(ENODE** node, TYP* tp);
	ENODE* MakeAutoNameNode(Symbol* sp);
	TYP* nameref(ENODE** node, int nt, Symbol* symi);
	TYP* nameref2(std::string name, ENODE** node, int nt, bool alloc, TypeArray* typearray, TABLE* tbl, Symbol* symi);
	// The following is called from declaration processing, so is made public
	TYP *ParseAssignOps(ENODE **node, Symbol* symi);
	TYP* ParsePostfixExpression(ENODE** node, int got_pa, Symbol* symi);
	TYP *ParseNonCommaExpression(ENODE **node, Symbol* symi);
	TYP* ParseNonAssignExpression(ENODE** node, Symbol* symi);
	//static TYP *ParseBinaryOps(ENODE **node, TYP *(*xfunc)(ENODE **), int nt, int sy);
	TYP *ParseExpression(ENODE **node, Symbol* symi);
	Function* MakeFunction(int symnum, Symbol* sp, bool isPascal);
	Symbol* FindMember(TYP* tp1, char* name);
	Symbol* FindMember(TABLE* tbl, char* name);
	void force_type(ENODE* dst, ENODE* src, TYP* tp);
	static ENODE* Autoincdec(TYP* tp, ENODE** node, int flag, bool isPostfix);
};


class Operand : public CompilerType
{
public:
	int num;					// number of the operand
	unsigned int mode;
	unsigned int preg : 32;		// primary virtual register number
	unsigned int sreg : 32;		// secondary virtual register number (indexed addressing modes)
	bool pcolored;
	bool scolored;
	unsigned short int pregs;	// subscripted register number
	unsigned short int sregs;
	unsigned int segment : 4;
	unsigned int defseg : 1;
	bool is_scaled;
	bool tempflag;
	bool memref;
	bool argref;							// refers to a function argument
	bool preserveNextReg;
	unsigned int type : 32;
	TYP* typep;
	TYP* tp;
	char FloatSize;
	bool isUnsigned;
	unsigned int lowhigh : 2;
	bool isVolatile;
	bool isPascal;
	unsigned int rshift : 8;
	bool isPtr;
	bool isConst;
	bool isBool;
	bool rhs;
	short int pdeep;		// previous stack depth on allocation
	short int deep;           /* stack depth on allocation */
	short int deep2;
	ENODE *offset;
	ENODE *offset2;
	ENODE* bit_offset;
	ENODE* bit_width;
	int8_t scale;
	Operand *next;			// For extended sizes (long)
	Operand* memop;
	Operand* toRelease;
	int display_opt;
public:
	Operand *Clone();
	static bool IsSameType(Operand *ap1, Operand *ap2);
	static bool IsEqual(Operand *ap1, Operand *ap2);
	bool IsReg() { return (mode == am_reg); };
	bool IsCr() { return (mode == amCrReg); };
	bool IsMem() {
		return (
			mode == am_direct ||
			mode == am_ind ||
			mode == am_indx ||
			mode == am_indx2
			);
	};
	char fpsize();

	Operand* GenerateBitfieldClear(int startpos, int width);
	Operand* GenerateZeroExtend(int startpos, int width);
	Operand *GenerateSignExtend(int64_t isize, int64_t osize, int flags);
	void MakeLegalReg(int flags, int64_t size);
	void MakeLegalCrReg(int flags, int64_t size);
	void MakeLegal(int flags, int64_t size);
	int OptRegConst(int regclass, bool tally=false);
	bool GetConstValue(Int128* pval);

	// Storage
	void PutAddressMode(txtoStream& ofs);
	void store(txtoStream& fp);
	void storeHex(txtoStream& fp);
	static Operand *loadHex(txtiStream& fp);
	void load(txtiStream& fp);
};

// Output code structure

class OCODE : public CompilerType
{
public:
	OCODE *fwd, *back, *comment;
	BasicBlock *bb;
	Instruction *insn;
	Instruction* insn2;
	short opcode;
	short length;
	std::string* ext;
	unsigned int segment : 4;
	unsigned int isVolatile : 1;
	unsigned int isReferenced : 1;	// label is referenced by code
	unsigned int remove : 1;
	unsigned int remove2 : 1;
	unsigned int leader : 1;
	unsigned int str : 1;
	short pregreg;
	short predop;
	int loop_depth;
	Operand *oper1, *oper2, *oper3, *oper4;
	__int16 phiops[100];
public:
	static OCODE *MakeNew();
	static OCODE *Clone(OCODE *p);
	static bool IsEqualOperand(Operand *a, Operand *b) { return (Operand::IsEqual(a, b)); };
	static void Swap(OCODE *ip1, OCODE *ip2);
	void MarkRemove() { 
		remove = true;
	};
	void MarkRemove2() { remove2 = true; };
	void Remove();
	bool HasTargetReg() const;
	bool HasTargetReg(int regno) const;
	int GetTargetReg(int *rg1, int *rg2) const;
	bool HasSourceReg(int) const;
	bool IsLoad() const;
	bool IsStore() const;
	//Edge *MakeEdge(OCODE *ip1, OCODE *ip2);
	// Optimizations
	bool IsSubiSP();
	void OptCom();
	void OptMul();
	void OptMulu();
	void OptDiv();
	void OptAnd();
	void OptMove();
	void OptRedor();
	void OptAdd();
	void OptSubtract();
	void OptLoad();
	void OptLoadByte();
	void OptLoadChar();
	void OptLoadHalf();
	void OptStoreHalf();
	void OptLoadWord();
	void OptNoUse();
	void OptDefUse();
	void OptStore();
	void OptSxb();
	void OptBra();
	void OptJAL();
	void OptUctran();
	void OptDoubleTargetRemoval();
	void OptHint();
	void OptLabel();
	void OptIndexScale();
	void OptLdi();
	void OptLea();
	void OptPush();
	void OptPop();
	void OptBne();
	void OptBeq();
	void OptIncrBranch();
	void OptScc();
	void OptSll();
	void OptSxw();
	void OptSxo() {};
	void OptVmask();
	void OptZxb();
	void OptZxw();
	int TargetDistance(int64_t i);

	static OCODE *loadHex(txtiStream& ifs);
	void store(txtoStream& ofs);
	void storeHex(txtoStream& ofs);
};

class OperandFactory : public Factory
{
public:
	Operand *MakeDataLabel(int64_t labno, int ndxreg);
	Operand *MakeCodeLabel(int64_t lab);
	Operand *MakeStrlab(std::string s, e_sg seg);
	Operand *MakeString(char *s);
	Operand *MakeStringAsNameConst(char *s, e_sg seg);
	Operand *makereg(int r);
	Operand *makecreg(int r);
	Operand* makeCrReg(int r);
	Operand* makeCrgReg(int r);
	Operand *makevreg(int r);
	Operand *makevmreg(int r);
	Operand *makefpreg(int r);
	Operand* makepreg(int r);
	Operand *MakeMask(int mask);
	Operand *MakeImmediate(int64_t i, int display_opt=0);
	Operand* MakeImmediate(Int128 i, int display_opt = 0);
	Operand* MakeMemoryIndirect(int disp, int regno);
	Operand *MakeIndirect(short int regno);
	Operand *MakeIndexedCodeLabel(int lab, int i);
	Operand* MakeIndexedDataLabel(int lab, int i);
	Operand *MakeIndexed(int64_t offset, int regno);
	Operand *MakeIndexed(ENODE *node, int regno);
	Operand *MakeNegIndexed(ENODE *node, int regno);
	Operand *MakeDoubleIndexed(int regi, int regj, int scale);
	Operand *MakeDirect(ENODE *node);
	Operand* MakeIndexedName(std::string nme, int i);
};

class FunctionFactory : public Factory
{
public:
	Function* MakeFunction(int symnum, Symbol* sp, bool isPascal);
};

class CodeGenerator
{
public:
	Statement* stmt;
public:
	virtual void banner() {};
	virtual Operand* GetTempRegister();
	virtual Operand* GetTempFPRegister();
	bool IsPascal(ENODE* ep);
	int64_t GetSavedRegisterList(CSet* rmask);
	Operand* MakeDataLabel(int64_t lab, int ndxreg);
	Operand* MakeCodeLabel(int64_t lab);
	Operand* MakeStringAsNameConst(char* s, e_sg seg);
	Operand* MakeString(char* s);
	Operand* MakeImmediate(int64_t i, int display_opt = 0);
	Operand* MakeImmediate(Int128 i, int display_opt = 0);
	Operand* MakeIndirect(int i);
	Operand* MakeIndexed(int64_t o, int i);
	Operand* MakeIndexedName(std::string nme, int i);
	Operand* MakeDoubleIndexed(int i, int j, int scale);
	Operand* MakeDirect(ENODE* node);
	Operand* MakeIndexed(ENODE* node, int rg);

	virtual void GenerateAddOnto(Operand* dst, Operand* src, int sz = 0) {
		GenerateTriadic(op_add, sz, dst, dst, src);
	};
	virtual void GenerateSubtractFrom(Operand* dst, Operand* src, int sz = 0) {
		GenerateTriadic(op_sub, sz, dst, dst, src);
	};
	virtual void GenerateBne(Operand* ap1, Operand* ap2, int64_t lab) {
		GenerateTriadic(op_bne, 0, ap1, ap2, MakeCodeLabel(lab));
	};
	virtual void GenerateBeq(Operand* ap1, Operand* ap2, int64_t lab) {
		GenerateTriadic(op_beq, 0, ap1, ap2, MakeCodeLabel(lab));
	};
	virtual void GenerateBgt(Operand* ap1, Operand* ap2, int64_t lab) {
		GenerateTriadic(op_bgt, 0, ap1, ap2, MakeCodeLabel(lab));
	};
	virtual void GenerateBgtu(Operand* ap1, Operand* ap2, int64_t lab) {
		GenerateTriadic(op_bgtu, 0, ap1, ap2, MakeCodeLabel(lab));
	};
	virtual void GenerateBlt(Operand* ap1, Operand* ap2, int64_t lab) {
		GenerateTriadic(op_blt, 0, ap1, ap2, MakeCodeLabel(lab));
	};
	virtual void GenerateBltu(Operand* ap1, Operand* ap2, int64_t lab) {
		GenerateTriadic(op_bltu, 0, ap1, ap2, MakeCodeLabel(lab));
	};
	virtual void GenerateBra(int64_t lab) {
		GenerateMonadic(op_bra, 0, MakeCodeLabel(lab));
	};
	virtual Operand* MakeBoolean(Operand* oper) { return (oper); };
	void GenerateHint(int num);
	void GenerateComment(char* cm);
	void GenMemop(int op, Operand* ap1, Operand* ap2, int64_t ssize, int typ);
	void GenerateLoadFloat(Operand* ap3, Operand* ap1, int64_t ssize, int64_t size, Operand* mask = nullptr) {
		throw new ArplException(ERR_CODEGEN, 0);
	};
	virtual void GenerateLoad(Operand* ap3, Operand* ap1, int64_t ssize, int64_t size, Operand* mask = nullptr);
	virtual void GenerateLoadAddress(Operand* ap3, Operand* ap1);
	virtual void GenerateStore(Operand* ap1, Operand* ap3, int64_t size, Operand* mask = nullptr);
	Operand* GenerateMux(ENODE*, int flags, int64_t size);
	Operand* GenerateHook(ENODE*, int flags, int64_t size);
	virtual Operand* GenerateLand(ENODE*, int flags, int op, bool safe);
	virtual Operand* GenerateSafeLand(ENODE*, int flags, int op) { return (nullptr); };
	virtual void GenerateBranchTrue(Operand* ap, int64_t label) {};
	virtual void GenerateBranchFalse(Operand* ap, int64_t label) {};
	virtual bool GenerateBranch(ENODE* node, int op, int64_t label, int predreg, unsigned int prediction, bool limit) { return (false); };
	virtual void GenerateLea(Operand* ap1, Operand* ap2) {
		GenerateDiadic(op_lea, 0, ap1, ap2);
	};
	virtual void GenerateMove(Operand* dstreg, Operand* srcreg, Operand* mask=nullptr) {
		GenerateTriadic(op_move, 0, dstreg, srcreg, mask);
	};
	virtual void GenerateCrMove(Operand* dstreg, Operand* srcreg, Operand* mask = nullptr) {
		GenerateTriadic(op_move, 0, dstreg, srcreg, mask);
	};
	virtual void GenerateFcvtdq(Operand* dst, Operand* src) {
		GenerateDiadic(op_fcvtdq, 0, dst, src);
	};
	virtual void SignExtendBitfield(Operand* ap3, uint64_t mask) {};
	virtual Operand* GenerateBitfieldClear(Operand* oper, int startpos, int width) {
		return (oper->GenerateBitfieldClear(startpos, width));
	};
	Operand* GenerateBitfieldAssign(ENODE* node, int flags, int64_t size);
	Operand* GenerateBitfieldAssignAdd(ENODE* node, int flags, int64_t size, int op);
	virtual void GenerateBitfieldInsert(Operand* ap1, Operand* ap2, int offset, int width) {};
	virtual void GenerateBitfieldInsert(Operand* ap1, Operand* ap2, Operand* offset, Operand* width) {};
	virtual void GenerateBitfieldInsert(Operand* ap1, Operand* ap2, ENODE* offset, ENODE* width) {};
	virtual Operand* GenerateBitfieldExtract(Operand* ap1, ENODE* offset, ENODE* width) { return (nullptr); };
	virtual Operand* GenerateBitfieldExtract(Operand* ap1, Operand* offset, Operand* width) { return (nullptr); };
	virtual Operand* GenerateAdd(Operand* dst, Operand* src1, Operand* src2);
	virtual Operand* GenerateAddImmediate(Operand* dst, Operand* src1, Operand* srci);
	virtual Operand* GenerateSubtractImmediate(Operand* dst, Operand* src1, Operand* srci);
	virtual Operand* GenerateAnd(Operand* dst, Operand* src1, Operand* src2);
	virtual Operand* GenerateAndImmediate(Operand* dst, Operand* src1, Operand* srci);
	virtual Operand* GenerateOr(Operand* dst, Operand* src1, Operand* src2);
	virtual Operand* GenerateOrImmediate(Operand* dst, Operand* src1, Operand* srci);
	virtual Operand* GenerateEorImmediate(Operand* dst, Operand* src1, Operand* srci);
	virtual Operand* GenerateSubtract(Operand* dst, Operand* src1, Operand* src2);
	virtual Operand* GenerateShift(ENODE* node, int flags, int64_t size, int op);
	virtual Operand* GenerateBinary(ENODE*, int flags, int64_t size, int op);
	Operand* GenerateBinaryFloat(ENODE* node, int flags, int64_t size, e_op op);
	Operand* GenerateBinaryPosit(ENODE* node, int flags, int64_t size, e_op op);
	Operand* GenerateVectorBinary(ENODE* node, int flags, int64_t size, e_op op);
	Operand* GenerateVectorBinaryFloat(ENODE* node, int flags, int64_t size, e_op op);
	Operand* GenerateAsaddDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size, int64_t siz1, int su, bool neg);
	Operand* GenerateAddDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size, int64_t siz1, int su);
	Operand* GenerateAutoconDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size, int64_t siz1, int su);
	Operand* GenerateClassconDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size, int64_t siz1, int su);
	Operand* GenerateAutofconDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size);
	Operand* GenerateAutopconDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size);
	Operand* GenerateNaconDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size, int64_t siz1, int su);
	Operand* GenerateAutovconDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size);
	Operand* GenerateAutovmconDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size);
	Operand* GenerateLabconDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size, int64_t siz1, int su);
	Operand* GenerateBitfieldDereference(ENODE* node, int flags, int64_t size, int opt);
	Operand* GenerateBitoffsetDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size, int opt);
	Operand* GenerateFieldrefDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size);
	Operand* GenerateRegvarDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size);
	Operand* GenerateFPRegvarDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size);
	Operand* GeneratePositRegvarDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size);
	Operand* GenerateDereference(ENODE* node, int flags, int64_t size, int su, int opt, int rhs);
	Operand* GenerateDereference2(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size, int64_t siz1, int su, int opt);
	Operand* GenerateAssignAdd(ENODE* node, int flags, int64_t size, int op);
	Operand* GenerateAssignMultiply(ENODE* node, int flags, int64_t size, int op);
	Operand* GenerateAssignModiv(ENODE* node, int flags, int64_t size, int op);
	void GenerateStructAssign(TYP* tp, int64_t offset, ENODE* ep, Operand* base);
	void GenerateArrayAssign(TYP* tp, ENODE* node1, ENODE* node2, Operand* base);
	Operand* GenerateAggregateAssign(ENODE* node1, ENODE* node2);
	Operand* GenerateAutocon(ENODE* node, int flags, int64_t size, TYP* type);
	virtual Operand* GenerateFloatcon(ENODE* node, int flags, int64_t size);
	virtual Operand* GenPositcon(ENODE* node, int flags, int64_t size);
	virtual Operand* GenLabelcon(ENODE* node, int flags, int64_t size);
	virtual Operand* GenNacon(ENODE* node, int flags, int64_t size);
	Operand* GenerateAssign(ENODE* node, int flags, int64_t size);
	Operand* GenerateBigAssign(Operand* ap1, Operand* ap2, int64_t size, int64_t ssize);
	Operand* GenerateImmToMemAssign(Operand* ap1, Operand* ap2, int64_t ssize);
	Operand* GenerateRegToMemAssign(Operand* ap1, Operand* ap2, int64_t ssize);
	Operand* GenerateVregToMemAssign(Operand* ap1, Operand* ap2, int64_t ssize);
	Operand* GenerateRegToRegAssign(ENODE* node, Operand* ap1, Operand* ap2, int64_t ssize);
	Operand* GenerateVregToVregAssign(ENODE* node, Operand* ap1, Operand* ap2, int64_t ssize);
	Operand* GenerateImmToRegAssign(Operand* ap1, Operand* ap2, int64_t ssize);
	Operand* GenerateMemToRegAssign(Operand* ap1, Operand* ap2, int64_t size, int64_t ssize);
	Operand* GenerateExpression(ENODE* node, int flags, int64_t size, int rhs);
	virtual void GenerateTrueJump(ENODE* node, int64_t label, unsigned int prediction);
	virtual void GenerateFalseJump(ENODE* node, int64_t label, unsigned int prediction);
	virtual Operand* GenerateEq(ENODE* node) { return (nullptr); };
	virtual Operand* GenerateNe(ENODE* node) { return (nullptr); };
	virtual Operand* GenerateLt(ENODE* node) { return (nullptr); };
	virtual Operand* GenerateLe(ENODE* node) { return (nullptr); };
	virtual Operand* GenerateGt(ENODE* node) { return (nullptr); };
	virtual Operand* GenerateGe(ENODE* node) { return (nullptr); };
	virtual Operand* GenerateLtu(ENODE* node) { return (nullptr); };
	virtual Operand* GenerateLeu(ENODE* node) { return (nullptr); };
	virtual Operand* GenerateGtu(ENODE* node) { return (nullptr); };
	virtual Operand* GenerateGeu(ENODE* node) { return (nullptr); };
	virtual Operand* GenerateFeq(ENODE* node) { return (nullptr); };
	virtual Operand* GenerateFne(ENODE* node) { return (nullptr); };
	virtual Operand* GenerateFlt(ENODE* node) { return (nullptr); };
	virtual Operand* GenerateFle(ENODE* node) { return (nullptr); };
	virtual Operand* GenerateFgt(ENODE* node) { return (nullptr); };
	virtual Operand* GenerateFge(ENODE* node) { return (nullptr); };
	virtual Operand *GenExpr(ENODE *node) { return (nullptr); };
	OCODE* GenerateLoadFloatConst(Operand* ap1, Operand* ap2);
	virtual void GenerateLoadConst(Operand *val, Operand *dst);
	void SaveTemporaries(Function *sym, int *sp, int *fsp, int* psp, int* vsp);
	void RestoreTemporaries(Function *sym, int sp, int fsp, int psp, int vsp);
	virtual void GenerateInterruptSave(Function* func) {
		throw new ArplException(ERR_CODEGEN, 0);
	};
	virtual void GenerateInterruptLoad(Function* func) {
		throw new ArplException(ERR_CODEGEN, 0);
	};
	int GenerateInlineArgumentList(Function *func, ENODE *plist);
	virtual int64_t PushArgument(ENODE *ep, int regno, int stkoffs, bool *isFloat) { return(0); };
	virtual int64_t PushArguments(Function *func, ENODE *plist) { return (0); };
	virtual void PopArguments(Function *func, int howMany, bool isPascal = true) {};
	virtual void GenerateIndirectJump(ENODE* node, Operand* oper, Function* func, int flags, int lab = 0) {};
	virtual void GenerateDirectJump(ENODE* node, Operand* oper, Function* func, int flags, int lab = 0) {};
	virtual bool GenerateInlineCall(ENODE* node, Function* func);
	virtual Operand *GenerateFunctionCall(ENODE *node, int flags, int lab=0);
	virtual int64_t GeneratePrepareFunctionCall(ENODE* node, Function* sym, int* sp, int* fsp, int* psp, int* vsp);
	void GenerateFunction(Function *fn) { fn->Generate(); };
	virtual void GenerateCoroutineExit(Function* func);
	virtual void GenerateReturn(Function* func, Statement* stmt);
	Operand* GenerateTrinary(ENODE* node, int flags, int64_t size, int op);
	virtual void GenerateUnlink(int64_t amt) {};
	virtual void GenerateUnlinkStack(Function* func, int64_t amt);
	virtual void RestoreRegisterVars(Function* func);
	Operand* GenerateCase(ENODE* node, Operand* sw);
//	Operand* GenerateSwitch(ENODE* node);
	void GenerateTabularSwitch(int64_t minv, int64_t maxv, Operand* ap, bool HasDefcase, int deflbl, int tablabel);
	virtual void GenerateCall(Operand* tgt) {
		GenerateMonadic(op_call, 0, tgt);
	};
	virtual void GenerateLocalCall(Operand* tgt) {
		GenerateMonadic(op_call, 0, tgt);
	};
	virtual void GenerateMillicodeCall(Operand* tgt);
	virtual void GenerateReturnAndDeallocate(int64_t amt);
	virtual void GenerateReturnInsn() {
		GenerateZeradic(op_ret);
	};
	virtual void GenerateInterruptReturn(Function* func) {
		GenerateZeradic(op_rti);
	};
	virtual void GenerateReturnAndDeallocate(Operand* ap1);
	virtual void GenerateLoadDataPointer(void);
	virtual void GenerateLoadBssPointer(void);
	virtual void GenerateLoadRodataPointer(void);
	virtual void GenerateSmallDataRegDecl(void);
	virtual void GenerateSignBitExtend(Operand*, Operand*);
	virtual void GenerateSignExtendByte(Operand*, Operand*);
	virtual void GenerateSignExtendWyde(Operand*, Operand*);
	virtual void GenerateSignExtendTetra(Operand*, Operand*);
	virtual void GenerateZeroExtendByte(Operand*, Operand*);
	virtual void GenerateZeroExtendWyde(Operand*, Operand*);
	virtual void GenerateZeroExtendTetra(Operand*, Operand*);
	virtual int GetSegmentIndexReg(e_sg seg);
	virtual Operand* GenerateIndex(ENODE* node, bool neg);

	virtual OCODE* GenerateReturnBlock(Function* fn);
	virtual void SaveRegisterVars(CSet* mask) {};
	virtual int RestoreGPRegisterVars(CSet* mask);
	virtual void GenerateCrGroupPush() { };
	virtual void GenerateCrGroupPop() { };
	virtual Operand* PatchEnter(OCODE* code, CSet* save_mask);
	};

class ThorCodeGenerator : public CodeGenerator
{
public:
	Operand* MakeBoolean(Operand* oper);
	void GenerateLea(Operand* ap1, Operand* ap2);
	void GenerateBranchTrue(Operand* ap, int label);
	void GenerateBranchFalse(Operand* ap, int label);
	void GenerateTrueJump(ENODE* node, int label, unsigned int prediction);
	void GenerateFalseJump(ENODE* node, int label, unsigned int prediction);
	bool GenerateBranch(ENODE *node, int op, int label, int predreg, unsigned int prediction, bool limit);
	void GenerateBeq(Operand*, Operand*, int);
	void GenerateBne(Operand*, Operand*, int);
	void GenerateBlt(Operand*, Operand*, int);
	void GenerateBle(Operand*, Operand*, int);
	void GenerateBgt(Operand*, Operand*, int);
	void GenerateBge(Operand*, Operand*, int);
	void GenerateBltu(Operand*, Operand*, int);
	void GenerateBleu(Operand*, Operand*, int);
	void GenerateBgtu(Operand*, Operand*, int);
	void GenerateBgeu(Operand*, Operand*, int);
	void GenerateBand(Operand*, Operand*, int);
	void GenerateBor(Operand*, Operand*, int);
	void GenerateBnand(Operand*, Operand*, int);
	void GenerateBnor(Operand*, Operand*, int);
	Operand* GenerateEq(ENODE* node);
	Operand* GenerateNe(ENODE* node);
	Operand* GenerateLt(ENODE* node);
	Operand* GenerateLe(ENODE* node);
	Operand* GenerateGt(ENODE* node);
	Operand* GenerateGe(ENODE* node);
	Operand* GenerateLtu(ENODE* node);
	Operand* GenerateLeu(ENODE* node);
	Operand* GenerateGtu(ENODE* node);
	Operand* GenerateGeu(ENODE* node);
	Operand* GenerateFeq(ENODE* node);
	Operand* GenerateFne(ENODE* node);
	Operand* GenerateFlt(ENODE* node);
	Operand* GenerateFle(ENODE* node);
	Operand* GenerateFgt(ENODE* node);
	Operand* GenerateFge(ENODE* node);
	Operand *GenExpr(ENODE *node);
	void LinkAutonew(ENODE *node);
	int64_t PushArgument(ENODE *ep, int regno, int stkoffs, bool *isFloat, int* push_count, bool large_argcount=true);
	int64_t PushArguments(Function *func, ENODE *plist);
	void PopArguments(Function *func, int howMany, bool isPascal = true);
	Operand* GenerateSafeLand(ENODE *, int flags, int op);
	void GenerateIndirectJump(ENODE* node, Operand* oper, Function* func, int flags, int lab = 0);
	void GenerateDirectJump(ENODE* node, Operand* oper, Function* func, int flags, int lab = 0);
	void SignExtendBitfield(Operand* ap3, uint64_t mask);
	void GenerateBitfieldInsert(Operand* dst, Operand* src, int offset, int width);
	void GenerateBitfieldInsert(Operand* dst, Operand* src, Operand* offset, Operand* width);
	void GenerateBitfieldInsert(Operand* ap1, Operand* ap2, ENODE* offset, ENODE* width);
	Operand* GenerateBitfieldExtract(Operand* src, Operand* offset, Operand* width);
	Operand* GenerateBitfieldExtract(Operand* ap1, ENODE* offset, ENODE* width);
	void GenerateUnlink(int64_t amt);
	void GenerateCall(Operand* tgt) {
		GenerateMonadic(op_bsr, 0, tgt);
	};
	void GenerateLocalCall(Operand* tgt) {
		GenerateMonadic(op_bsr, 0, tgt);
	};
	void GenerateReturnInsn() {
		GenerateZeradic(op_rts);
	};
	void GenerateInterruptReturn(Function* func) {
		GenerateZeradic(op_rti);
	};
	void GenerateLoadFloat(Operand* ap3, Operand* ap1, int ssize, int size, Operand* mask = nullptr);
	void GenerateInterruptSave(Function* func);
	void GenerateInterruptLoad(Function* func);
	void GenerateLoadConst(Operand* ap1, Operand* ap2);
};

class starkCodeGenerator : public CodeGenerator
{
public:
	void banner();
	Operand* MakeBoolean(Operand* oper);
	Operand* GenerateLand(ENODE*, int flags, int op, bool safe);
	void GenerateLea(Operand* ap1, Operand* ap2);
	void GenerateBranchTrue(Operand* ap, int64_t label);
	void GenerateBranchFalse(Operand* ap, int64_t label);
	void GenerateTrueJump(ENODE* node, int64_t label, unsigned int prediction);
	void GenerateFalseJump(ENODE* node, int64_t label, unsigned int prediction);
	bool GenerateBranch(ENODE* node, int op, int64_t label, int predreg, unsigned int prediction, bool limit);
	void GenerateBeq(Operand*, Operand*, int64_t);
	void GenerateBne(Operand*, Operand*, int64_t);
	void GenerateBlt(Operand*, Operand*, int64_t);
	void GenerateBle(Operand*, Operand*, int64_t);
	void GenerateBgt(Operand*, Operand*, int64_t);
	void GenerateBge(Operand*, Operand*, int64_t);
	void GenerateBltu(Operand*, Operand*, int64_t);
	void GenerateBleu(Operand*, Operand*, int64_t);
	void GenerateBgtu(Operand*, Operand*, int64_t);
	void GenerateBgeu(Operand*, Operand*, int64_t);
	void GenerateBand(Operand*, Operand*, int64_t);
	void GenerateBor(Operand*, Operand*, int64_t);
	void GenerateBnand(Operand*, Operand*, int64_t);
	void GenerateBnor(Operand*, Operand*, int64_t);
	Operand* GenerateEq(ENODE* node);
	Operand* GenerateNe(ENODE* node);
	Operand* GenerateLt(ENODE* node);
	Operand* GenerateLe(ENODE* node);
	Operand* GenerateGt(ENODE* node);
	Operand* GenerateGe(ENODE* node);
	Operand* GenerateLtu(ENODE* node);
	Operand* GenerateLeu(ENODE* node);
	Operand* GenerateGtu(ENODE* node);
	Operand* GenerateGeu(ENODE* node);
	Operand* GenerateFeq(ENODE* node);
	Operand* GenerateFne(ENODE* node);
	Operand* GenerateFlt(ENODE* node);
	Operand* GenerateFle(ENODE* node);
	Operand* GenerateFgt(ENODE* node);
	Operand* GenerateFge(ENODE* node);
	Operand* GenExpr(ENODE* node);
	void LinkAutonew(ENODE* node);
	int64_t PushArgument(ENODE* ep, int regno, int stkoffs, bool* isFloat, int* push_count, bool large_argcount = true);
	int64_t PushArguments(Function* func, ENODE* plist);
	void PopArguments(Function* func, int howMany, bool isPascal = true);
	Operand* GenerateSafeLand(ENODE*, int flags, int op);
	void GenerateIndirectJump(ENODE* node, Operand* oper, Function* func, int flags, int lab = 0);
	void GenerateDirectJump(ENODE* node, Operand* oper, Function* func, int flags, int lab = 0);
	void SignExtendBitfield(Operand* ap3, uint64_t mask);
	void GenerateBitfieldInsert(Operand* dst, Operand* src, int offset, int width);
	void GenerateBitfieldInsert(Operand* dst, Operand* src, Operand* offset, Operand* width);
	void GenerateBitfieldInsert(Operand* ap1, Operand* ap2, ENODE* offset, ENODE* width);
	Operand* GenerateBitfieldExtract(Operand* src, Operand* offset, Operand* width);
	Operand* GenerateBitfieldExtract(Operand* ap1, ENODE* offset, ENODE* width);
	Operand* GenerateShift(ENODE* node, int flags, int64_t size, int op);
	void GenerateUnlink(int64_t amt);
	void GenerateCall(Operand* tgt) {
		GenerateMonadic(op_bsr, 0, tgt);
	};
	void GenerateLocalCall(Operand* tgt) {
		GenerateMonadic(op_bsr, 0, tgt);
	};
	void GenerateReturnInsn() {
		GenerateZeradic(op_rts);
	};
	void GenerateInterruptReturn(Function* func) {
		GenerateZeradic(op_rti);
	};
	void GenerateReturnAndDeallocate(int64_t amt);
	void GenerateReturnAndDeallocate(Operand* ap1);
	void GenerateLoadFloat(Operand* ap3, Operand* ap1, int64_t ssize, int64_t size, Operand* mask = nullptr);
	void GenerateInterruptSave(Function* func);
	void GenerateInterruptLoad(Function* func);
	void GenerateLoadConst(Operand* operi, Operand* dst);
	void GenerateLoadDataPointer(void);
	void GenerateLoadBssPointer(void);
	void GenerateLoadRodataPointer(void);
	void GenerateSmallDataRegDecl(void);
	void GenerateSignExtendByte(Operand*, Operand*);
	void GenerateSignExtendWyde(Operand*, Operand*);
	void GenerateSignExtendTetra(Operand*, Operand*);
	void GenerateLoadAddress(Operand* ap3, Operand* ap1);
	void GenerateLoad(Operand* ap3, Operand* ap1, int64_t ssize, int64_t size, Operand* mask = nullptr);
	void GenerateStore(Operand* ap1, Operand* ap3, int64_t size, Operand* mask = nullptr);
	void GenerateLoadStore(e_op opcode, Operand* ap1, Operand* ap2);
	Operand* GenerateAddImmediate(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateAndImmediate(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateOrImmediate(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateEorImmediate(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateAdd(Operand* dst, Operand* src1, Operand* src2);
	Operand* GenerateSubtract(Operand* dst, Operand* src1, Operand* src2);

	OCODE* GenerateReturnBlock(Function* fn);

	Operand* GenerateFloatcon(ENODE* node, int flags, int64_t size);
	Operand* GenPositcon(ENODE* node, int flags, int64_t size);
	Operand* GenLabelcon(ENODE* node, int flags, int64_t size);
	Operand* GenNacon(ENODE* node, int flags, int64_t size);
	void ConvertOffsetWidthToBeginEnd(Operand* offset, Operand* width, Operand** op_begin, Operand** op_end);
	int GetSegmentIndexReg(e_sg seg);
	void SaveRegisterVars(CSet* mask);
	bool CheckForShortAddressMode(e_op opcode, Operand* oper, e_op* opopcode);
};

class QuplsCodeGenerator : public CodeGenerator
{
public:
	void banner();
	Operand* MakeBoolean(Operand* oper);
	Operand* GenerateLand(ENODE*, int flags, int op, bool safe);
	void GenerateLea(Operand* ap1, Operand* ap2);
	void GenerateBranchTrue(Operand* ap, int64_t label);
	void GenerateBranchFalse(Operand* ap, int64_t label);
	void GenerateTrueJump(ENODE* node, int64_t label, unsigned int prediction);
	void GenerateFalseJump(ENODE* node, int64_t label, unsigned int prediction);
	bool GenerateBranch(ENODE* node, int op, int64_t label, int predreg, unsigned int prediction, bool limit);
	void GenerateBeq(Operand*, Operand*, int64_t);
	void GenerateBne(Operand*, Operand*, int64_t);
	void GenerateBlt(Operand*, Operand*, int64_t);
	void GenerateBle(Operand*, Operand*, int64_t);
	void GenerateBgt(Operand*, Operand*, int64_t);
	void GenerateBge(Operand*, Operand*, int64_t);
	void GenerateBltu(Operand*, Operand*, int64_t);
	void GenerateBleu(Operand*, Operand*, int64_t);
	void GenerateBgtu(Operand*, Operand*, int64_t);
	void GenerateBgeu(Operand*, Operand*, int64_t);
	void GenerateBand(Operand*, Operand*, int64_t);
	void GenerateBor(Operand*, Operand*, int64_t);
	void GenerateBnand(Operand*, Operand*, int64_t);
	void GenerateBnor(Operand*, Operand*, int64_t);
	Operand* GenerateEq(ENODE* node);
	Operand* GenerateNe(ENODE* node);
	Operand* GenerateLt(ENODE* node);
	Operand* GenerateLe(ENODE* node);
	Operand* GenerateGt(ENODE* node);
	Operand* GenerateGe(ENODE* node);
	Operand* GenerateLtu(ENODE* node);
	Operand* GenerateLeu(ENODE* node);
	Operand* GenerateGtu(ENODE* node);
	Operand* GenerateGeu(ENODE* node);
	Operand* GenerateFeq(ENODE* node);
	Operand* GenerateFne(ENODE* node);
	Operand* GenerateFlt(ENODE* node);
	Operand* GenerateFle(ENODE* node);
	Operand* GenerateFgt(ENODE* node);
	Operand* GenerateFge(ENODE* node);
	Operand* GenExpr(ENODE* node);
	void LinkAutonew(ENODE* node);
	int64_t PushArgument(ENODE* ep, int regno, int stkoffs, bool* isFloat, int* push_count, bool large_argcount = true);
	int64_t PushArguments(Function* func, ENODE* plist);
	void PopArguments(Function* func, int howMany, bool isPascal = true);
	Operand* GenerateSafeLand(ENODE*, int flags, int op);
	void GenerateIndirectJump(ENODE* node, Operand* oper, Function* func, int flags, int lab = 0);
	void GenerateDirectJump(ENODE* node, Operand* oper, Function* func, int flags, int lab = 0);
	void SignExtendBitfield(Operand* ap3, uint64_t mask);
	void GenerateBitfieldInsert(Operand* dst, Operand* src, int offset, int width);
	void GenerateBitfieldInsert(Operand* dst, Operand* src, Operand* offset, Operand* width);
	void GenerateBitfieldInsert(Operand* ap1, Operand* ap2, ENODE* offset, ENODE* width);
	Operand* GenerateBitfieldExtract(Operand* src, Operand* offset, Operand* width);
	Operand* GenerateBitfieldExtract(Operand* ap1, ENODE* offset, ENODE* width);
	Operand* GenerateShift(ENODE* node, int flags, int64_t size, int op);
	void GenerateUnlink(int64_t amt);
	void GenerateCall(Operand* tgt) {
		GenerateMonadic(op_bsr, 0, tgt);
	};
	void GenerateLocalCall(Operand* tgt) {
		GenerateMonadic(op_bsr, 0, tgt);
	};
	void GenerateReturnInsn() {
		GenerateZeradic(op_rts);
	};
	void GenerateInterruptReturn(Function* func) {
		GenerateZeradic(op_rti);
	};
	void GenerateReturnAndDeallocate(int64_t amt);
	void GenerateReturnAndDeallocate(Operand* ap1);
	void GenerateLoadFloat(Operand* ap3, Operand* ap1, int64_t ssize, int64_t size, Operand* mask = nullptr);
	void GenerateInterruptSave(Function* func);
	void GenerateInterruptLoad(Function* func);
	void GenerateLoadConst(Operand* operi, Operand* dst);
	void GenerateLoadDataPointer(void);
	void GenerateLoadBssPointer(void);
	void GenerateLoadRodataPointer(void);
	void GenerateSmallDataRegDecl(void);
	void GenerateSignExtendByte(Operand*, Operand*);
	void GenerateSignExtendWyde(Operand*, Operand*);
	void GenerateSignExtendTetra(Operand*, Operand*);
	void GenerateLoadAddress(Operand* ap3, Operand* ap1);
	void GenerateLoad(Operand* ap3, Operand* ap1, int64_t ssize, int64_t size, Operand* mask = nullptr);
	void GenerateStore(Operand* ap1, Operand* ap3, int64_t size, Operand* mask = nullptr);
	void GenerateLoadStore(e_op opcode, Operand* ap1, Operand* ap2);
	Operand* GenerateAddImmediate(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateAndImmediate(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateOrImmediate(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateEorImmediate(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateAdd(Operand* dst, Operand* src1, Operand* src2);
	Operand* GenerateSubtract(Operand* dst, Operand* src1, Operand* src2);

	OCODE* GenerateReturnBlock(Function* fn);

	Operand* GenerateFloatcon(ENODE* node, int flags, int64_t size);
	Operand* GenPositcon(ENODE* node, int flags, int64_t size);
	Operand* GenLabelcon(ENODE* node, int flags, int64_t size);
	Operand* GenNacon(ENODE* node, int flags, int64_t size);
	void ConvertOffsetWidthToBeginEnd(Operand* offset, Operand* width, Operand** op_begin, Operand** op_end);
	int GetSegmentIndexReg(e_sg seg);
	void SaveRegisterVars(CSet* mask);
	bool CheckForShortAddressMode(e_op opcode, Operand* oper, e_op* opopcode);
};

class LB650CodeGenerator : public CodeGenerator
{
public:
	void banner();
	Operand* MakeBoolean(Operand* oper);
	Operand* GenerateLand(ENODE*, int flags, int op, bool safe);
	void GenerateLea(Operand* ap1, Operand* ap2);
	void GenerateBranchTrue(Operand* ap, int64_t label);
	void GenerateBranchFalse(Operand* ap, int64_t label);
	void GenerateTrueJump(ENODE* node, int64_t label, unsigned int prediction);
	void GenerateFalseJump(ENODE* node, int64_t label, unsigned int prediction);
	bool GenerateBranch(ENODE* node, int op, int64_t label, int predreg, unsigned int prediction, bool limit);
	void GenerateBeq(Operand*, Operand*, int64_t);
	void GenerateBne(Operand*, Operand*, int64_t);
	void GenerateBlt(Operand*, Operand*, int64_t);
	void GenerateBle(Operand*, Operand*, int64_t);
	void GenerateBgt(Operand*, Operand*, int64_t);
	void GenerateBge(Operand*, Operand*, int64_t);
	void GenerateBltu(Operand*, Operand*, int64_t);
	void GenerateBleu(Operand*, Operand*, int64_t);
	void GenerateBgtu(Operand*, Operand*, int64_t);
	void GenerateBgeu(Operand*, Operand*, int64_t);
	void GenerateBand(Operand*, Operand*, int64_t);
	void GenerateBor(Operand*, Operand*, int64_t);
	void GenerateBnand(Operand*, Operand*, int64_t);
	void GenerateBnor(Operand*, Operand*, int64_t);
	Operand* GenerateEq(ENODE* node);
	Operand* GenerateNe(ENODE* node);
	Operand* GenerateLt(ENODE* node);
	Operand* GenerateLe(ENODE* node);
	Operand* GenerateGt(ENODE* node);
	Operand* GenerateGe(ENODE* node);
	Operand* GenerateLtu(ENODE* node);
	Operand* GenerateLeu(ENODE* node);
	Operand* GenerateGtu(ENODE* node);
	Operand* GenerateGeu(ENODE* node);
	Operand* GenerateFeq(ENODE* node);
	Operand* GenerateFne(ENODE* node);
	Operand* GenerateFlt(ENODE* node);
	Operand* GenerateFle(ENODE* node);
	Operand* GenerateFgt(ENODE* node);
	Operand* GenerateFge(ENODE* node);
	Operand* GenExpr(ENODE* node);
	void LinkAutonew(ENODE* node);
	int64_t PushArgument(ENODE* ep, int regno, int stkoffs, bool* isFloat, int* push_count, bool large_argcount = true);
	int64_t PushArguments(Function* func, ENODE* plist);
	void PopArguments(Function* func, int howMany, bool isPascal = true);
	Operand* GenerateSafeLand(ENODE*, int flags, int op);
	void GenerateIndirectJump(ENODE* node, Operand* oper, Function* func, int flags, int lab = 0);
	void GenerateDirectJump(ENODE* node, Operand* oper, Function* func, int flags, int lab = 0);
	void SignExtendBitfield(Operand* ap3, uint64_t mask);
	void GenerateBitfieldInsert(Operand* dst, Operand* src, int offset, int width);
	void GenerateBitfieldInsert(Operand* dst, Operand* src, Operand* offset, Operand* width);
	void GenerateBitfieldInsert(Operand* ap1, Operand* ap2, ENODE* offset, ENODE* width);
	Operand* GenerateBitfieldExtract(Operand* src, Operand* offset, Operand* width);
	Operand* GenerateBitfieldExtract(Operand* ap1, ENODE* offset, ENODE* width);
	Operand* GenerateShift(ENODE* node, int flags, int64_t size, int op);
	void GenerateUnlink(int64_t amt);
	void GenerateCall(Operand* tgt) {
		GenerateMonadic(op_bsr, 0, tgt);
	};
	void GenerateLocalCall(Operand* tgt) {
		GenerateMonadic(op_bsr, 0, tgt);
	};
	void GenerateReturnInsn() {
		GenerateZeradic(op_rts);
	};
	void GenerateInterruptReturn(Function* func) {
		GenerateZeradic(op_rti);
	};
	void GenerateReturnAndDeallocate(int64_t amt);
	void GenerateReturnAndDeallocate(Operand* ap1);
	void GenerateLoadFloat(Operand* ap3, Operand* ap1, int64_t ssize, int64_t size, Operand* mask = nullptr);
	void GenerateInterruptSave(Function* func);
	void GenerateInterruptLoad(Function* func);
	void GenerateLoadConst(Operand* operi, Operand* dst);
	void GenerateLoadDataPointer(void);
	void GenerateLoadBssPointer(void);
	void GenerateLoadRodataPointer(void);
	void GenerateSmallDataRegDecl(void);
	void GenerateSignExtendByte(Operand*, Operand*);
	void GenerateSignExtendWyde(Operand*, Operand*);
	void GenerateSignExtendTetra(Operand*, Operand*);
	void GenerateLoadAddress(Operand* ap3, Operand* ap1);
	void GenerateLoad(Operand* ap3, Operand* ap1, int64_t ssize, int64_t size, Operand* mask = nullptr);
	void GenerateStore(Operand* ap1, Operand* ap3, int64_t size, Operand* mask = nullptr);
	void GenerateLoadStore(e_op opcode, Operand* ap1, Operand* ap2);
	Operand* GenerateAddImmediate(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateAndImmediate(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateOrImmediate(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateEorImmediate(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateAdd(Operand* dst, Operand* src1, Operand* src2);
	Operand* GenerateSubtract(Operand* dst, Operand* src1, Operand* src2);

	OCODE* GenerateReturnBlock(Function* fn);

	Operand* GenerateFloatcon(ENODE* node, int flags, int64_t size);
	Operand* GenPositcon(ENODE* node, int flags, int64_t size);
	Operand* GenLabelcon(ENODE* node, int flags, int64_t size);
	Operand* GenNacon(ENODE* node, int flags, int64_t size);
	void ConvertOffsetWidthToBeginEnd(Operand* offset, Operand* width, Operand** op_begin, Operand** op_end);
	int GetSegmentIndexReg(e_sg seg);
	void SaveRegisterVars(CSet* mask);
};

class RiscvCodeGenerator : public CodeGenerator
{
public:
	Operand* GetTempFPRegister();
	Operand* MakeBoolean(Operand* oper);
	void GenerateBranchTrue(Operand* ap, int label);
	void GenerateBranchFalse(Operand* ap, int label);
	void GenerateTrueJump(ENODE* node, int label, unsigned int prediction);
	void GenerateFalseJump(ENODE* node, int label, unsigned int prediction);
	bool GenerateBranch(ENODE* node, int op, int label, int predreg, unsigned int prediction, bool limit);
	void GenerateBand(Operand*, Operand*, int);
	void GenerateBor(Operand*, Operand*, int);
	void GenerateBnand(Operand*, Operand*, int);
	void GenerateBnor(Operand*, Operand*, int);
	void GenerateBne(Operand*, Operand*, int);
	void GenerateBeq(Operand*, Operand*, int);
	void GenerateBgt(Operand*, Operand*, int);
	void GenerateBgtu(Operand*, Operand*, int);
	void GenerateBge(Operand*, Operand*, int);
	void GenerateBgeu(Operand*, Operand*, int);
	void GenerateBlt(Operand* ap1, Operand* ap2, int lab);
	void GenerateBltu(Operand* ap1, Operand* ap2, int lab);
	void GenerateBle(Operand* ap1, Operand* ap2, int lab);
	void GenerateBleu(Operand* ap1, Operand* ap2, int lab);
	Operand* GenerateEq(ENODE* node);
	Operand* GenerateNe(ENODE* node);
	Operand* GenerateLt(ENODE* node);
	Operand* GenerateLtu(ENODE* node);
	Operand* GenerateLe(ENODE* node);
	Operand* GenerateLeu(ENODE* node);
	Operand* GenerateGt(ENODE* node);
	Operand* GenerateGtu(ENODE* node);
	Operand* GenerateGe(ENODE* node);
	Operand* GenerateGeu(ENODE* node);
	Operand* GenerateFeq(ENODE* node);
	Operand* GenerateFne(ENODE* node);
	Operand* GenerateFlt(ENODE* node);
	Operand* GenerateFle(ENODE* node);
	Operand* GenerateFgt(ENODE* node);
	Operand* GenerateFge(ENODE* node);
	Operand* GenExpr(ENODE* node);
	void GenerateFcvtdq(Operand* dst, Operand* src) {
		GenerateDiadic(op_fcvtqdd, 0, dst, src);
	};
	void GenerateLoadFloat(Operand* ap3, Operand* ap1, int ssize, int size, Operand* mask = nullptr);
	void GenerateLoad(Operand* ap3, Operand* ap1, int ssize, int size, Operand* mask = nullptr);
	void GenerateLoadAddress(Operand* ap3, Operand* ap1);
	void GenerateStore(Operand* ap1, Operand* ap3, int size, Operand* mask = nullptr);
	void GenerateLea(Operand* ap1, Operand* ap2) {
		GenerateDiadic(op_la, 0, ap1, ap2);
	};
	void GenerateMove(Operand* dstreg, Operand* srcreg, Operand* mask = nullptr) {
		GenerateTriadic(op_mv, 0, dstreg, srcreg, mask);
	};
	int64_t PushArgument(ENODE* ep, int regno, int stkoffs, bool* isFloat, int* push_count, bool large_argcount = true);
	int64_t PushArguments(Function* func, ENODE* plist);
	void PopArguments(Function* func, int howMany, bool isPascal = true);
	void GenerateIndirectJump(ENODE* node, Operand* oper, Function* func, int flags, int lab = 0);
	void GenerateDirectJump(ENODE* node, Operand* oper, Function* func, int flags, int lab = 0);
	void GenerateCall(Operand* tgt);
	void GenerateLocalCall(Operand* tgt);
	void GenerateMillicodeCall(Operand* tgt);
	void GenerateReturnAndDeallocate(int64_t amt);
	void GenerateReturnInsn();
	void GenerateInterruptReturn(Function* func);
	void GenerateInterruptSave(Function* func);
	void GenerateInterruptLoad(Function* func);
	void GenerateLoadConst(Operand* ap1, Operand* ap2);
};

class BigfootCodeGenerator : public CodeGenerator
{
public:
	void banner();
	Operand* MakeBoolean(Operand* oper);
	Operand* BoolToInt(Operand* oper);
	Operand* GenerateLand(ENODE*, int flags, int op, bool safe);
	int64_t GetSavedRegisterList(CSet* rmask);
	void GenerateLea(Operand* ap1, Operand* ap2);
	void GenerateBranchTrue(Operand* ap, int64_t label);
	void GenerateBranchFalse(Operand* ap, int64_t label);
	void GenerateTrueJump(ENODE* node, int64_t label, unsigned int prediction);
	void GenerateFalseJump(ENODE* node, int64_t label, unsigned int prediction);
	bool GenerateBranch(ENODE* node, int op, int64_t label, int predreg, unsigned int prediction, bool limit);
	void GenerateBra(int64_t);
	void GenerateBeq(Operand*, Operand*, int64_t);
	void GenerateBne(Operand*, Operand*, int64_t);
	void GenerateBlt(Operand*, Operand*, int64_t);
	void GenerateBle(Operand*, Operand*, int64_t);
	void GenerateBgt(Operand*, Operand*, int64_t);
	void GenerateBge(Operand*, Operand*, int64_t);
	void GenerateBltu(Operand*, Operand*, int64_t);
	void GenerateBleu(Operand*, Operand*, int64_t);
	void GenerateBgtu(Operand*, Operand*, int64_t);
	void GenerateBgeu(Operand*, Operand*, int64_t);
	void GenerateBand(Operand*, Operand*, int64_t);
	void GenerateBor(Operand*, Operand*, int64_t);
	void GenerateBnand(Operand*, Operand*, int64_t);
	void GenerateBnor(Operand*, Operand*, int64_t);
	Operand* GenerateEq(ENODE* node);
	Operand* GenerateNe(ENODE* node);
	Operand* GenerateLt(ENODE* node);
	Operand* GenerateLe(ENODE* node);
	Operand* GenerateGt(ENODE* node);
	Operand* GenerateGe(ENODE* node);
	Operand* GenerateLtu(ENODE* node);
	Operand* GenerateLeu(ENODE* node);
	Operand* GenerateGtu(ENODE* node);
	Operand* GenerateGeu(ENODE* node);
	Operand* GenerateFeq(ENODE* node);
	Operand* GenerateFne(ENODE* node);
	Operand* GenerateFlt(ENODE* node);
	Operand* GenerateFle(ENODE* node);
	Operand* GenerateFgt(ENODE* node);
	Operand* GenerateFge(ENODE* node);
	Operand* GenExpr(ENODE* node);
	void LinkAutonew(ENODE* node);
	int64_t PushArgument(ENODE* ep, int regno, int stkoffs, bool* isFloat, int* push_count, bool large_argcount = true);
	int64_t PushArguments(Function* func, ENODE* plist);
	void PopArguments(Function* func, int howMany, bool isPascal = true);
	Operand* GenerateSafeLand(ENODE*, int flags, int op);
	void GenerateIndirectJump(ENODE* node, Operand* oper, Function* func, int flags, int lab = 0);
	void GenerateDirectJump(ENODE* node, Operand* oper, Function* func, int flags, int lab = 0);
	void GenerateSignExtendBit(Operand* ap3, Operand* width);
	void GenerateZeroExtendBit(Operand* ap3, Operand* width);
	void GenerateBitfieldInsert(Operand* dst, Operand* src, int offset, int width);
	void GenerateBitfieldInsert(Operand* dst, Operand* src, Operand* offset, Operand* width);
	void GenerateBitfieldInsert(Operand* ap1, Operand* ap2, ENODE* offset, ENODE* width);
	Operand* GenerateBitfieldExtract(Operand* src, Operand* offset, Operand* width);
	Operand* GenerateBitfieldExtract(Operand* ap1, ENODE* offset, ENODE* width);
	void GenerateUnlink(int64_t amt);
	void GenerateCall(Operand* tgt) {
		GenerateMonadic(op_bsr, 0, tgt);
	};
	void GenerateLocalCall(Operand* tgt) {
		GenerateMonadic(op_bsr, 0, tgt);
	};
	void GenerateReturnInsn() {
		GenerateZeradic(op_rts);
	};
	void GenerateInterruptReturn(Function* func) {
		GenerateZeradic(op_rti);
	};
	void GenerateReturnAndDeallocate(int64_t amt);
	void GenerateReturnAndDeallocate(Operand* ap1);
	void GenerateLoadFloat(Operand* ap3, Operand* ap1, int64_t ssize, int64_t size, Operand* mask = nullptr);
	void GenerateInterruptSave(Function* func);
	void GenerateInterruptLoad(Function* func);
	void GenerateLoadConst(Operand* operi, Operand* dst);
	void GenerateLoadDataPointer(void);
	void GenerateLoadBssPointer(void);
	void GenerateLoadRodataPointer(void);
	void GenerateSmallDataRegDecl(void);
	void GenerateSignExtendByte(Operand*, Operand*);
	void GenerateSignExtendWyde(Operand*, Operand*);
	void GenerateSignExtendTetra(Operand*, Operand*);
	void GenerateLoadAddress(Operand* ap3, Operand* ap1);
	void GenerateLoad(Operand* ap3, Operand* ap1, int64_t ssize, int64_t size, Operand* mask = nullptr);
	void GenerateStore(Operand* ap1, Operand* ap3, int64_t size, Operand* mask = nullptr);
	void GenerateStoreImmediate(Operand* ap1, Operand* ap2, int64_t size);
	void GenerateLoadStore(e_op opcode, Operand* ap1, Operand* ap2);
	Operand* GenerateBinary(ENODE* node, int flags, int64_t size, int op);
	Operand* GenerateAddImmediate(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateAndImmediate(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateOrImmediate(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateEorImmediate(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateOr(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateEor(Operand* dst, Operand* src1, Operand* srci);

	OCODE* GenerateReturnBlock(Function* fn);
	void GenerateUnlinkStack(Function* fn, int64_t amt);

	Operand* GenerateFloatcon(ENODE* node, int flags, int64_t size);
	Operand* GenPositcon(ENODE* node, int flags, int64_t size);
	Operand* GenLabelcon(ENODE* node, int flags, int64_t size);
	Operand* GenNacon(ENODE* node, int flags, int64_t size);
	void ConvertOffsetWidthToBeginEnd(Operand* offset, Operand* width, Operand** op_begin, Operand** op_end);
	int GetSegmentIndexReg(e_sg seg);
	void SaveRegisterVars(CSet* mask);
	int RestoreGPRegisterVars(CSet* save_mask);
	void GenerateCrGroupPush();
	void GenerateCrGroupPop();
	void GenerateCrMove(Operand* dstreg, Operand* srcreg, Operand* mask = nullptr);
};

class i386CodeGenerator : public CodeGenerator
{
private:
	Operand* RegAsDirect(int regno);
public:
	void banner();
	Operand* MakeBoolean(Operand* oper);
	Operand* GenerateLand(ENODE*, int flags, int op, bool safe);
	void GenerateLea(Operand* ap1, Operand* ap2);
	void GenerateBranchTrue(Operand* ap, int64_t label);
	void GenerateBranchFalse(Operand* ap, int64_t label);
	void GenerateTrueJump(ENODE* node, int64_t label, unsigned int prediction);
	void GenerateFalseJump(ENODE* node, int64_t label, unsigned int prediction);
	bool GenerateBranch(ENODE* node, int op, int64_t label, int predreg, unsigned int prediction, bool limit);
	void GenerateBeq(Operand*, Operand*, int64_t);
	void GenerateBne(Operand*, Operand*, int64_t);
	void GenerateBlt(Operand*, Operand*, int64_t);
	void GenerateBle(Operand*, Operand*, int64_t);
	void GenerateBgt(Operand*, Operand*, int64_t);
	void GenerateBge(Operand*, Operand*, int64_t);
	void GenerateBltu(Operand*, Operand*, int64_t);
	void GenerateBleu(Operand*, Operand*, int64_t);
	void GenerateBgtu(Operand*, Operand*, int64_t);
	void GenerateBgeu(Operand*, Operand*, int64_t);
	void GenerateBand(Operand*, Operand*, int64_t);
	void GenerateBor(Operand*, Operand*, int64_t);
	void GenerateBnand(Operand*, Operand*, int64_t);
	void GenerateBnor(Operand*, Operand*, int64_t);
	Operand* GenerateEq(ENODE* node);
	Operand* GenerateNe(ENODE* node);
	Operand* GenerateLt(ENODE* node);
	Operand* GenerateLe(ENODE* node);
	Operand* GenerateGt(ENODE* node);
	Operand* GenerateGe(ENODE* node);
	Operand* GenerateLtu(ENODE* node);
	Operand* GenerateLeu(ENODE* node);
	Operand* GenerateGtu(ENODE* node);
	Operand* GenerateGeu(ENODE* node);
	Operand* GenerateFeq(ENODE* node);
	Operand* GenerateFne(ENODE* node);
	Operand* GenerateFlt(ENODE* node);
	Operand* GenerateFle(ENODE* node);
	Operand* GenerateFgt(ENODE* node);
	Operand* GenerateFge(ENODE* node);
	Operand* GenExpr(ENODE* node);
	void LinkAutonew(ENODE* node);
	int64_t PushArgument(ENODE* ep, int regno, int stkoffs, bool* isFloat, int* push_count, bool large_argcount = true);
	int64_t PushArguments(Function* func, ENODE* plist);
	void PopArguments(Function* func, int howMany, bool isPascal = true);
	Operand* GenerateSafeLand(ENODE*, int flags, int op);
	void GenerateIndirectJump(ENODE* node, Operand* oper, Function* func, int flags, int lab = 0);
	void GenerateDirectJump(ENODE* node, Operand* oper, Function* func, int flags, int lab = 0);
	void SignExtendBitfield(Operand* ap3, uint64_t mask);
	void GenerateBitfieldInsert(Operand* dst, Operand* src, int offset, int width);
	void GenerateBitfieldInsert(Operand* dst, Operand* src, Operand* offset, Operand* width);
	void GenerateBitfieldInsert(Operand* ap1, Operand* ap2, ENODE* offset, ENODE* width);
	Operand* GenerateBitfieldExtract(Operand* src, Operand* offset, Operand* width);
	Operand* GenerateBitfieldExtract(Operand* ap1, ENODE* offset, ENODE* width);
	void GenerateUnlink(int64_t amt);
	virtual void GenerateBra(int64_t lab) {
		GenerateMonadic(op_jmp, 0, MakeCodeLabel(lab));
	};
	void GenerateCall(Operand* tgt) {
		GenerateMonadic(op_call, 0, tgt);
	};
	void GenerateLocalCall(Operand* tgt) {
		GenerateMonadic(op_call, 0, tgt);
	};
	void GenerateReturnInsn() {
		GenerateZeradic(op_ret);
	};
	void GenerateInterruptReturn(Function* func) {
		GenerateZeradic(op_iret);
	};
	void GenerateMove(Operand* dstreg, Operand* srcreg, Operand* mask = nullptr);
	void GenerateReturnAndDeallocate(int64_t amt);
	void GenerateReturnAndDeallocate(Operand* ap1);
	void GenerateLoadFloat(Operand* ap3, Operand* ap1, int64_t ssize, int64_t size, Operand* mask = nullptr);
	void GenerateInterruptSave(Function* func);
	void GenerateInterruptLoad(Function* func);
	void GenerateLoadConst(Operand* operi, Operand* dst);
	void GenerateLoadDataPointer(void);
	void GenerateLoadBssPointer(void);
	void GenerateLoadRodataPointer(void);
	void GenerateSmallDataRegDecl(void);
	void GenerateSignExtendByte(Operand*, Operand*);
	void GenerateSignExtendWyde(Operand*, Operand*);
	void GenerateLoadAddress(Operand* ap3, Operand* ap1);
	void GenerateLoad(Operand* ap3, Operand* ap1, int64_t ssize, int64_t size, Operand* mask = nullptr);
	void GenerateStore(Operand* src, Operand* dst, int64_t size, Operand* mask = nullptr);
	void GenerateStoreImmediate(Operand* ap1, Operand* ap2, int64_t size);
	void GenerateLoadStore(e_op opcode, Operand* ap1, Operand* ap2);
	Operand* GenerateAdd(Operand* dst, Operand* src1, Operand* src2);
	Operand* GenerateAddImmediate(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateSubtractImmediate(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateAnd(Operand* dst, Operand* src1, Operand* src2);
	Operand* GenerateAndImmediate(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateOr(Operand* dst, Operand* src1, Operand* src2);
	Operand* GenerateOrImmediate(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateEorImmediate(Operand* dst, Operand* src1, Operand* srci);
	Operand* GenerateShift(ENODE* node, int flags, int64_t size, int op);
	OCODE* GenerateReturnBlock(Function* fn);

	Operand* GenerateFloatcon(ENODE* node, int flags, int64_t size);
	Operand* GenPositcon(ENODE* node, int flags, int64_t size);
	Operand* GenLabelcon(ENODE* node, int flags, int64_t size);
	Operand* GenNacon(ENODE* node, int flags, int64_t size);
	Operand* GenerateIndex(ENODE* node, bool neg);
	void ConvertOffsetWidthToBeginEnd(Operand* offset, Operand* width, Operand** op_begin, Operand** op_end);
	int GetSegmentIndexReg(e_sg seg);
	void SaveRegisterVars(CSet* mask);
};

// Control Flow Graph
// For now everything in this class is static and there are no member variables
// to it.
class CFG
{
public:
	static void Create();
	static void CalcDominatorTree();
	static void CalcDominanceFrontiers();
	static void InsertPhiInsns();
	static OCODE *FindLabel(int64_t i) { return (PeepList::FindLabel(i)); };
	static void Rename();
	static void Search(BasicBlock *);
	static void Subscript(Operand *oper);
	static int WhichPred(BasicBlock *x, int y);
};


/*      output code structure   */
/*
OCODE {
	OCODE *fwd, *back, *comment;
	short opcode;
	short length;
	unsigned int isVolatile : 1;
	unsigned int isReferenced : 1;	// label is referenced by code
	unsigned int remove : 1;
	short pregreg;
	short predop;
	Operand *oper1, *oper2, *oper3, *oper4;
};
typedef OCODE OCODE;
*/

class IntStack
{
public:
	int *stk;
	int sp;
	int size;
public:
	static IntStack *MakeNew(int sz) {
		IntStack *s;
		s = (IntStack *)allocx(sizeof(IntStack));
		s->stk = (int *)allocx(sz * sizeof(int));
		s->sp = sz;
		s->size = sz;
		return (s);
	}
	static IntStack *MakeNew() {
		return (MakeNew(1000));
	}
	void push(int v) {
		if (sp > 0) {
			sp--;
			stk[sp] = v;
		}
		else
			throw new ArplException(ERR_STACKFULL, 0);
	};
	int pop() {
		int v = 0;
		if (sp < size) {
			v = stk[sp];
			sp++;
			return (v);
		}
		throw new ArplException(ERR_STACKEMPTY, 0);
	};
	int tos() {
		return (stk[sp]);
	};
	bool IsEmpty() { return (sp == size); };
};

class Edge : public CompilerType
{
public:
	bool backedge;
	Edge *next;
	Edge *prev;
	BasicBlock *src;
	BasicBlock *dst;
};

class BasicBlock : public CompilerType
{
public:
	int num;
	Edge *ohead;
	Edge *otail;
	Edge *ihead;
	Edge *itail;
	Edge *dhead;
	Edge *dtail;
public:
	int length;		// number of instructions
	unsigned int changed : 1;
	unsigned int isColored : 1;
	unsigned int isRetBlock : 1;
	int depth;
	CSet *gen;		// use
	CSet *kill;		// def
	CSet *LiveIn;
	CSet *LiveOut;
	CSet *live;
	CSet *MustSpill;
	CSet *NeedLoad;
	CSet *DF;		// dominance frontier
	CSet *trees;
	int HasAlready;
	int Work;
	static CSet *livo;
	BasicBlock *next;
	BasicBlock *prev;
	OCODE *code;
	OCODE *lcode;
	static BasicBlock *RootBlock;
	static int nBasicBlocks;
	CSet *color;
public:
	static bool AllCodeHasBasicBlock(OCODE* start);
	static BasicBlock *MakeNew();
	static BasicBlock *Blockize(OCODE *start);
	Edge *MakeOutputEdge(BasicBlock *dst);
	Edge *MakeInputEdge(BasicBlock *src);
	Edge *MakeDomEdge(BasicBlock *dst);
	static void Unite(int father, int son);
	void ComputeLiveVars();
	void AddLiveOut(BasicBlock *ip);
	bool IsIdom(BasicBlock *b);
	void ExpandReturnBlocks();

	void UpdateLive(int);
	void CheckForDeaths(int r);
	static void ComputeSpillCosts();
	static void InsertMove(int reg, int rreg, int blk);
	void BuildLivesetFromLiveout();
	static void DepthSort();
	static bool Coalesce();
	void InsertSpillCode(int reg, int64_t offs);
	void InsertFillCode(int reg, int64_t offs);
	static void SetAllUncolored();
	void Color();
	static void ColorAll();
};

class Map
{
public:
	int newnums[3072];
};

// A "range" in Briggs terminology
class Range : public CompilerType
{
public:
	int var;
	int num;
	CSet *blocks;
	int degree;
	int lattice;
	bool spill;
	__int16 color;
	int regclass;		// 1 = integer, 2 = floating point, 4 = vector
	// Cost accounting
	float loads;
	float stores;
	float copies;
	float others;
	bool infinite;
	float cost;
	static int treeno;
public:
	Range() { };
	static Range *MakeNew();
	void ClearCosts();
	float SelectRatio() { return (cost / (float)degree); };
};

class Forest
{
public:
	short int treecount;
	Range *trees[1032];
	Function *func;
	CSet low, high;
	IntStack *stk;
	static int k;
	short int map[3072];
	short int pass;
	// Cost accounting
	float loads;
	float stores;
	float copies;
	float others;
	bool infinite;
	float cost;
	Var *var;
public:
	Forest();
	Range *MakeNewTree();
	Range *PlantTree(Range *t);
	void ClearCosts() {
		int r;
		for (r = 0; r < treecount; r++)
			trees[r]->ClearCosts();
	}
	void ClearCut() {
		int r;
		for (r = 0; r < treecount; r++) {
			delete trees[r];
			trees[r] = nullptr;
		}
	};
	void CalcRegclass();
	void SummarizeCost();
	void Renumber();
	void push(int n) { stk->push(n); };
	int pop() { return (stk->pop()); };
	void Simplify();
	void PreColor();
	void Color();
	void Select() { Color(); };
	int SelectSpillCandidate();
	int GetSpillCount();
	int GetRegisterToSpill(int tree);
	bool SpillCode();
	void ColorBlocks();
	bool IsAllTreesColored();
	unsigned int ColorUncolorable(unsigned int);
};


class Var : public CompilerType
{
public:
	Var *next;
	int num;
	int cnum;
	Forest trees;
	CSet *forest;
	CSet *visited;
	IntStack *istk;
	int subscript;
	int64_t spillOffset;	// offset in stack where spilled
	e_rc regclass;
	static int nvar;
public:
	static Var *MakeNew();
	void GrowTree(Range *, BasicBlock *);
	// Create a forest for a specific Var
	void CreateForest();
	// Create a forest for each Var object
	static void CreateForests();
	static void Renumber(int old, int nw);
	static void RenumberNeg();
	static Var *Find(int);
	static Var *Find2(int);
	static Var *FindByCnum(int);
	static Var *FindByMac(int reg);
	static Var *FindByTreeno(int tn);
	static CSet *Find3(int reg, int blocknum);
	static int FindTreeno(int reg, int blocknum);
	static int PathCompress(int reg, int blocknum, int *);
	static void DumpForests(int);
	void Transplant(Var *);
	static bool Coalesce2();
	Var *GetVarToSpill(CSet *exc);
};

class IGraph
{
public:
	int *bitmatrix;
	__int16 *degrees;
	__int16 **vecs;
	int size;
	int K;
	Forest *frst;
	int pass;
	enum e_am workingRegclass;
	enum e_op workingMoveop;
public:
	~IGraph();
	void Destroy();
	void MakeNew(int n);
	void ClearBitmatrix();
	void Clear();
	int BitIndex(int x, int y, int *intndx, int *bitndx);
	void Add(int x, int y);
	void Add2(int x, int y);
	void AddToLive(BasicBlock *b, Operand *ap, OCODE *ip);
	void AddToVec(int x, int y);
	void InsertArgumentMoves();
	bool Remove(int n);
	static int FindTreeno(int reg, int blocknum) { return (Var::FindTreeno(reg, blocknum)); };
	bool DoesInterfere(int x, int y);
	int Degree(int n) { return ((int)degrees[n]); };
	__int16 *GetNeighbours(int n, int *count) { if (count) *count = degrees[n]; return (vecs[n]); };
	void Unite(int father, int son);
	void Fill();
	void AllocVecs();
	void BuildAndCoalesce();
	void Print(int);
};


class Instruction
{
public:
	const char *mnem;		// mnemonic
	short opcode;	// matches OCODE opcode
	short extime;	// execution time, divide may take hundreds of cycles
	uint8_t targetCount;		// number of target operands
	bool memacc;	// instruction accesses memory
	unsigned int amclass1;	// address mode class, one for each possible operand
	unsigned int amclass2;
	unsigned int amclass3;
	unsigned int amclass4;
public:
	static void SetMap();
	static Instruction *GetMapping(int op);
	bool IsFlowControl();
	bool IsLoad();
	bool IsIntegerLoad();
	bool IsStore();
	bool IsExt();
	bool IsSetInsn() {
		return (opcode == op_seq || opcode == op_sne
			|| opcode == op_slt || opcode == op_sle || opcode == op_sgt || opcode == op_sge
			|| opcode == op_sltu || opcode == op_sleu || opcode == op_sgtu || opcode == op_sgeu
			);
	};
	short InvertSet();
	static Instruction *FindByMnem(std::string& mn);
	static Instruction *Get(int op);
	inline bool HasTarget() { return (targetCount != 0); };
	size_t store(txtoStream& ofs);
	size_t storeHex(txtoStream& ofs);	// hex intermediate representation
	size_t storeHRR(txtoStream& ofs);	// human readable representation
	static Instruction *loadHex(std::ifstream& fp);
	int load(std::ifstream& ifs, Instruction **p);
};

class CSE {
public:
	short int nxt;
  ENODE *exp;           /* optimizable expression */
  short int uses;           /* number of uses */
  short int duses;          /* number of dereferenced uses */
  short int reg;            /* AllocateRegisterVarsd register */
  bool voidf;      /* cannot optimize flag */
  bool isfp;
	bool isPosit;
	bool is_vector;
public:
	void AccUses(int val);					// accumulate uses
	void AccDuses(int val);					// accumulate duses
	int OptimizationDesireability();
};

class starkCSE : public CSE
{
public:
	int OptimizationDesireability();
};

class QuplsCSE : public CSE
{
public:
	int OptimizationDesireability();
};

class LB650CSE : public CSE
{
public:
	int OptimizationDesireability();
};

class BigfootCSE : public CSE
{
public:
	int OptimizationDesireability();
};

class CSETable
{
public:
	CSE table[500];
	short int csendx;
	short int cseiter;
	short int searchpos;
public:
	CSETable();
	~CSETable();
	CSE *First() { cseiter = 0; return &table[0]; };
	CSE *Next() { cseiter++; return (cseiter < csendx ? &table[cseiter] : nullptr); };
	void Clear() { ZeroMemory(table, sizeof(table)); csendx = 0; };
	void Sort(int (*)(const void *a, const void *b));
	void Assign(CSETable *);
	int voidauto2(ENODE *node);
	CSE *InsertNode(ENODE *node, int duse, bool *first);
	CSE *Search(ENODE *node);
	CSE *SearchNext(ENODE *node);
	CSE *SearchByNumber(ENODE *node);

	void GenerateRegMask(CSE *csp, CSet *mask, CSet *rmask);
	int AllocateGPRegisters();
	int AllocateFPRegisters();
	int AllocatePositRegisters();
	int AllocateVectorRegisters();
	int AllocateRegisterVars();
	void InitializeTempRegs();

	int Optimize(Statement *);

	// Debugging
	void Dump();
};

class Statement {
public:
	int number;
	std::string* name;
	e_stmt stype;
	Statement *outer;
	Statement *next;
	Statement *prolog;
	Statement *epilog;
	Function* fi;				// Function connected to statement
	bool nkd;
	bool tabular;				// tabular switch statement
	bool contains_label;
	int predreg;		// assigned predicate register
	ENODE *exp;         // condition or expression
	ENODE *initExpr;    // initialization expression - for loops
	ENODE *incrExpr;    // increment expression - for loops
	ENODE* iexp;
	Statement *s1, *s2; // internal statements
	int num;			// resulting expression type (hash code for throw)
	int64_t *label;     // label number for goto
	int64_t *casevals;	// case values
	TABLE ssyms;		// local symbols associated with statement
	char *fcname;       // firstcall block var name
	char *lptr;			// pointer to source code
	char *lptr2;			// pointer to source code
	unsigned int prediction : 2;	// static prediction for if statements
	int depth;
	e_sym kw;				// statement's keyword
	static int throwlab;
	static int oldthrow;
	static int olderthrow;
	static bool lc_in_use;
	bool generated;
	
	Statement* MakeStatement(int typ, int gt);

	// Parsing
	Symbol* gsearch2(std::string na, __int16 rettype, TypeArray* typearray, bool exact) {};
	static int64_t* GetCasevals();
	Statement* ParseDefault();
	Statement* ParseCheckStatement();
	Statement *ParseStop();
	Statement *ParseCompound(bool assign_func);
	Statement *ParseDo();
	Statement *ParseFor();
	Statement *ParseForever();
	Statement *ParseFirstcall();
	Statement *ParseIf();
	Statement *ParseCatch();
	Statement *ParseCase();
	int CheckForDuplicateCases();
	Statement *ParseThrow();
	Statement *ParseContinue();
	Statement *ParseAsm();
	Statement *ParseTry();
	Statement *ParseExpression();
	Statement* ParseExpression(ENODE** node, Symbol* symi);
	Statement *ParseLabel(bool pt=true);
	Statement *ParseWhile();
	Statement *ParseUntil();
	Statement *ParseGoto();
	Statement *ParseReturn();
	Statement *ParseBreak();
	Statement *ParseSwitch();
	Statement* ParseYield();
	Statement* Parse(ENODE** node, Symbol* symi, bool* has_label=nullptr);
	Statement* Parse(bool* has_label=nullptr);

	// Optimization
	void scan();
	void scan_compound();
	void repcse();
	void repcse_compound();
	void update();
	void update_compound();

	// Code generation
	int FindNextLabel(int);
	void ResetGenerated();
	Operand *MakeDataLabel(int lab, int ndxreg);
	Operand *MakeCodeLabel(int lab);
	Operand *MakeStringAsNameConst(char *s, e_sg seg);
	Operand *MakeString(char *s);
	Operand *MakeImmediate(int64_t i);
	Operand *MakeIndirect(int i);
	Operand *MakeIndexed(int64_t o, int i);
	Operand* MakeIndexedName(std::string nme, int i);
	Operand *MakeDoubleIndexed(int i, int j, int scale);
	Operand *MakeDirect(ENODE *node);
	Operand *MakeIndexed(ENODE *node, int rg);
	void GenStore(Operand *ap1, Operand *ap3, int size);

	void GenMixedSource();
	void GenMixedSource2();
	void GenerateStop();
	void GenerateAsm();
	void GenerateFirstcall();
	void GenerateWhile();
	void GenerateUntil();
	bool IsDecByOne();
	bool IsNEZeroTest();
	bool IsInitNonZero();
	bool FindLoopVar(int64_t);
	void GenerateCountedLoop();
	void GenerateFor();
	void GenerateForever();
	void GenerateIf();
	void GenerateDoWhile();
	void GenerateDoUntil();
	void GenerateDoLoop();
	void GenerateDoOnce();
	void GenerateCase();
	void GenerateDefault();
	int CountSwitchCasevals();
	int CountSwitchCases();
	bool IsOneHotSwitch();
	void GetMinMaxSwitchValue(int64_t* min, int64_t* max);
	void GenerateSwitchSearch(Case* cases, Operand*, Operand*, int, int, int, int, int, bool, bool);
	void GenerateSwitchLo(Case* cases, Operand*, Operand*, int, int, int, bool, bool, bool last_case);
	void GenerateSwitchLop2(Case* cases, Operand*, Operand*, int, int, int, bool, bool);
	void GenerateNakedTabularSwitch(int64_t, Operand*, int);
	void GenerateSwitchStatements();
	std::string GenerateSwitchTargetName(int labno);
	void GenerateTry();
	void GenerateThrow();
	void GenerateCatch(int opt, int oldthrowlab, int olderthrow);
	void GenerateCheck();
	void GenerateFuncBody();
	void GenerateSwitch();
	void GenerateLinearSwitch();
	void GenerateTabularSwitch(int64_t, int64_t, Operand*, bool, int, int);
	void GenerateYield();
	bool Generate(int opt = 0);
	void CheckReferences(int* sp, int* bp, int* gp, int* gp1, int* gp2);
	void CheckCompoundReferences(int* sp, int* bp, int* gp, int* gp1, int* gp2);
	// Debugging
	void Dump();
	void DumpCompound();
	void ListCompoundVars();
	// Serialization
	void store(txtoStream& fs);
	void storeIf(txtoStream& ofs);
	void storeWhile(txtoStream& fs);
	void storeCompound(txtoStream& ofs);

	void storeHex(txtoStream& ofs);
	void storeHexIf(txtoStream& ofs);
	void storeHexDo(txtoStream& ofs, e_stmt type);
	void storeHexWhile(txtoStream& fs, e_stmt type);
	void storeHexFor(txtoStream& fs);
	void storeHexForever(txtoStream& fs);
	void storeHexSwitch(txtoStream& fs);
	void storeHexCompound(txtoStream& ofs);
};

class StatementGenerator : public CompilerType
{
public:
	static Operand* MakeCodeLabel(int lab);
	static Operand* MakeImmediate(int64_t i);
	static Operand* MakeIndirect(int i);
	static void GenerateCompound(Statement* stmt);
	static Operand* GenerateCase(ENODE* node, Operand* sw_ap);
	static Operand* GenerateSwitch(ENODE* node);
	static void GenerateDefault(Statement* stmt);
	static void GenerateSwitchStatements(Statement* type);
	static bool IsTabularSwitch(int64_t numcases, int64_t min, int64_t max, bool nkd);
	void GenerateSwitch(Statement* stmt);
	virtual void GenerateLinearSwitch(Statement* type);
	virtual void GenerateNakedTabularSwitch(Statement* stmt, int64_t minv, Operand* ap, int tablabel) {};
	virtual void GenerateTabularSwitch(Statement* stmt, int64_t minv, int64_t maxv, Operand* ap, bool HasDefcase, int deflbl, int tablabel) {};
	static void GenerateSwitchLo(Statement* stmt, Case* cases, Operand* ap, Operand* ap2, int lo, int xitlab, int deflab, bool is_unsigned, bool one_hot, bool last_case);
	static void GenerateSwitchSearch(Statement* stmt, Case* cases, Operand* ap, Operand* ap2, int midlab, int lo, int hi, int xitlab, int deflab, bool is_unsigned, bool one_hot);
};

class starkStatementGenerator : public StatementGenerator
{
public:
	void GenerateNakedTabularSwitch(Statement* stmt, int64_t minv, Operand* ap, int tablabel);
	void GenerateTabularSwitch(Statement* stmt, int64_t minv, int64_t maxv, Operand* ap, bool HasDefcase, int deflbl, int tablabel);
};

class QuplsStatementGenerator : public StatementGenerator
{
public:
	void GenerateNakedTabularSwitch(Statement* stmt, int64_t minv, Operand* ap, int tablabel);
	void GenerateTabularSwitch(Statement* stmt, int64_t minv, int64_t maxv, Operand* ap, bool HasDefcase, int deflbl, int tablabel);
};

class LB650StatementGenerator : public StatementGenerator
{
public:
	void GenerateNakedTabularSwitch(Statement* stmt, int64_t minv, Operand* ap, int tablabel);
	void GenerateTabularSwitch(Statement* stmt, int64_t minv, int64_t maxv, Operand* ap, bool HasDefcase, int deflbl, int tablabel);
};

#ifdef THOR
class ThorStatementGenerator : public StatementGenerator
{
public:
	void GenerateNakedTabularSwitch(Statement* stmt, int64_t minv, Operand* ap, int tablabel);
	void GenerateTabularSwitch(Statement* stmt, int64_t minv, int64_t maxv, Operand* ap, bool HasDefcase, int deflbl, int tablabel);
};
#endif

class RiscvStatementGenerator : public StatementGenerator
{
public:
	void GenerateNakedTabularSwitch(Statement* stmt, int64_t minv, Operand* ap, int tablabel);
	void GenerateTabularSwitch(Statement* stmt, int64_t minv, int64_t maxv, Operand* ap, bool HasDefcase, int deflbl, int tablabel);
};

class BigfootStatementGenerator : public StatementGenerator
{
public:
	void GenerateNakedTabularSwitch(Statement* stmt, int64_t minv, Operand* ap, int tablabel);
	void GenerateTabularSwitch(Statement* stmt, int64_t minv, int64_t maxv, Operand* ap, bool HasDefcase, int deflbl, int tablabel);
};

class i386StatementGenerator : public StatementGenerator
{
public:
	void GenerateNakedTabularSwitch(Statement* stmt, int64_t minv, Operand* ap, int tablabel);
	void GenerateTabularSwitch(Statement* stmt, int64_t minv, int64_t maxv, Operand* ap, bool HasDefcase, int deflbl, int tablabel);
};

class StatementFactory : public Factory
{
public:
	Statement* MakeStatement(int typ, int gt);
};


class Stringx
{
public:
  std::string str;
};

class Declaration
{
private:
	void SetType(Symbol* sp);
	int decl_level; 
	int pa_level;
	Symbol* CreateNonameVar();
	bool isTypedefs[100];
public:
	static int depth;
	e_decltype dcltp;
	bool isLocal;
	bool isTypedef;
	bool isFar;
	TYP* head;
	TYP* tail;
	int16_t bit_offset;
	int16_t bit_width;
	int16_t bit_next;
	int16_t bit_max;
	int8_t funcdecl;
	e_sc istorage_class;
	TABLE* itable;
	short inline_threshold;
	int64_t int_save_mask;
	int8_t operating_mode;
	Statement* stmt;
	Function* func;
	std::string* declid;
public:
	Declaration() {
		stmt = nullptr;
	};
	Declaration(Statement* st);
	Declaration *next;
	void AssignParameterName();
	int64_t declare(Symbol* parent, TABLE* table, e_sc sc, int64_t ilc, int ztype, Symbol** symo, bool local, short int depth=0);
	void ParseEnumerationList(TABLE *table, Float128 amt, Symbol *parent, bool power);
	void ParseEnum(TABLE *table);
	void ParseVoid();
	void ParseInline();
	void ParseInterrupt();
	void ParseConst();
	void ParseTypedef();
	void ParseDeclspec();
	void ParseNaked();
	void ParseShort();
	void ParseLong();
	void ParseBool();
	void ParseBit();
	void ParseInt(bool nt = true);
	void ParseInt64();
	void ParseInt32();
	void ParseChar();
	void ParseInt8();
	void ParseByte();
	void ParseFloat(int prec=64);
	void ParseDouble();
	void ParseTriple();
	void ParseDecimal();
	void ParseFloat128();
	void ParsePosit();
	void ParseClass();
	void ParseVarAttribute();
	void ParseAlign();
	int ParseStruct(TABLE* table, e_bt typ, Symbol** sym);
	void ParseVector(TABLE* table, Symbol** sym, e_sc sc);
	void ParseVectorMask();
	Symbol *ParseSpecifierId();
	void ParseDoubleColon(Symbol *sp);
	void ParseBitfieldSpec(bool isUnion);
	void ParsePrecisionSpec();
	int ParseSpecifier(TABLE* table, Symbol** sym, e_sc sc);
	Symbol *ParsePrefixId(Symbol*);
	Symbol *ParsePrefixOpenpa(bool isUnion, Symbol*);
	TYP* ParsePrefix(bool isUnion,Symbol*, bool local, Symbol**);
	void ParseSuffixOpenbr();
	Function* ParseSuffixOpenpa(Function *);
	Symbol *ParseSuffix(Symbol *sp);
	static void ParseFunctionAttribute(Function *sym, bool needpa);
	int ParseFunction(TABLE* table, Symbol* sp, Symbol* parent, e_sc al, bool local);
	Function* ParseFunctionJ2(Function* fn);
	void ParseCoroutine();
	void ParseAssign(Symbol *sp);
	void DoDeclarationEnd(Symbol *sp, Symbol *sp1);
	void DoInsert(Symbol *sp, TABLE *table);
	Symbol *FindSymbol(Symbol *sp, TABLE *table);

	int GenerateStorage(txtoStream& tfs, int nbytes, int al, int ilc);
	static Function* MakeFunction(int symnum, Symbol* sym, bool isPascal, bool isInline);
	static void MakeFunction(Symbol* sp, Symbol* sp1);
	void FigureStructOffsets(int64_t bgn, Symbol* sp);
	bool IsScalar(e_sym lastst);
	bool IsFloat(e_sym lastst);
};

class StructDeclaration : public Declaration
{
private:
	Symbol* isym;
private:
	int ParseTag(TABLE* table, e_bt ztype, Symbol** sym);
	Symbol* CreateSymbol(char* nmbuf, TABLE* table, e_bt ztype, int* ret);
public:
	StructDeclaration() { Declaration(nullptr); };
	void GetType(TYP** hd, TYP** tl) {
		*hd = head; *tl = tail;
	};
	void ParseAttribute(Symbol* sym);
	void ParseAttributes(Symbol* sym);
	void ParseMembers(Symbol* sym, int ztype);
	int Parse(TABLE* table, int ztype, Symbol** sym);
};

class ClassDeclaration : public Declaration
{
public:
	void GetType(TYP** hd, TYP** tl) {
		*hd = head; *tl = tail;
	};
	void ParseMembers(Symbol * sym, int ztype);
	int Parse(int ztype);
};

class AutoDeclaration : public Declaration
{
public:
	ENODE* Parse(Symbol *parent, TABLE *ssyms, Statement* st);
	int ParseId(Symbol* parent, TABLE* ssyms, Statement* st);
	void ParseThread(Symbol* parent, TABLE* ssyms, Statement* st);
	void ParseStatic(Symbol* parent, TABLE* ssyms, Statement* st);
	void ParseExtern(Symbol* parent, TABLE* ssyms, Statement* st);
	void ParseIntrinsicType(Symbol* parent, TABLE* ssyms, Statement* st);
};

class ParameterDeclaration : public Declaration
{
public:
	int number;
	int ellip;	// parameter number of the ellipsis if present
public:
	TABLE* Parse(int, bool throw_away, Function* st);
};

class GlobalDeclaration : public Declaration
{
public:
	void Parse();
	static GlobalDeclaration *Make();
};

class Compiler
{
public:
	int typenum;
	int symnum;
	short int funcnum;
	Symbol symbolTable[32768];
	Symbol* symTables[10];
	Function functionTable[3000];
	Function* programFn;
	TYP typeTable[32768];
	OperandFactory of;
	FunctionFactory ff;
	ExpressionFactory ef;
	StatementFactory sf;
	StatementGenerator* sg;
	AutoDeclaration ad;
	SymbolFactory syf;
	struct clit* casetab;
	Float128* quadtab;
	e_cpu cputype;
	short int pass;
	bool ipoll;
	bool nogcskips;
	bool os_code;
	int pollCount;
	bool exceptions;
	short int autoInline;
	short int table_density;		// switch table density threshold as a percentage.
	short int reg_in_use[2048];
	short int CrRegInUse[2048];
	CSet temp_in_use;
	CSet saved_in_use;
	CSet savedCrInUse;
	CSet tempCrInUse;
	char firstid[128];
	char lastid[128];
	int64_t lc_static;
	int expr_depth;
public:
	Compiler() { 
		int i;

		for (i = 0; i < 10; i++)
			symTables[i] = nullptr;
		symTables[0] = &symbolTable[0];
		typenum = 0; ipoll = false; pollCount = 33;
		autoInline = 5;
		table_density = 33;
#ifdef STARK
		sg = new starkStatementGenerator;
#endif
#ifdef LB650
		sg = new LB650StatementGenerator;
#endif
#ifdef QUPLS
		sg = new QuplsStatementGenerator;
#endif
#ifdef QUPLS40
		sg = new QuplsStatementGenerator;
#endif
#ifdef THOR
		sg = new ThorStatementGenerator;
#endif
#ifdef RISCV
		sg = new RiscvStatementGenerator;
#endif
		casetab = nullptr;
		quadtab = nullptr;
	};
	GlobalDeclaration *decls;
	void compile();
	int GetUnusedTemp();
	int PreprocessFile(char *nm);
	void CloseFiles();
	void AddStandardTypes();
	void AddBuiltinFunctions();
	static int GetReturnBlockSize();
	int main2(int c, char **argv);
	void storeHex(txtoStream& ofs);
	void loadHex(txtiStream& ifs);
	void storeTables();
	void DumpGlobals();
};

class Register
{
public:
	int number;
	int depth;					// stack level
	short int nextreg;	// next register in list
	short int prevreg;	// previous register in list
	int8_t cls;					// register class GPR, FLT, CCR
	bool isCr;					// condition code register
	bool isCrg;					// condition code group register
	bool isTemp;
	bool isLastTemp;
	bool isFirstTemp;
	bool isArg;
	bool isSaved;
	bool containsValue;
	CSet isPushed;
	CSet inUse;
public:
	static Operand* GetTempCrRegister();
};

class CPU
{
public:
	std::string fileExt;
	int nregs;
	int pagesize;
	int cache_line_size;
	int code_align;
	int NumRegs;
	int NumArgRegs;
	int NumTmpRegs;
	int NumTmpCrRegs;
	int NumSavedRegs;
	int NumFargRegs;
	int NumFtmpRegs;
	int NumFsavedRegs;
	int NumvArgRegs;
	int NumvTmpRegs;
	int NumvSavedRegs;
	int NumvmArgRegs;
	int NumvmTmpRegs;
	int NumvmSavedRegs;
	int NumSavedCrRegs;
	int argregs[96];
	int tmpregs[96];
	int saved_regs[96];
	int fargregs[96];
	int ftmpregs[96];
	int fsaved_regs[96];
	int vargregs[96];
	int vtmpregs[96];
	int vsaved_regs[96];
	int vmargregs[96];
	int vmtmpregs[96];
	int vmsaved_regs[96];
	Register tmpCrRegs[96];
	Register regs[96];
	CSet availableTemps;
	int savedCrRegs[96];
	bool SupportsBand;
	bool SupportsBor;
	bool SupportsBBS;
	bool SupportsBBC;
	bool SupportsPush;
	bool SupportsPop;
	bool SupportsLink;
	bool SupportsUnlink;
	bool SupportsBitfield;
	bool SupportsLDM;
	bool SupportsSTM;
	bool SupportsPtrdif;
	bool SupportsEnter;
	bool SupportsLeave;
	bool SupportsIndexed;
	bool SupportsTrinary;
	void SetRealRegisters();
	void SetVirtualRegisters();
	bool Addsi;
	int pushpop_multiple;
	int ext_op;
	int extu_op;
	int mov_op;
	int lea_op;
	int ldi_op;
	int ldo_op;
	int ldt_op;
	int ldw_op;
	int ldb_op;
	int ldd_op;
	int ldbu_op;
	int ldwu_op;
	int ldtu_op;
	int sto_op;
	int stt_op;
	int stw_op;
	int stb_op;
	int std_op;
	int sizeOfWord;
	int sizeOfPtr;
	int sizeOfFP;
	int sizeOfFPS;
	int sizeOfFPT;
	int sizeOfFPD;
	int sizeOfFPQ;
	int sizeOfInt;
	int sizeOfDecimal;
	int sizeOfPosit;
	int RIimmSize;			// size in bits of immediate field for RI instructions
	Instruction* itbl;
	int itbl_cnt;
	CPU();
	virtual void InitRegs();
	int GetTypePrecision(e_bt typ);
	int GetTypeSize(e_bt typ);
	virtual int ReturnBlockSize() {
		return (4 * sizeOfWord);
	};
	virtual char* RegMoniker(int32_t regno);
};

class starkCPU : public CPU
{
public:
	starkCPU();
	void InitRegs();
	char* RegMoniker(int32_t regno);
};

class QuplsCPU : public CPU
{
public:
	QuplsCPU();
	char* RegMoniker(int32_t regno);
};

class LB650CPU : public CPU
{
public:
	LB650CPU();
	char* RegMoniker(int32_t regno);
};

class RiscvCPU : public CPU
{
public:
	RiscvCPU();
};

class BigfootCPU : public CPU
{
public:
	BigfootCPU();
	int ReturnBlockSize() {
		return (4 * sizeOfWord);
	};
	virtual char* RegMoniker(int32_t regno);
};

class i386CPU : public CPU
{
public:
	i386CPU();
	int ReturnBlockSize() {
		return (4 * sizeOfWord);
	};
	char* RegMoniker(int32_t regno);
};

class BigfootOCODE : public OCODE
{
public:
	void OptAdd();
};

//#define SYM     struct sym
//#define TYP     struct typ
//#define TABLE   struct stab

 
#endif
