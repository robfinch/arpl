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

//TYP *tail = (TYP *)NULL;
TABLE tagtable;
TABLE scrap_table;
TYP stdconst;
TYP stdvector;
TYP stdvectormask;
Stringx names[20];
int nparms = 0;
int funcdecl = 0;		//0,1, or 2
int nfc = 0;
int isFirstCall = 0;
int catchdecl = FALSE;
bool isUnion = false;
int isUnsigned = FALSE;
int isSigned = FALSE;
int isVolatile = FALSE;
int isVirtual = FALSE;
bool isInline = false;
int isIO = FALSE;
int isConst = FALSE;
bool DoesContextSave = false;
bool isRegister = false;
bool isAuto = false;
bool isFuncBody;
bool isFuncPtr;
bool isLeaf = false;
int missingArgumentName = FALSE;
int disableSubs;
int parsingParameterList = false;
int unnamedCnt = 0;
int needParseFunction = 0;
int isStructDecl = FALSE;
int worstAlignment = 0;
char *stkname = 0;
int64_t sp_init = 0;
std::string *classname;
bool isPrivate = true;
Symbol *currentClass;
int mangledNames = FALSE;
int defaultcc = 1;
int isCoroutine = 0;
std::string* UnknownFuncName();

bool IsDeclBegin(int st)
{
	return (st == star || st == id || st == openpa || st == openbr);
}

int     imax(int i, int j)
{       return ((i > j) ? i : j);
}


char *my_strdup(char *s)
{
	char *p;
	if (s == nullptr)
		return (nullptr);
	int n = strlen(s);
	int m = sizeof(char);
	p = (char *)allocx(sizeof(char)*(n+1));
	memcpy(p,s,sizeof(char)*(n));
	p[n] = '\0';
	return (p);
}

std::string* UnknownFuncName()
{
	static int fn = 0;
	char buf[300];

	fn++;
	sprintf_s(buf, sizeof(buf), "_UnknownFn%d", fn);
	return new std::string(buf);
}

void Declaration::SetType(Symbol *sp)
{
	if (head) {
		if (bit_width <= 0) {
			sp->tp = head;
		}
		else {
			if (head->IsFloatType()) {
				sp->tp = head;
				sp->tp->precision = bit_width;
			}
			else if (head->IsVectorType()) {
				sp->tp = head;
				sp->tp->numele = bit_width;
			}
			else {
				sp->tp = TYP::Make(bt_bitfield,head->size);
				sp->tp->isUnsigned = head->isUnsigned;
				//  		*(sp->tp) = *head;
				// The width eventually ends up in an extract or deposit instruction so,
				// minus one is subtracted here. (The instruction wants one less than
				// the actual width.
				sp->tp->bit_width = makeinode(en_icon, (int64_t)bit_width-1);
				sp->tp->bit_offset = makeinode(en_icon, bit_offset);
			}
		}
	}
	else {
		sp->tp = TYP::Make(bt_int,cpu.sizeOfInt);
		sp->tp->lst.headp = sp;
	}
}

// Ignore const
void Declaration::ParseConst()
{
	isConst = TRUE;
	NextToken();
}

void Declaration::ParseTypedef()
{
	if (decl_level==1)
		isTypedef = true;
	NextToken();
}

void Declaration::ParseNaked()
{
	isNocall = TRUE;
	head = (TYP *)TYP::Make(bt_oscall,2);
	tail = head;
	NextToken();
}

void Declaration::ParseVoid()
{
	head = (TYP *)TYP::Make(bt_void,0);
	tail = head;
	head->isVolatile = isVolatile;
	head->isIO = isIO;
	NextToken();
	if (lastst==kw_interrupt) {
		ParseInterrupt();
	}
	if (lastst==kw_nocall || lastst==kw_naked) {
		isNocall = TRUE;
		NextToken();
	}
	bit_max = 0;
}

void Declaration::ParseInterrupt()
{
	isInterrupt = true;
	isPascal = false;
	head = (TYP*)TYP::Make(bt_interrupt, 2);
	tail = head;
	sp_init = 0LL;
	DoesContextSave = false;
	NextToken();
	int_save_mask = 0;
	if (lastst == openpa) {
		NextToken();
		int_save_mask = GetIntegerExpression(nullptr,nullptr,0).low;
		/*
		if (lastst!=id)
				error(ERR_IDEXPECT);
		*/
		if (lastst == comma) {
			NextToken();
			DoesContextSave = GetIntegerExpression(nullptr,nullptr,0).low;
		}
		needpunc(closepa,49);
		//stkname = my_strdup(compiler.lastid);
	}
}

void Declaration::ParseShort()
{
	bit_max = 32;
	NextToken();
	switch(lastst) {
	case kw_int:
		NextToken();
		if (isUnsigned) {
			head = (TYP *)TYP::Make(bt_ushort,4);
			tail = head;
		}
		else {
			head = (TYP *)TYP::Make(bt_short,4);
			tail = head;
		}
		break;
	case kw_float:
		NextToken();
		bit_max = 16;
		head = (TYP*)TYP::Make(bt_half, 2);
		tail = head;
		break;
	default:
		if (isUnsigned) {
			head = (TYP *)TYP::Make(bt_ushort,4);
			tail = head;
		}
		else {
			head = (TYP *)TYP::Make(bt_short,4);
			tail = head;
		}
		break;
	}
	head->isUnsigned = isUnsigned;
	head->isVolatile = isVolatile;
	head->isIO = isIO;
	head->isShort = TRUE;
}

void Declaration::ParseLong()
{
	NextToken();
	if (lastst==kw_int) {
		head = (TYP*)TYP::Make(bt_long, cpu.sizeOfWord);
		tail = head;
		bit_max = head->precision;
		NextToken();
	}
	else if (lastst == kw_long) {
		//head = (TYP*)TYP::Make(bt_i128, cpu.sizeOfWord * 2);
		//bit_max = 128;
		head = (TYP*)TYP::Make(bt_long, cpu.sizeOfWord);
		tail = head;
		bit_max = 128;
		NextToken();
		if (lastst == kw_int)
			NextToken();
	}
	else if (lastst==kw_float) {
		head = (TYP *)TYP::Make(bt_double,cpu.sizeOfFPD);
		tail = head;
		bit_max = head->precision;
		NextToken();
	}
	else if (lastst==kw_double) {
		head = TYP::Copy(&stdquad);
		//head = (TYP *)TYP::Make(bt_quad,8);
		tail = head;
		bit_max = head->precision;
		NextToken();
	}
	else if (lastst == kw_posit) {
		head = (TYP*)TYP::Copy(&stdposit);
		tail = head;
		bit_max = head->precision;
		NextToken();
	}
	else {
		if (isUnsigned) {
			head =(TYP *)TYP::Make(bt_ulong,cpu.sizeOfWord);
			tail = head;
			bit_max = head->precision;
		}
		else {
			head = (TYP *)TYP::Make(bt_long,cpu.sizeOfWord);
			tail = head;
			bit_max = head->precision;
		}
	}
	//NextToken();
	if (lastst==kw_task) {
		isTask = TRUE;
		NextToken();
		bit_max = 64;
	}
	if (lastst==kw_oscall) {
		isOscall = TRUE;
		NextToken();
		bit_max = 64;
	}
	else if (lastst==kw_nocall || lastst==kw_naked) {
		isNocall = TRUE;
		NextToken();
		bit_max = 64;
	}
	head->isUnsigned = isUnsigned;
	head->isVolatile = isVolatile;
	head->isIO = isIO;
}

void Declaration::ParseInt(bool nt)
{
//printf("Enter ParseInt\r\n");
	if (isUnsigned) {
		head = TYP::Make(bt_uint, cpu.sizeOfInt);
		tail = head;
	}
	else {
		head = TYP::Make(bt_int, cpu.sizeOfInt);
		tail = head;
	}
	bit_max = cpu.sizeOfInt * 8;
	if (head==nullptr)
		return;
	head->isUnsigned = isUnsigned;
	head->isVolatile = isVolatile;
	head->isIO = isIO;
	if (nt)
		NextToken();
	head->size = head->precision >> 3;
	bit_max = head->precision;
	if (lastst==kw_vector) {
		TYP* btp = head;// ->GetIndex();
		head = TYP::Make(bt_vector,64);
		head->numele = 8;
		head->btpp = btp;
		tail = head;
		NextToken();
	}
	if (lastst==kw_task) {
		isTask = TRUE;
		NextToken();
	}
	if (lastst==kw_oscall) {
		isOscall = TRUE;
		NextToken();
	}
	else if (lastst==kw_nocall || lastst==kw_naked) {
		isNocall = TRUE;
		NextToken();
	}
//printf("Leave ParseInt\r\n");
}

void Declaration::ParseInline()
{
	ENODE* node;

	NextToken();
	if (lastst == openpa) {
		NextToken();
		inline_threshold = (short)GetIntegerExpression(&node, nullptr, 1).low;
		needpunc(closepa, 57);
	}
}

// Dead code, bits are not directly supported, bit-fields are.
void Declaration::ParseBit()
{
	//printf("Enter ParseInt\r\n");
	head = TYP::Make(bt_bit, cpu.sizeOfWord);
	tail = head;
	if (head == nullptr)
		return;
	head->isUnsigned = isUnsigned;
	head->isVolatile = isVolatile;
	head->isIO = isIO;
	head->isBits = true;
	NextToken();
	head->size = 1;
	bit_max = cpu.sizeOfWord;
}

void Declaration::ParseBool()
{
	//printf("Enter ParseInt\r\n");
	head = TYP::Make(bt_bool, cpu.sizeOfWord);
	tail = head;
	if (head == nullptr)
		return;
	head->isUnsigned = TRUE;
	head->isVolatile = isVolatile;
	head->isIO = isIO;
	head->isBits = true;
	NextToken();
	head->size = 1;
	head->precision = 8;
	bit_max = cpu.sizeOfWord;
}

void Declaration::ParseFloat(int prec)
{
	int64_t sz;

//	head = TYP::Copy(&stddouble);
	head = (TYP*)TYP::Make(bt_double, prec);
	tail = head;
	head->isVolatile = isVolatile;
	head->isIO = isIO;
	NextToken();
	ParsePrecisionSpec();
	if (lastst==kw_vector) {
		//int btp = head->GetIndex();
		TYP* btp = head;
		sz = head->size;
		head = TYP::Make(bt_vector,64);
		head->numele = 64/sz;
		head->precision = 512;
		head->lane_precision = sz;
		head->btpp = btp;
		tail = head;
		NextToken();
	}
	bit_max = head->precision;
}

void Declaration::ParseDouble()
{
	int64_t sz;

	head = (TYP *)TYP::Make(bt_double,8);
	tail = head;
	head->isVolatile = isVolatile;
	head->isIO = isIO;
	NextToken();
	if (lastst==kw_vector) {
		//int btp = head->GetIndex();
		TYP* btp = head;
		sz = head->size;
		head = TYP::Make(bt_vector,64);
		head->numele = 64/sz;
		head->precision = 512;
		head->btpp = btp;
		tail = head;
		NextToken();
	}
	bit_max = head->precision;
}

void Declaration::ParseDecimal()
{
	head = (TYP*)TYP::Make(bt_decimal, 16);
	head->isDecimal = true;
	tail = head;
	head->isVolatile = isVolatile;
	head->isIO = isIO;
	if (lastst == kw_vector) {
		//int btp = head->GetIndex();
		TYP* btp = head;
		head = TYP::Make(bt_vector, 64);
		head->numele = 4;
		head->precision = 512;
		head->btpp = btp;
		tail = head;
		NextToken();
	}
	bit_max = head->precision;
}

// ToDo: fixup posits
void Declaration::ParsePosit()
{
	head = (TYP*)TYP::Make(bt_posit, 8);
	tail = head;
	head->isVolatile = isVolatile;
	head->isIO = isIO;
	NextToken();
	if (lastch == '\'') {
		getch();
		NextToken();
		if (lastst == iconst) {
			head->precision = ival;
			head->size = head->precision / 8;
			NextToken();
			if ((head->precision & 7) != 0
				|| head->precision < 16
				|| head->precision > 128) {
				error(ERR_PRECISION);
				head->precision = cpu.sizeOfWord * 8;
			}
		}
		else
			error(ERR_INT_CONST);
	}
	if (lastst == kw_vector) {
		//int btp = head->GetIndex();
		TYP* btp = head;
		head = TYP::Make(bt_vector, 64);
		head->numele = 8;
		head->btpp = btp;
		tail = head;
		NextToken();
	}
	bit_max = head->precision;
}

// Dead code, triples are not supported.
void Declaration::ParseTriple()
{
	head = (TYP *)TYP::Make(bt_triple, 12);
	head->precision = 96;
	tail = head;
	head->isVolatile = isVolatile;
	head->isIO = isIO;
	NextToken();
	if (lastst == kw_vector) {
		TYP* btp = head;// ->GetIndex();
		head = TYP::Make(bt_vector, 64);
		head->numele = 5;
		head->btpp = btp;
		tail = head;
		NextToken();
	}
	bit_max = head->precision;
}

void Declaration::ParseFloat128()
{
	head = (TYP *)TYP::Make(bt_quad, 16);
	tail = head;
	head->precision = 128;
	head->isVolatile = isVolatile;
	head->isIO = isIO;
	NextToken();
	if (lastst == kw_vector) {
		//int btp = head->GetIndex();
		TYP* btp = head;
		head = TYP::Make(bt_vector, 64);
		head->numele = 4;
		head->btpp = btp;
		tail = head;
		NextToken();
	}
	bit_max = head->precision;
}

void Declaration::ParseVector(TABLE* table, Symbol** sym, e_sc sc)
{
	TYP* btp;
	bool is_float = true;

	NextToken();
	if (IsFloat((e_sym)lastst)) {
		ParseSpecifier(table, sym, sc);
		btp = head;
	}
	else if (IsScalar((e_sym)lastst)) {
		ParseSpecifier(table, sym, sc);
		is_float = false;
		btp = head;
	}
	else {
		btp = head = TYP::Make(bt_single, cpu.sizeOfFPS);
	}
	head->isVolatile = isVolatile;
	head->isIO = isIO;
	head = TYP::Make(bt_vector,64);
	head->numele = 64 / btp->size;
	if (is_float) {
		switch (btp->size) {
		case 16:	head->lane_precision = 'h'; break;
		case 32:	head->lane_precision = 's'; break;
		case 64:	head->lane_precision = 'd'; break;
		case 128:	head->lane_precision = 'q'; break;
		}
	}
	else {
		switch (btp->size) {
		case 16:	head->lane_precision = 'w'; break;
		case 32:	head->lane_precision = 't'; break;
		case 64:	head->lane_precision = 'o'; break;
		case 128:	head->lane_precision = 'h'; break;
		}
	}
	head->btpp = btp;
	tail = head;
	bit_max = head->precision;
}

void Declaration::ParseVectorMask()
{
	head = (TYP *)TYP::Make(bt_vector_mask,cpu.sizeOfWord);
	tail = head;
	head->isVolatile = isVolatile;
	head->isIO = isIO;
	head->numele = 0;
	NextToken();
	bit_max = head->precision;
}

void Declaration::ParseInt32()
{
	if (isUnsigned) {
		head = (TYP *)TYP::Make(bt_ushort,4);
		tail = head;
	}
	else {
		head = (TYP *)TYP::Make(bt_short,4);
		tail = head;
	}
	bit_max = 32;
	NextToken();
	if( lastst == kw_int )
		NextToken();
	head->isUnsigned = isUnsigned;
	head->isVolatile = isVolatile;
	head->isIO = isIO;
	head->isShort = TRUE;
}

void Declaration::ParseInt64()
{
	if (isUnsigned) {
		head = (TYP *)TYP::Make(bt_uint,cpu.sizeOfInt);
		tail = head;
	}
	else {
		head = (TYP *)TYP::Make(bt_int,cpu.sizeOfInt);
		tail = head;
	}
	bit_max = 64;
	NextToken();
	if( lastst == kw_int )
		NextToken();
	head->isUnsigned = isUnsigned;
	head->isVolatile = isVolatile;
	head->isIO = isIO;
	head->isShort = TRUE;
}

void Declaration::ParseChar()
{
	if (isUnsigned) {
		head = (TYP *)TYP::Make(isInline ? bt_iuchar : bt_uchar,2);
		tail = head;
	}
	else {
		head =(TYP *)TYP::Make(isInline ? bt_ichar : bt_char,2);
		tail = head;
	}
	head->isUnsigned = isUnsigned;
	head->isVolatile = isVolatile;
	head->isIO = isIO;
	NextToken();
	/*
	if (lastst == colon) {
		NextToken();
		if (lastst == iconst) {
			head->precision = ival;
			NextToken();
			if ((head->precision & 7) != 0
				|| head->precision > 32) {
				error(ERR_PRECISION);
				head->precision = 16;
			}
		}
		else
			error(ERR_INT_CONST);
	}
	*/
	if (lastst==kw_oscall) {
		isOscall = TRUE;
		NextToken();
	}
	if (lastst==kw_nocall || lastst==kw_naked) {
		isNocall = TRUE;
		NextToken();
	}
	bit_max = head->precision;
}

void Declaration::ParseInt8()
{
	if (isUnsigned) {
		head = (TYP *)TYP::Make(bt_ubyte,1);
		tail = head;
	}
	else {
		head =(TYP *)TYP::Make(bt_byte,1);
		tail = head;
	}
	head->isUnsigned = isUnsigned;
	head->isVolatile = isVolatile;
	head->isIO = isIO;
	NextToken();
	if (lastst==kw_oscall) {
		isOscall = TRUE;
		NextToken();
	}
	if (lastst==kw_nocall || lastst==kw_naked) {
		isNocall = TRUE;
		NextToken();
	}
	bit_max = head->precision;
}

void Declaration::ParseByte()
{
	if (isUnsigned) {
		head = (TYP *)TYP::Make(bt_ubyte,1);
		tail = head;
	}
	else {
		head =(TYP *)TYP::Make(bt_byte,1);
		tail = head;
	}
	NextToken();
	head->isUnsigned = !isSigned;
	head->isVolatile = isVolatile;
	head->isIO = isIO;
	bit_max = head->precision;
}

// The specifier id must be a type.
// 
Symbol *Declaration::ParseSpecifierId()
{
	Symbol *sp;

	dfs.printf("<ParseSpecifierId>%s",compiler.lastid);
	declid = new std::string(compiler.lastid);
	sp = tagtable.Find(compiler.lastid,false);//gsyms[0].Find(compiler.lastid);
	if (sp) {
		NextToken();
		dfs.printf("Found: type (struct, union or class).\n");
		if (sp->tp) {
			// The type has evaluated to this type, reset the head and tail.
			head = tail = sp->tp;
			return (sp);
		}
	}
	if (sp == nullptr) {
		sp = gsyms[0].Find(compiler.lastid, false);
		if (sp) {
			
			if (sp->storage_class != sc_typedef && sp->storage_class != sc_type) {
				return (nullptr);
			}
			
			head = tail = sp->tp;
			if (sp->tp) {
				NextToken();
				return(sp);
			}
		}
	}
	// The id was not found, allocate a symbol and assign the type. It may have
	// been just a symbol by itself without a type. Assume an integer.
	if (sp == nullptr) {
		declid = nullptr;
		sp = SymbolFactory::Make(
			compiler.lastid,
			TYP::Make(bt_int, cpu.sizeOfInt),
			nullptr,
			currentFn->depth,
			sc_type
		);
		if (sp == nullptr)
			throw new ArplException(ERR_OUT_OF_MEMORY, 0);
	}

	// Did we just have a symbol name by itself?
	if (head == nullptr) {
		head = (TYP*)TYP::Make(bt_int, cpu.sizeOfInt);
		tail = head;
		bit_max = head->precision;
	}
	sp->SetType(head);
//		head = tail = sp->tp;
//					head = tail = maketype(bt_long,4);
	NextToken();
	dfs.puts("</ParseSpecifierId>");
	return (sp);
}

void Declaration::ParseClass()
{
	ClassDeclaration cd;

	NextToken();
	cd.bit_max = bit_max;
	cd.bit_next = bit_next;
	cd.bit_offset = bit_offset;
	cd.bit_width = bit_width;
	cd.Parse(bt_class);
	cd.GetType(&head, &tail);
	bit_max = cd.bit_max;
	bit_next = cd.bit_next;
	bit_offset = cd.bit_offset;
	bit_width = cd.bit_width;
}

int Declaration::ParseStruct(TABLE* table, e_bt typ, Symbol **sp)
{
	StructDeclaration sd;
	int rv;

	NextToken();
	sd.bit_max = bit_max;
	sd.bit_next = bit_next;
	sd.bit_offset = bit_offset;
	sd.bit_width = bit_width;
	sd.isTypedef = isTypedef;
	rv = sd.Parse(table, typ, sp);
	sd.GetType(&head, &tail);
	bit_max = sd.bit_max;
	bit_next = sd.bit_next;
	bit_offset = sd.bit_offset;
	bit_width = sd.bit_width;
	return (rv);
}

void Declaration::ParseDeclspec()
{
	NextToken();
	needpunc(openpa, 74);
	NextToken();	// dllimport
	needpunc(closepa, 75);
}

bool Declaration::IsScalar(e_sym lastst)
{
	return (
		lastst == kw_int ||
		lastst == kw_long ||
		lastst == kw_char ||
		lastst == kw_byte ||
		lastst == kw_unsigned ||
		lastst == kw_short ||
		lastst == kw_enum ||
		lastst == kw_exception
		);
}

bool Declaration::IsFloat(e_sym lastst)
{
	return (
		lastst == kw_half ||
		lastst == kw_float ||
		lastst == kw_double ||
		lastst == kw_quad ||
		lastst == kw_single
		);
}

// Parse a specifier. This is the first part of a declaration.
// Returns:
// 0 usually, 1 if only a specifier is present
//
int Declaration::ParseSpecifier(TABLE* table, Symbol** sym, e_sc sc)
{
	Symbol *sp;
	Function* fn;
	ClassDeclaration cd;
	StructDeclaration sd;
	bool rv = false;
	int rv2;

	dfs.printf("<ParseSpecifier>\n");
	isUnsigned = FALSE;
	isSigned = FALSE;
	isVolatile = FALSE;
	isVirtual = FALSE;
	isIO = FALSE;
	isConst = FALSE;
	fn = nullptr;
	dfs.printf("A");
	for (;;) {
		switch (lastst) {

			case kw_declspec: ParseDeclspec(); break;
				
			case kw_const:		ParseConst();	break;
			case kw_typedef:	ParseTypedef(); break;
			case kw_nocall:
			case kw_naked:		ParseNaked();	break;

			case kw_oscall:
				isOscall = TRUE;
				head = tail = (TYP *)TYP::Make(bt_oscall,2);
				NextToken();
				goto lxit;
				
			case kw_user:
				NextToken();
				operating_mode = 0;
				break;
			case kw_supervisor:
				NextToken();
				operating_mode = 1;
				break;
			case kw_hypervisor:
				NextToken();
				operating_mode = 2;
				break;
			case kw_machine:
				NextToken();
				operating_mode = 3;
				break;

			case kw_interrupt:
				ParseInterrupt();
				goto lxit;

			case kw_coroutine:
				ParseCoroutine();
				break;

      case kw_virtual:
        dfs.printf("virtual");
        isVirtual = TRUE;
        NextToken();
        break;

			case kw_kernel:
				isKernel = TRUE;
				head =(TYP *) TYP::Make(bt_kernel,2);
				tail = head;
				NextToken();
				goto lxit;

			case kw_pascal:
				isPascal = TRUE;
				NextToken();
				break;
			case kw_cdecl:
				NextToken();
				isPascal = false;
				break;

			case kw_inline:
				ParseInline();
				break;

			case kw_register:
				isRegister = TRUE;
				NextToken();
				break;

			// byte and char default to unsigned unless overridden using
			// the 'signed' keyword
			//
			case kw_bit:	ParseBit(); goto lxit;
			case kw_bool:	ParseBool(); goto lxit;
			case kw_byte:   ParseByte(); goto lxit;
			case kw_char:	ParseChar(); goto lxit;
			case kw_int8:	ParseInt8(); goto lxit;
			case kw_int16:	ParseChar(); goto lxit;
			case kw_int32:	ParseInt32(); goto lxit;
			case kw_int64:	ParseInt64(); goto lxit;
			case kw_short:	ParseShort();	goto lxit;
			case kw_long:	ParseLong();	goto lxit;	// long, long int
			case kw_int:	ParseInt(true);		goto lxit;

      case kw_task:
        isTask = TRUE;
        NextToken();
				break;

			case kw_signed:
				isSigned = TRUE;
				NextToken();
				break;

			// unsigned may be used without a further type specifier, so assume an int
			// if none specified. Otherwise go back and process the token.
			case kw_unsigned:
				isUnsigned = TRUE;
				NextToken();
				if (IsScalar((e_sym)lastst))
					continue;
				ParseInt(false);
				goto lxit;

			case kw_volatile:
				NextToken();
				if (lastst==kw_inout) {
                    NextToken();
                    isIO = TRUE;
                }
				isVolatile = TRUE;
				break;

			case ellipsis:
				head = (TYP *)TYP::Make(bt_ellipsis,cpu.sizeOfWord);
				tail = head;
				head->isVolatile = isVolatile;
				head->isIO = isIO;
				NextToken();
				bit_max = cpu.sizeOfWord/8;
				goto lxit;

			case id:	
				sp = ParseSpecifierId();
				if (sym)
					*sym = sp;
//					NextToken();
				goto lxit;

			case kw_half: ParseFloat(16); goto lxit;
			case kw_single: ParseFloat(32); goto lxit;
			case kw_float:	ParseFloat(64); goto lxit;
			case kw_double:	ParseDouble(); goto lxit;
			case kw_float128:	ParseFloat128(); goto lxit;
			case kw_posit:	ParsePosit(); goto lxit;
			case kw_decimal: ParseDecimal(); goto lxit;

			case kw_triple:
				head = TYP::Copy(&stdtriple);
				tail = head;
				head->isVolatile = isVolatile;
				head->isIO = isIO;
				NextToken();
				bit_max = head->precision;
				goto lxit;

			case kw_vector:	ParseVector(table, sym, sc); goto lxit;
			case kw_vector_mask: ParseVectorMask(); goto lxit;

			case kw_void:	ParseVoid(); goto lxit;

			case kw_enum:
				NextToken();
				ParseEnum(table);
				bit_max = 16;
				goto lxit;

			case kw_class:
				ParseClass();
				goto lxit;

			case kw_struct:
				if (rv2 = ParseStruct(&tagtable, bt_struct, &sp)) {
					*sym = sp;
					rv = true;
					goto lxit;
				}
				*sym = sp;
				goto lxit;

			case kw_union:
				if (rv2 = ParseStruct(&tagtable, bt_union, &sp)) {
					*sym = sp;
					rv = true;
					goto lxit;
				}
				*sym = sp;
				goto lxit;

      case kw_exception:
				head = (TYP *)TYP::Make(bt_exception,8);
				tail = head;
				head->isVolatile = isVolatile;
				head->isIO = isIO;
				NextToken();
				bit_max = 64;
				goto lxit;
			
			case kw_attribute:
				ParseFunctionAttribute(fn,true);
				break;

			default:
				goto lxit;
			}
	}
lxit:;
	dfs.printf("</ParseSpecifier>\n");
	return (rv);
}

void Declaration::ParseDoubleColon(Symbol *sp)
{
	Symbol *sym;
	bool gotDouble = false;

	while (lastst==double_colon) {
		gotDouble = true;
		sym = tagtable.Find(compiler.lastid,false);
		if (sym) {
			sp->parent = sym->GetIndex();//gsearch(compiler.lastid);
			sp->parentp = sym;
		}
		else {
			sp->parent = 0;
			sp->parentp = nullptr;
			break;
		}
		NextToken();
		if (lastst != id) {
			error(ERR_IDEXPECT);
			break;
		}
	}
	if (gotDouble)
	    NextToken();
	//currentClass = sp->GetParentPtr();
	currentClass = sp->parentp;
	if (sp->parentp) {
		dfs.printf("Setting parent:%s|\r\n",
			(char*)sp->parentp->name->c_str());
	}
}

void Declaration::ParseBitfieldSpec(bool isUnion)
{
	dfs.puts("<ParseBitfieldSpec>");
	NextToken(1);
	switch (lastst) {
	case id: ParseVarAttribute(); return;
	case kw_align: ParseAlign(); return;
	case kw_long: bit_width = cpu.sizeOfInt * 16; NextToken(); break;
	case kw_short: bit_width = cpu.sizeOfInt * 4; NextToken(); break;
	case kw_quad: bit_width = cpu.sizeOfFPQ * 8; NextToken(); break;
	case kw_double: bit_width = cpu.sizeOfFPD * 8; NextToken(); break;
	case kw_single: bit_width = cpu.sizeOfFPS * 8; NextToken(); break;
	case kw_half: bit_width = cpu.sizeOfFPS * 4; NextToken(); break;
	case kw_byte: bit_width = 8; NextToken(); break;
	case kw_bit: bit_width = 1; NextToken(); break;
	default:
		bit_width = (int)GetIntegerExpression((ENODE**)NULL, nullptr, 0).low;
	}
	if (isUnion)
		bit_offset = 0;
	else
		bit_offset = bit_next;
	if (bit_width < 0 || bit_width > 128) {
		error(ERR_BITFIELD_WIDTH);
		bit_width = 1;
	}
	if (bit_width == 0 || bit_offset + bit_width > bit_max)
		bit_offset = 0;
	bit_next = bit_offset + bit_width;
	dfs.puts("</ParseBitfieldSpec>\n");
}

void Declaration::ParseVarAttribute()
{
	do {
		if (strcmp(compiler.lastid, "_cache") == 0) {
			NextToken();
			needpunc(double_colon, 86);
			if (lastst == id) {
				if (strcmp(compiler.lastid, "_none") == 0) {
					head->rd_cache = new std::string(".none");
					head->wr_cache = new std::string(".none");
				}
				else if (strcmp(compiler.lastid, "_rd") == 0) {
					head->rd_cache = new std::string(".rd");
					head->wr_cache = new std::string(".none");
				}
				else if (strcmp(compiler.lastid, "_rda") == 0) {
					head->rd_cache = new std::string(".rda");
					head->wr_cache = new std::string(".none");
				}
				else if (strcmp(compiler.lastid, "_wt") == 0) {
					head->rd_cache = new std::string(".rd");
					head->wr_cache = new std::string(".wt");
				}
				else if (strcmp(compiler.lastid, "_wta") == 0) {
					head->rd_cache = new std::string(".rd");
					head->wr_cache = new std::string(".wta");
				}
				else if (strcmp(compiler.lastid, "_rwta") == 0) {
					head->rd_cache = new std::string(".rda");
					head->wr_cache = new std::string(".wta");
				}
				else if (strcmp(compiler.lastid, "_wb") == 0) {
					head->rd_cache = new std::string(".rd");
					head->wr_cache = new std::string(".wb");
				}
				else if (strcmp(compiler.lastid, "_wba") == 0) {
					head->rd_cache = new std::string(".rd");
					head->wr_cache = new std::string(".wba");
				}
				else if (strcmp(compiler.lastid, "_rwba") == 0) {
					head->rd_cache = new std::string(".rda");
					head->wr_cache = new std::string(".wba");
				}
				NextToken();
			}
		}
		if (lastst != comma)
			break;
		NextToken();
	} while (1);
}

void Declaration::ParseAlign()
{
	NextToken();
	head->align = GetIntegerExpression(nullptr, nullptr, 0).low;
}

void Declaration::ParsePrecisionSpec()
{
	dfs.puts("<ParsePrecisionSpec>");
	switch (lastst) {
	case kw_long: head->precision = 128; NextToken(); break;
	case kw_short: head->precision = 32; NextToken(); break;
	case kw_quad: head->precision = 128; NextToken(); break;
	case kw_double: head->precision = 64; NextToken(); break;
	case kw_single: head->precision = 32; NextToken(); break;
	case kw_half: head->precision = 16; NextToken(); break;
	case kw_byte: head->precision = 8; NextToken(); break;
	default:;
//		error(ERR_BAD_PRECISION);
	}
	dfs.puts("</ParsePrecisionSpec>\n");
}

Symbol *Declaration::ParsePrefixId(Symbol* symi)
{
	Symbol *sp;
	Expression exp;

	dfs.puts("<ParsePrefixId>");
	if (declid) delete declid;
	declid = new std::string(compiler.lastid);
	dfs.printf("B|%s|",(char *)declid->c_str());
	sp = exp.gsearch2(std::string(compiler.lastid), bt_int, nullptr, false);
	if (symi) {
		sp = symi;
	}
	else {
		if (sp == nullptr)
			sp = Symbol::alloc();
		dfs.printf("C");
		if (funcdecl == 1) {
			if (currentFn)
				sp->fi = currentFn;
			else
				sp->fi = MakeFunction(sp->id, sp, defaultcc == 1, false);
			if (nparms > 19)
				error(ERR_TOOMANY_PARAMS);
			else {
				names[nparms].str = *declid;
				nparms++;
			}
		}
	}
	dfs.printf("D"); 
	NextToken();
	ParseDoubleColon(sp);
	if (declid) delete declid;
	declid = new std::string(compiler.lastid);
	sp->SetName(*declid);
	dfs.printf("E"); 
	if (lastst == colon) {
		ParseBitfieldSpec(isUnion);
		goto lxit;	// no ParseDeclarationSuffix()
	}
	goto lxit;
	sp = ParseSuffix(sp);
lxit:
	dfs.puts("</ParsePrefixId>");
	return (sp);
}

// Dead Code
/*
Symbol *Declaration::ParsePrefixOpenpa(bool isUnion, Symbol* symi)
{
	TYP *temp1, *temp2, *temp3, *temp4;
	Symbol *sp;
	Function* fn;

	dfs.puts("<ParsePrefixOpenpa>\n");
	NextToken();
	temp1 = head;
	temp2 = tail;
	head = tail = (TYP *)NULL;	// It might be a typecast following.
	// Do we have (getchar)()
	// This processing is difficult to do with a loop, so a recursive
	// call is made.
	fn = currentFn;
	//declare(symi, TABLE * table, sc_none, 0, bt_struct, &sp, local);
	declare(symi, 0, bt_struct, &sp, symi->parentp!=nullptr);
	currentFn = fn;
//	sp = ParsePrefix(isUnion, symi); 
//	needpunc(closepa,20);
	// Head could be NULL still if a type hasn't been found
	// eg. int (getchar)();
	if (head)
		isFuncPtr = head->type == bt_pointer;
	goto xit;
	temp3 = head;
	temp4 = tail;
	//if (isFuncPtr) {
	//	head->btp = temp1->GetIndex();
	//}
	//else
	{
		head = temp1;
		tail = temp2;
	}
	sp = ParseSuffix(sp);
	// (getchar)() returns temp4 = NULL
	if (temp4!=NULL) {
		temp4->btp = head->GetIndex();
		temp4->btpp = head;
		if(temp4->type == bt_pointer && temp4->val_flag != 0 && head != NULL)
			temp4->size *= head->size;
		head = temp3;
	}
xit:
	dfs.puts("</ParsePrefixOpenpa>\n");
	return (sp);
}
*/

Symbol* Declaration::CreateNonameVar()
{
	static int varno = 0;
	char nmbuf[300];
	Symbol* sp;

	sprintf_s(nmbuf, sizeof(nmbuf), "__noname_var%d", varno);
	sp = Symbol::alloc();
	sp->SetName(nmbuf);
	sp->tp = head;
	sp->storage_class = sc_member;
	varno++;
	return (sp);
}

// There may be only a single identifier in the prefix. This identifier may
// contain a class spec or namespace spec.

TYP *Declaration::ParsePrefix(bool isUnion, Symbol* symi, bool local, Symbol** os)
{   
	TYP *temp1, *temp2, *temp3;
	Symbol *sp, *symo, *sp1;
	ParameterDeclaration pd;
	static int level = 0;

	dfs.printf("<Declaration_ParsePrefix>(%d)\n",lastst);
	sp = nullptr;
j1:
	switch (lastst) {

	case kw_const:
		dfs.printf("C");
		isConst = TRUE;
		NextToken();
		goto j1; 

	case semicolon:
		dfs.printf(";");
		sp = CreateNonameVar();
		break;

		//		case ellipsis:
	case id:
		dfs.printf("I");
		sp1 = sp = ParsePrefixId(symi);
		if (lastst == semicolon) {
			goto lxit;
		}
		if (lastst == comma) {
			goto lxit;
		}
		if (lastst == closepa) {
			goto lxit;
		}
		if (lastst == dot || lastst == pointsto) {
			window_pos = 0;
			NextToken();
			goto lxit;
		}
		sp = ParseSuffix(sp);
		isFuncPtr = false;
		lastst;
		/*
		if (lastst == begin)
			needParseFunction = 2;
		*/
		goto lxit;

	case star:
		dfs.printf("*");
		bit_max = cpu.sizeOfPtr * 8;
		dfs.putch('*');
		NextToken();
		temp2 = head;
		temp3 = tail;
		head = TYP::Make(bt_pointer, cpu.sizeOfPtr);
		head->btpp = temp2;
		if (tail == nullptr)
			tail = head;
		if (pa_level > 0)
			isFuncPtr = true;
		goto j1;

	case openpa:
		dfs.printf("(");
		/*
		if (os && (*os)->storage_class == sc_type) {
			(*os)->tp->btpp = (*os)->tp;
			(*os)->tp = TYP::Copy(&stdfunc);
		}
		*/
		level++;
		pa_level++;
//		if (!IsDeclBegin(lastst) || true) {
		do {
			Function* fn = currentFn;
			TABLE* table;
			NextToken();
//			NextToken();
			if (fn) {
				if (fn->body == nullptr) {
					Statement* bdy;
					fn->body = bdy = cg.stmt->MakeStatement(st_compound, 0);
					fn->sym->stmt = bdy;
				}
				table = &currentFn->body->ssyms;
			}
			else
				table = &gsyms[0];
			//currentFn->params = *pd.Parse(funcdecl, false, currentFn);
			//symo = nullptr;
			// declare() will process a list of comma separated items.
			declare(symi, table, fn->sym->parent ? sc_member : sc_global, 0, 0, &symo, local, currentFn ? currentFn->depth : 0);
			lastst;
		} while (lastst == semicolon || lastst == comma);
		/*
		else {
			ParsePrefix(isUnion, symi, local, &sp);
			symo = sp;
		}
		*/
		level--;
		funcdecl = 0;
		sp = symo;
		//funcdecl--;
		//sp = ParsePrefixOpenpa(isUnion, symi);
		if (lastst != closepa) {
			error(ERR_SYNTAX);
		}
		if (lastst == closepa) {
			pa_level--;
			NextToken();
			if (lastst == begin) {
				needParseFunction = 2;
				//currentFn->body = cg.stmt->ParseCompound(true);
				goto lxit;
			}
		}
		else if (lastst == semicolon)
			NextToken();
		/*
		if (currentStmt) {
			if (currentStmt->stype == st_compound) {
				if (level == 0) {
					if (lastst == openpa) {
						NextToken();
						pd.Parse(1, true);
						needpunc(closepa, 60);
					}
				}
			}
		}
		else
			*/
		{
			lastst;
			//sp = 
			temp2 = head;
			temp3 = tail;
				sp = ParseSuffix(sp);
				head = temp2;
				tail = temp3;
			//sp = symo;
			//temp1 = TYP::Make(bt_pointer, cpu.sizeOfPtr);
			//temp1->btp = head->GetIndex();
			//temp1->btpp = head;
			//head = temp1;
			//if (tail == NULL)
			//	tail = head;
			lastst;
			if (lastst == begin)
				needParseFunction = 2;
		}

		//if (symo == nullptr) {
		//	if (lastst == closepa)
		//		NextToken();
		//}
		//else if (!symo->fi)
		//	needpunc(closepa, 60);
		//else if (!symo->fi->hasParameters)
		//	needpunc(closepa, 60);
		goto lxit;

	default:
		dfs.printf("D");
		if (lastst == semicolon) {
			NextToken();
			goto lxit;
		}
		if (lastst == closepa)
			goto lxit;
		sp = ParseSuffix(sp);
		dfs.printf("d");
		goto lxit;
	}
lxit:
	// Strip out extra "Func returns Func" due to (((
	if (head) {
		/*
		while (head->btpp && (head->btpp->type == bt_func || head->btpp->type == bt_ifunc)) {
			if (tail == head)
				tail = head->btpp;
			head = head->btpp;
		}
		*/
	}
	dfs.puts("</ParseDeclPrefix>\n");
	if (os)
		*os = sp;
	return (head);
}


// Take care of trailing [] in a declaration. These indicate array indicies.

void Declaration::ParseSuffixOpenbr()
{
	TYP *temph, *tempt;
	int64_t sz2;
	TYP* dimen[20];
	int nn, kk;

	temph = head;
	tempt = tail;
	for (nn = 0; lastst == openbr; nn++) {
		NextToken();
		// We allow a vector type variable to specify the working number of elements,
		// which is an optional specification. Empty brackets mean to use the full
		// size of the vector.
		if (head->type == bt_vector) {
			if (lastst != closebr) {
				sz2 = GetIntegerExpression((ENODE**)NULL, nullptr, 1).low;
				head->numele = sz2;
				needpunc(closebr, 22);
				return;
			}
			else {
				NextToken();
				return;
			}
		}
		dimen[nn] = (TYP*)TYP::Make(bt_pointer, cpu.sizeOfPtr);
		dimen[nn]->val_flag = true;
		dimen[nn]->isArray = true;
		dimen[nn]->btpp = nullptr;
		if (lastst != closebr) {
			sz2 = GetIntegerExpression((ENODE**)NULL, nullptr, 1).low;
			dimen[nn]->size = head->size;
			dimen[nn]->numele = sz2;
			needpunc(closebr, 22);
		}
		else {
			dimen[nn]->size = head->size;
			dimen[nn]->numele = 0;
			dimen[nn]->unknown_size = true;
			NextToken();
			break;
		}
	}
	for (kk = nn-1; kk > 0; kk--) {
		dimen[kk]->dimen = nn - kk;
		dimen[kk]->size = dimen[kk]->size * dimen[kk]->numele;
		dimen[kk - 1]->size = dimen[kk]->size;
	}
	dimen[0]->size = dimen[0]->size * dimen[0]->numele;
	//	if (nn == 1)
	head = tail = dimen[0];
	for (kk = 0; kk < nn; kk++) {
		tail->btpp = dimen[kk];
		tail = dimen[kk];
	}
	tail->btpp = temph;
	tail = temph;
}

/*
void Declaration::ParseSuffixOpenbr()
{
	TYP *temp1;
	long sz2;

	NextToken();
	temp1 = (TYP *)TYP::Make(bt_pointer, cpu.sizeOfPtr);
	temp1->val_flag = 1;
	temp1->isArray = TRUE;
	if (tail)
		tail->btp = temp1->GetIndex();
	if (lastst == closebr) {
		temp1->size = 0;
		temp1->numele = 0;
		if (tail)
			temp1->dimen = tail->dimen + 1;
		else
			temp1->dimen = 1;
		NextToken();
	}
	else if (tail != NULL) {
		sz2 = (int)GetIntegerExpression((ENODE **)NULL);
		temp1->size = sz2 * head->size;
		temp1->numele = sz2;
		temp1->dimen = tail->dimen + 1;
		dfs.printf("Setting array size:%d\n", (int)temp1->size);
		temp1->alignment = tail->alignment;
		needpunc(closebr, 21);
	}
	else {
		sz2 = (int)GetIntegerExpression((ENODE **)NULL);
		temp1->size = sz2;
		temp1->numele = sz2;
		temp1->dimen = 1;
		needpunc(closebr, 22);
	}
	if (head == nullptr)
		head = temp1;
	tail = temp1;
}
*/
void Declaration::ParseFunctionAttribute(Function *sym, bool needpa)
{
	NextToken();
	if (needpa)
		needpunc(openpa,0);
	do {
		switch(lastst) {
		case openpa:
			ParseFunctionAttribute(sym, false);
			needpunc(closepa, 73);
			break;

		case kw_no_temps:
			sym->UsesTemps = false;
			NextToken();
			break;
		/*
		case kw_no_parms:
			sym->UsesStackParms = false;
			NextToken();
			break;
		*/
		default:
			NextToken();
		}
	} while (lastst==comma);
	if (needpa)
		needpunc(closepa,0);
}

// We get here after '(' is encountered and there wasn't a ')' next.
// If we're in the process of processing a parameter list then a function was 
// encountered in the parameter list. The parameters for that function need to
// be processed.

Function* Declaration::ParseFunctionJ2(Function* sp)
{
	int fd;
	std::string odecl;
	TYP* tempHead, * tempTail;
	int isd;
	int nump = 0;
	int numa = 0;
	int ellipos = -1;
	Function* cf;
	Symbol* sym;
	ParameterDeclaration pd;

	dfs.printf("r");
	//cf = currentFn;
 // currentFn = sp;
	dfs.printf("s");
	// Parse the parameter list for a function pointer passed as a
	// parameter.
	// Parse parameter list for a function pointer defined within
	// a structure.
	if (parsingParameterList || isStructDecl) {
		//sym = Symbol::alloc();
		//sym->fi = MakeFunction(sym->id, sym, defaultcc == 1, isInline);
		//sym->SetName(*UnknownFuncName());
		dfs.printf("s ");
		fd = funcdecl;
		needParseFunction = 0;
		dfs.printf("Set false\n");
		if (declid)
			odecl = *declid;
		else
			odecl = "";
		tempHead = head;
		tempTail = tail;
		isd = isStructDecl;
		head = tail = nullptr;
		//ParseParameterDeclarations(10);	// parse and discard
		funcdecl = 10;
		//				SetType(sp);
		cf = currentFn;
		currentFn = sp;
		nump = sp->NumParms;
		sp->BuildParameterList(&nump, &numa, &ellipos);
		currentFn = cf;
		needParseFunction = 0;
		dfs.printf("Set false\n");
		//				sp->parms = sym;
		sp->NumParms = nump;
		if (ellipos >= 0)
			sp->NumFixedAutoParms = ellipos + 1;
		else
			sp->NumFixedAutoParms = nump;
//		if (cf != nullptr)
//			sp->params.AddTo(&cf->params);
		isStructDecl = isd;
		head = tempHead;
		tail = tempTail;
//		if (declid) delete declid;
		declid = new std::string(odecl);
		funcdecl = fd;
		// There may be more parameters in the list.???
		if (lastst == comma) {
			return (sp);
		}
		needpunc(closepa, 23);
		while (lastst == kw_attribute)
			ParseFunctionAttribute(sp,true);
		if (lastst == assign)
			return (sp);

		if (lastst == begin) {
			needParseFunction = 2;
			dfs.printf("Set true1\n");
			if (sp->params.headp && sp->proto.headp) {
				dfs.printf("Matching parameter types to prototype.\n");
				if (!sp->ParameterTypesMatch(sp))
					error(ERR_PARMLIST_MISMATCH);
			}
			//temp1->type = bt_ifunc;
		}
		// Could be a function prototype in a parameter list followed by a comma.
		else if (lastst == comma && parsingParameterList > 0) {
			sp->params.CopyTo(&sp->proto);
			return (sp);
		}
		// If the declaration is ending in a semicolon then it was really
		// a function prototype, so move the parameters to the prototype
		// area.
		else if (lastst == semicolon || lastst == closepa) {
			sp->params.CopyTo(&sp->proto);
			sp->IsPrototype = true;
		}
		else {
			if (funcdecl > 0 && lastst == closepa)
				;
			else
				error(ERR_SYNTAX);
		}
		dfs.printf("Z\r\n");
		//				if (isFuncPtr)
		//					temp1->type = bt_func;
		//				if (lastst != begin)
		//					temp1->type = bt_func;
		//				if (lastst==begin) {
		//					ParseFunction(sp);
		//				}
		return (sp);
	}
	/*
	else {
		int ppl = parsingParameterList;
		parsingParameterList = false;
		cf = currentFn;
		currentFn = sp;
		sp->BuildParameterList(&nump, &numa);
		parsingParameterList = ppl;
		currentFn = cf;
		sp->NumParms = nump;
		sp->numa = numa;
		//needpunc(closepa,23);
		if (lastst == semicolon) {
			sp->params.CopyTo(&sp->proto);
			needParseFunction = false;
		}
	}
	*/
	needpunc(closepa,63);
	dfs.printf("Y");
	sp->PrintParameterTypes();
	dfs.printf("X");
	return (sp);
}

// Take care of following open parenthesis (). These indicate a function
// call. There may or may not be following parameters. A following '{' is
// looked for and if found a flag is set to parse the function body.

Function* Declaration::ParseSuffixOpenpa(Function *sp)
{
	TYP *temp1, *temp2, *temp3;
	TYP *tempHead, *tempTail;
	int fd;
	std::string odecl;
	int isd;
	int nump = 0;
	int numa = 0;
	
	dfs.printf("<openpa>\n");
	dfs.printf("****************************\n");
	dfs.printf("****************************\n");
	dfs.printf("Function: %s\n", (char *)sp->sym->name->c_str());
	dfs.printf("****************************\n");
	dfs.printf("****************************\n");
	NextToken();
	sp->IsPascal = isPascal;
	sp->IsInline = isInline;
	sp->IsFar = isFar;
	sp->IsCoroutine = isCoroutine;
	sp->IsUnknown = true;
	/*
	if (currentClass == nullptr) {
		if (!gsyms->Find(sp->sym->name->c_str()))
			gsyms->insert(sp->sym);
	}
	else {
		if (!gCurrentTbl->Find(sp->sym->name->c_str()))
			gCurrentTbl->insert(sp->sym);
	}
	*/
	// An asterik before the function name indicates a function pointer but only
	// if it's bracketed properly, otherwise it could be the return value that's
	// a pointer.
	isFuncPtr = head ? head->type==bt_pointer : false;
	temp1 =(TYP *) TYP::Make(bt_func,0/*isFuncPtr ? bt_func : bt_ifunc,0*/);
	temp1->val_flag = true;
	dfs.printf("o ");
	/*
	if (isFuncPtr) {
		dfs.printf("Got function pointer in declarations.\n");
		temp2 = (TYP *)TYP::Make(bt_pointer, 0);
		//temp1->btp = head->btp;
		//head->btp = temp1->GetIndex();
		temp1->btp = head->GetIndex();
		temp1->btpp = head;
		temp2->btp = temp1->GetIndex();
		temp2->btpp = temp1;
		head = temp1;
	}
	else
	*/
	{
		temp1->btpp = head;
		head = temp1;
		//tail->btpp = temp1;
		//tail = temp1;
	}
	dfs.printf("p ");
	if (tail==nullptr) {
			tail = head;
	}
	dfs.printf("q ");
	needParseFunction = 1;
	sp->params.Clear();
	sp->sym->parentp = currentClass;// ->GetIndex();
	sp->sym->tp = head;
	if(lastst == closepa) {
		NextToken();
		while (lastst == kw_attribute)
			ParseFunctionAttribute(sp,true);
	  if(lastst == begin) {
		  temp1->type = bt_ifunc;
		  needParseFunction = 2;
		}
	  //else {
		 // if (lastst != semicolon) {
			//	sp = ParseFunctionJ2(sp);
			//	return (sp);
			//}
	  //  temp1->type = bt_func;
		 // needParseFunction = 0;
		 // dfs.printf("Set false\n");
	  //}
		if (sp) {
			currentFn = sp;
			sp->NumParms = 0;
			sp->numa = 0;
		}
  }
	else {
		sp = ParseFunctionJ2(sp);
	}
  dfs.printf("</openpa>\n");
	return(sp);
}


// Take care of the () or [] trailing part of a declaration.
// There could be multiple sets of [] so a loop is formed to accomodate
// this. There will be only a single set of () indicating parameters.

Symbol *Declaration::ParseSuffix(Symbol *sp)
{
	TYP* tp;
	ENODE* node;
	Symbol* sp1;

	dfs.printf("<ParseDeclSuffix>\n");

	sp1 = sp;
	while(true) {
		switch (lastst) {

		case openbr:
			dfs.printf("[");
			ParseSuffixOpenbr();
			break;                // We want to loop back for more brackets

		case openpa:
			dfs.printf("(");
			// The declaration doesn't have to have an identifier name; it could
			// just be a type chain. so sp incoming might be null. We need a place
			// to stuff the parameter / protoype list so we may as well create
			// the symbol here if it isn't yet defined.
			if (sp == nullptr) {
				sp1 = Symbol::alloc();
				sp1->fi = MakeFunction(sp1->number, sp1, defaultcc == 1, isInline);
//				sp1->SetName(*UnknownFuncName());
				sp1->SetName(std::string(compiler.lastid));
				sp1->tp = TYP::Copy(&stdfunc);
				sp1->tp->type = bt_func;
				sp1->tp->btpp = head;
				sp1->storage_class = sc_global;
				sp1->segment = codeseg;
			}
			else if (sp->fi == nullptr) {
				TYP* ty = sp->tp;
				ty = head;
				head = TYP::Copy(&stdfunc);
				head->btpp = ty;
				sp->fi = MakeFunction(sp1->number, sp1, defaultcc == 1, isInline);
				sp->SetName(*sp->name);
				sp->tp = head;
				sp->tp->type = bt_func;
				sp->tp->btpp = head->btpp;
				sp->storage_class = sp->parentp ? sc_member : sc_global;
				sp->segment = codeseg;
				sp1 = sp;
			}
			func = sp1->fi;
			//else {
			//	sp1 = Symbol::alloc();
			//	sp1->fi = MakeFunction(sp1->number, sp1, defaultcc == 1, isInline);
			//	sp1->name = UnknownFuncName();
			//	sp1->parent = sp->id;
			//}
			//sp1->tp = head;
			parsingParameterList++;
			sp1->fi = ParseSuffixOpenpa(sp1->fi);
			parsingParameterList--;
			goto lxit;

		case assign:
			dfs.printf("=");
			if (sp1)
				if (sp1->tp == nullptr)
					sp1->SetType(head);
			if (parsingParameterList) {
				NextToken();
				currentSym = sp1;
				SetType(sp1);
				GetConstExpression(&node, sp1);
				sp1->defval = node;
			}
			goto lxit;
		
		default:
			goto lxit;
		}
	}
lxit:
	dfs.printf("</ParseDeclSuffix>\n");
	return (sp1);
}

void Declaration::AssignParameterName()
{
	char buf[20];

	sprintf_s(buf, sizeof(buf), "__p%d", nparms);
	delete declid;
	declid = new std::string(buf);
	if (nparms > 19) {
		error(ERR_TOOMANY_PARAMS);
	}
	else {
		names[nparms].str = *declid;
		nparms++;
	}
	missingArgumentName = TRUE;
}


void Declaration::ParseAssign(Symbol *sp)
{
	TYP *tp1, *tp2;
	enum e_node op;
	ENODE *ep1, *ep2;
	Expression exp(cg.stmt);
	exp.head = head;
	exp.tail = tail;
	bool madenode;

	dfs.printf("<ParseAssign>");

	if (parsingParameterList) {
		GetConstExpression(&ep2, sp);
		sp->defval = ep2;
	}
	else {
		if (sp) {
			if (sp->tp->isArray && !sp->IsExternal) {
				doinit(sp, true);
				dfs.printf("</ParseAssign>");
				return;
			}
		}
		NextToken();
		ep1 = exp.MakeAutoNameNode(sp);
		if (ep1 == nullptr) {
			dfs.printf("</ParseAssign>");
			return;
		}
		ep1->constflag = false;
		ep1->sym = sp;
		tp1 = exp.CondAddRef(&ep1, sp->tp);
		//tp1 = exp.nameref(&ep1, TRUE);
		op = en_assign;
		ep2 = nullptr;
		tp2 = exp.ParseAssignOps(&ep2, sp);
		ep2->i_rhs = ep1->i;
		ep1->i_lhs = ep2->i;
		if (tp2 == nullptr || !ep1->IsLValue())
			error(ERR_LVALUE);
		else {
//			tp1 = 
				forcefit(&ep2, tp2, &ep1, tp1, false, true);
			ep1 = makenode(op, ep1, ep2);
			ep1->tp = tp1;
		}
		// Move vars with initialization data over to the data segment.
		if (ep1->segment == bssseg)
			ep1->segment = dataseg;
		sp->segment = (e_sg)ep1->segment;
		sp->initexp = makenode(en_void, nullptr, ep1);
	}
	dfs.printf("</ParseAssign>");
}


// Processing done when the end of a declaration (;) is reached.

void Declaration::DoDeclarationEnd(Symbol *sp, Symbol *sp1)
{
	int nn;
	TYP *tp1;
	ENODE *ep1, *ep2;
	Expression exp(cg.stmt);

	if (sp == nullptr)
		return;
	if (sp->tp == nullptr)
		return;
	if (sp->tp->type == bt_class && (sp->storage_class != sc_type
		&& sp->storage_class != sc_typedef)) {
		nn = sp->tp->lst.FindRising(*sp->tp->sname);
		if (nn > 0) {
			if (sp1) {
				ep1 = nullptr;
				// Build an expression that references the ctor.
				tp1 = exp.nameref2(*sp->tp->sname, &ep1, TRUE, false, nullptr, nullptr, sp);
				// Create a function call node for the ctor.
				if (tp1 != nullptr) {
					// Make an expresison that references the var name as the
					// argument to the ctor.
					ep2 = makesnode(en_nacon, sp->fi ? sp->GetFullName() : sp->name, sp->mangledName, sp->value.i);
					ep1 = makenode(en_fcall, ep1, ep2);
					//ep1->p[2] = tp1;
				}
				sp->initexp = makenode(en_void, nullptr, ep1);
			}
		}
		/*
			// First see if there is a ctor. If there are no ctors there's
			// nothing to do.
			memset(typearray,0,sizeof(typearray));
			sp1 = search2(sp->tp->sname,&sp->tp->lst,typearray);
			if (sp1) {
				// Build an expression that references the ctor.
				tp1 = nameref2(sp->tp->sname,&ep1,TRUE,false,typearray);
				// Create a function call node for the ctor.
				if (tp1!=nullptr) {
					memcpy(typearray,GetParameterTypes(sp),sizeof(typearray));
					ep1 = makenode(en_fcall, ep1, nullptr);
				}
				sp->initexp = ep1;
			}
		*/
	}
}

void Declaration::DoInsert(Symbol *sp, TABLE *table)
{
	dfs.printf("<DoInsert>");
	if ((sp->tp->type == bt_class)
		&& (sp->storage_class == sc_type || sp->storage_class == sc_typedef))
		; // Do nothing. The class was already entered in the tag table.
	else if ((sp->tp->type == bt_struct || sp->tp->type == bt_union)
		&& (sp->storage_class == sc_type && sp->storage_class != sc_typedef)
		&& sp->tp->sname->length() > 0)
		; // If there was no struct tag and this is a typedef, then it
			// still needs to be inserted into the table.
	else {
		dfs.printf("insert type: %d\n", sp->tp->type);
		dfs.printf((char *)"***Inserting:%s into %p\n", (char *)sp->name->c_str(), (char *)table);
		// Need to know the type before a name can be generated.
		if (sp->tp->type == bt_func || sp->tp->type == bt_ifunc)
			if (sp->fi)
				sp->mangledName = sp->BuildSignature(!sp->fi->IsPrototype);
		if (sp->parentp && ((sp->tp->type == bt_func || sp->tp->type == bt_ifunc)
			|| (sp->tp->type == bt_pointer && (sp->tp->btpp->type == bt_func || sp->tp->btpp->type == bt_ifunc))))
		{
			//insState = 1;
//			if (sp->fi == nullptr)
//				sp->fi = MakeFunction(sp->id, sp, defaultcc == 1, false);

			if (sp->fi) {
				sp->fi->InsertMethod();
				sp->fi->depth = sp->depth;
			}
			// A pointer to a function still needs to be placed in the symbol table. It
			// does not need to be as a method.
			else if (sp->tp->type == bt_pointer) {
				Symbol* s;
				if ((s = table->insert(sp)) != sp) {
				}
			}
		}
		else {
			//insState = 2;
			// Inserts ultimately coming from statement ssyms.
			if (sp->IsTypedef())
				tagtable.insert(sp);
			else {
				Symbol* sp1;
				if (sp1 = sp->Find(*sp->name)) {
				}
				Symbol* s;
				if ((s = table->insert(sp)) != sp) {
				}
			}
		}
	}
	dfs.printf("</DoInsert>\n");
}

Symbol *Declaration::FindSymbol(Symbol *sp, TABLE *table)
{
	Symbol *sp1;
	Function *fn;
	TypeArray* ta;

	dfs.printf("<FindSymbol>");
	sp1 = nullptr;
	//dfs.printf("Table:%p, sp:%p Fn:%p\r\n", table, sp, currentFn);
	if (sp) {
		int nn;
		// If a function body is being processed we want to look for
		// symbols by rising through the hierarchy. Otherwise we want a
		// lower level defined symbol to shadow one at a hight level.
		if (isFuncBody && sp->parentp) {
			nn = sp->parentp->tp->lst.FindRising(*sp->name);
			if (nn)
				sp1 = sp->parentp->FindRisingMatch(false);
			else {
				nn = sp->parentp->lsyms.Find(*sp->name);
				if (nn) {
					Function* fn = sp->fi->FindExactMatch(TABLE::matchno);
					if (fn) sp1 = fn->sym;
				}
			}
		}
		else {
			nn = sp->tp->lst.Find(*sp->name);
			if (nn) {
				Function* fn = sp->fi->FindExactMatch(TABLE::matchno);
				if (fn) sp1 = fn->sym;
			}
			else {
				nn = sp->lsyms.Find(*sp->name);
				if (nn) {
					Function* fn = sp->fi->FindExactMatch(TABLE::matchno);
					if (fn) sp1 = fn->sym;
				}
			}
		}
		if (nn == 0) {
			nn = table->FindRising(*sp->name);
			if (nn)
				sp1 = sp->FindRisingMatch(false);
		}
	}
	else {
		sp1 = table->Find(*sp->name, false);
		if (TABLE::matchno > 1) {
		}
	}
	dfs.printf("h");
	if (sp->tp) {
		dfs.printf("h1");
		if (sp->tp->type == bt_ifunc || sp->tp->type == bt_func) {
			dfs.printf("h2");
			fn = sp->fi->FindExactMatch(TABLE::matchno);
			if (fn)
				sp1 = fn->sym;
			else
				sp1 = nullptr;
			dfs.printf("i");
		}
	}
	else {
		dfs.printf("j");
		if (TABLE::matchno)
			sp1 = TABLE::match[TABLE::matchno - 1];
		else
			sp1 = nullptr;
	}
	dfs.printf("</FindSymbol>\n");
	return (sp1);
}

int Declaration::ParseFunction(TABLE* table, Symbol* sp, Symbol* parent, e_sc al, bool local)
{
	Symbol* sp1;
	bool flag;
	bool fn_doneinit = false;
	Function* ofn;
	Function* fn;
	
	if (sp != nullptr && sp->IsExternal)
		return (0);
	sp1 = nullptr;
	if (sp1 == nullptr)
		sp1 = FindSymbol(sp, table);
	dfs.printf("k");
	if (sp1 != nullptr) {
		if (!sp1->tp->IsFunc() && sp->tp->IsFunc()) {
			sp1->tp = sp->tp;
			sp1->fi = sp->fi;
		}
		if (sp->fi) {
			if (sp1->fi->ParameterTypesMatch(sp->fi)) {
				if (!sp->fi->IsPrototype && sp1->fi->IsPrototype)
					MakeFunction(sp, sp1);
			}
		}
	}
	flag = false;
	if (sp1) {
		if (sp1->tp) {
			dfs.printf("l");
			flag = sp1->tp->type == bt_func;
			if (flag) {
				// Turn sp1 prototype into actual function instance.
				if (sp1->fi->IsPrototype)
					MakeFunction(sp, sp1);
			}
		}
	}
	if (sp->tp->type == bt_ifunc && flag) {
		MakeFunction(sp, sp1);
		sp = sp1;
	}
	else {
		// Here the symbol wasn't found in the table.
		if (sp1 == nullptr || !sp1->tp->IsSameType(sp1->tp,sp->tp,true))
			DoInsert(sp, table);
	}
	dfs.printf("J");
	if (needParseFunction==2) {
		needParseFunction = 0;
		if (sp->fi == nullptr) {
			TYP* t;
			sp->fi = MakeFunction(sp->number, sp, isPascal, isInline);
			sp->fi->IsFar = isFar;
			sp->fi->IsCoroutine = isCoroutine;
			sp->lsyms.ownerp = parent;
			sp->fi->depth = sp->depth;
			t = sp->tp;
			sp->tp = TYP::Copy(&stdfunc);
			sp->tp->btpp = t;
		}
		ofn = currentFn;
		if (sp->fi)
			currentFn = sp->fi;
		if (lastst != closepa)
			fn_doneinit = sp->fi->Parse(local);
		if (lastst == closepa) {
			NextToken();
			if (lastst == begin)
				needParseFunction = 2;
			return(0);
			NextToken();
			if (lastst == openpa) {
				int np, na, ellipos;
				Symbol* sp = (Symbol*)Symbol::alloc();
				NextToken();
				Function* fn = compiler.ff.MakeFunction(sp->number, sp, false);
				fn->depth = sp->depth;
				fn->BuildParameterList(&np, &na, &ellipos);
				if (ellipos >= 0)
					fn->NumFixedAutoParms = ellipos + 1;
				else
					fn->NumFixedAutoParms = np;
				if (lastst == closepa) {
					NextToken();
					while (lastst == kw_attribute)
						Declaration::ParseFunctionAttribute(fn,true);
				}
				needpunc(closepa, 52);
				if (lastst == begin) {
					needParseFunction = 2;
				}
			}
		}
		/*
		fn = sp->fi->FindExactMatch(TABLE::matchno);
		if (fn) {
			if (!sp->fi->alloced)
				delete sp->fi;
			sp->fi = fn;
			insState = 0;
		}
		else {
			fn = MakeFunctiontion(sp->id);
			memcpy(fn, sp->fi, sizeof(Function));
			if (!sp->fi->alloced)
				delete sp->fi;
			sp->fi = fn;
			switch (insState) {
			case 1:	sp->fi->InsertMethod(); break;
			case 2: table->insert(sp); break;
			}
			insState = 3;
		}
		*/
		if (sp->tp->type != bt_pointer) {
			if (ofn)
				currentFn = ofn;
			return (1);
		}
	}
	/*
	if (insState == 1 || insState == 2) {
		if (sp->tp->type == bt_ifunc || sp->tp->type == bt_func) {
			fn = sp->fi->FindExactMatch(TABLE::matchno);
			if (fn) {
				if (!sp->fi->alloced)
					delete sp->fi;
				sp->fi = fn;
			}
			else {
				fn = allocFunction(sp->id);
				memcpy(fn, sp->fi, sizeof(Function));
				if (!sp->fi->alloced)
					delete sp->fi;
				sp->fi = fn;
				switch (insState) {
				case 1:	sp->fi->InsertMethod(); break;
				case 2: table->insert(sp); break;
				}
				insState = 3;
			}
		}
		else if (insState==2)
			table->insert(sp);
	}
		*/
		//         if(sp->tp->type == bt_ifunc) { /* function body follows */
		//             ParseFunction(sp);
		//             return nbytes;
		//         }
	dfs.printf("K");
	if ((al == sc_global || al == sc_static || al == sc_thread) && !fn_doneinit &&
		sp->tp->type != bt_func && sp->tp->type != bt_ifunc && sp->storage_class != sc_typedef) {
		//(atan)()
		if (lastst != closepa && !sp->IsExternal)
			doinit(sp, false);
	}
	return (0);
}

void Declaration::FigureStructOffsets(int64_t bgn, Symbol* sp)
{
	TABLE* pt;
	Symbol* hd;
	int64_t nn;
	int64_t ps;
	int64_t bt;
	static int level = 0;
	int count = 0;

	ps = bgn;
	level++;
	for (hd = sp->tp->lst.headp; hd; hd = hd->nextp) {
		hd->value.i = ps;
		hd->tp->struct_offset = ps;
		if (hd->tp->IsStructType()) {
			if (level > 15)
				break;
			FigureStructOffsets(ps, hd);
		}
		if (hd->tp->bit_offset)
			if (hd->tp->bit_offset->i > 0)
				continue;
		if (sp->tp->type != bt_union)
			ps = ps + hd->tp->size;
		count++;
		if (count > 1000)
			break;
	}
	level--;
}

/*
 *      process declarations of the form:
 *
 *              <type>  <specifier>, <specifier>...;
 *
 *      leaves the declarations in the symbol table pointed to by
 *      table and returns the number of bytes declared. al is the
 *      allocation type to assign, ilc is the initial location
 *      counter. if al is sc_member then no initialization will
 *      be processed. ztype should be bt_struct for normal and in
 *      structure ParseSpecifierarations and sc_union for in union ParseSpecifierarations.
 */
int64_t Declaration::declare(Symbol* parent, TABLE* table, e_sc sc, int64_t ilc, int ztype, Symbol** symo, bool local, short depth)
{
	Expression exp;	// for gsearch;
	Symbol* sp;
	Symbol* sp1;
	Function* fn, *cf;
	TYP* dhead, * tp1, * tp2;
	ENODE* ep1, * ep2;
	int op;
	int fn_doneinit = 0;
	bool flag;
	int parentBytes = 0;
	std::string name;
	std::string tmpstr;
	int nbytes;
	int itdef;
	int insState = 0;
	Symbol* fp;
	e_sc al = sc;
	static int old_nbytes = 0;

	cf = currentFn;
	decl_level++;
	if (decl_level == 1)
		isTypedef = false;
	dfs.printf("<declare>\n");
	nbytes = 0;
	insState = 0;
	dfs.printf("A");
	classname = new std::string("");
	sp = nullptr;
	sp1 = nullptr;
	if (ParseSpecifier(table, &sp, al)) {
		goto xit1;
	}
	gCurrentTbl = table;
	ofs.write(" ");
	dfs.printf("B");
	dhead = head;
	for (;;) {
//		if (declid) delete declid;
		declid = nullptr;
		dfs.printf("b");
		bit_width = -1;
		tp1 = ParsePrefix(ztype == bt_union, nullptr, local, &sp);
		if (sp) {
			sp->depth = depth;
			if (sp->tp == nullptr)
				sp->SetType(tp1);
		}
		if (symo)
			*symo = sp;
		if (dhead == nullptr)
			dhead = &stdint;
		if (dhead->type == bt_bit) {
			if (head->isArray) {
				head->size += 7;
				head->size /= 8;
			}
		}
		if (sp)
			declid = new std::string(*sp->name);
		if (declid == nullptr)
			declid = new std::string("");
		if (al == sc_static) {
			tmpstr = GetNamespace();
			tmpstr += *declid;
			declid = new std::string(tmpstr);
		}

		if (funcdecl > 0)
			if (local)
				if (sp)
					sp->fi->Islocal = local;

		// If a function declaration is taking place and just the type is
		// specified without a parameter name, assign an internal compiler
		// generated name.
		if (dcltp==dt_parameter && declid->length() == 0)
			AssignParameterName();

		dfs.printf("C");
		if (declid->length() > 0 || classname->length() != 0 || (sp && sp->name && sp->name->length() != 0)) {      // otherwise just struct tag...
			// No symbol? Make one.
			if (sp == nullptr)
				sp = SymbolFactory::Make(*declid, nullptr, parent, depth, sc_none);

			// Not sure about this case: exit((void *)(void)); returned no name
			// Setting to declid "fixes" this.
			else if (sp->name->length() == 0)
				sp->SetName(*declid);
			
			// sets tp of sym to head
			SetType(sp);

//			fp = FindSymbol(sp, table);
			fp = exp.gsearch2(*sp->name, bt_int, nullptr, false);
			if (fp)	// Duplicate symbol???

			// If storage has already been allocated, go back and blank it out.
			if (fp && fp->storage_pos != 0 && funcdecl == 0) {
				int cnt = 0;
				/*
				std::streampos cpos = ofs.tellp();
				std::streampos pos = fp->storage_pos;
				ofs.seekp(fp->storage_pos);
				while (pos < fp->storage_endpos&& fp->storage_endpos > fp->storage_pos&& cnt < 65536) {
					cnt++;
					ofs.write(" ");
				}
				ofs.seekp(cpos);
				*/
			}
			sp->storage_pos = ofs.tellp();
			if (funcdecl <= 0)
				sp->IsInline = isInline;
			sp->IsRegister = isRegister;
			isRegister = false;
			sp->IsAuto = isAuto;
			sp->IsParameter = parsingParameterList > 0;
			if (sp->parentp == nullptr)
				sp->parentp = parent;
			if (al == sc_member)
				sp->IsPrivate = isPrivate;
			else
				sp->IsPrivate = false;
			sp->SetName(classname->length() > 0 ? *classname : sp->name->length() > 0 ? *sp->name : *declid);
			dfs.printf("D");
			if (classname) delete classname;
			classname = new std::string("");
			if (sp->tp->type == bt_func || sp->tp->type == bt_ifunc) {
				if (sp->fi)
					sp->fi->IsVirtual = isVirtual;
			}
			sp->storage_class = al;
			if (al == sc_external)
				sp->IsExternal = true;
			else if (al == sc_global)
				sp->IsExternal = false;

			sp->isConst = isConst;
			if (al != sc_member && !parsingParameterList) {
				//							sp->isTypedef = isTypedef;
				if (isTypedef) {
					sp->storage_class = sc_typedef;
				}
				//isTypedef = FALSE;
			}
			if (bit_width > 0 && bit_offset > 0)
				nbytes = old_nbytes - ilc;
			old_nbytes = ilc + nbytes;
			if (!sp->IsTypedef() && !sp->tp->IsFunc() && nbytes > 0 && !sp->IsExternal)
				nbytes += GenerateStorage(ofs, nbytes, al, ilc);
			dfs.printf("G");
			// Why the follwing???
			//if ((sp->tp->type == bt_func) && sp->storage_class == sc_global)
			//	sp->storage_class = sc_external;

			// Set the (struct member) storage offset.
			sp->SetStorageOffset(head, nbytes, al, ilc, ztype);

			// Increase the storage allocation by the type size.
			nbytes += sp->AdjustNbytes(nbytes, al, ztype);

			dfs.printf("H");
			// For a class declaration there may not be any variables declared as
			// part of the declaration. In that case the symbol name is an empty
			// string. There's nothing to insert in the symbol table.
			name = *sp->name;
			//if (strcmp(name.c_str(), "__Skip") == 0)
			//	printf("hl");
//			if (sp->name->length() > 0) {
			if (sp->storage_class == sc_member || sp->IsExternal) {
				Symbol* s;
				if ((s=table->insert(sp)) != sp) {
				}
			}
			else {
				if (sp && sp->fi) {
					sp->fi->int_save_mask = int_save_mask;
					sp->fi->operating_mode = operating_mode;
				}
				if (ParseFunction(table, sp, parent, al, local)) {
					if (local) {
						Symbol* s;
						if ((s = table->insert(sp)) != sp) {
						}
						//table->insert(sp);// ownerp->lsyms.insert(sp);
					}
					sp->storage_endpos = ofs.tellp();
					decl_level--;
					if (cf)
						currentFn = cf;
					return (nbytes);
				}
			}
			if (dcltp == dt_parameter) {
				func->params.insert(sp);
			}
			//			}
		}
		if (sp)
			if (sp->parentp == nullptr) {
				sp->parentp = parent;
			}
		if (funcdecl > 0) {
			if (lastst == closepa) {
				goto xit1;
			}
			if (lastst == comma) {
				goto xit1;
				break;
				NextToken();
				if (IsDeclBegin(lastst) == false)
					goto xit1;
				head = dhead;
				continue;
				//				break;
			}
			if (lastst == semicolon) {
				goto xit1;
			}
			if (lastst == assign) {
				ParseAssign(sp);
				if (lastst == semicolon)
					goto xit1;
			}
		}
		else if (catchdecl == TRUE) {
			if (lastst == closepa)
				goto xit1;
		}

		// If semi-colon is encountered we are at the end of the declaration.
		else if (lastst == semicolon) {
			DoDeclarationEnd(sp, sp1);
			break;	// semicolon
		}

		// Handle an assignment
		else if (lastst == assign) {
			ParseAssign(sp);
			if (lastst == semicolon) {
				goto xit1;
				break;
			}
		}
		// See if there is a list of variable declarations
		if (lastst == comma) {
			NextToken();
			head = dhead;
			continue;
		}
		// Maybe want to eval an expression
		// s.x = 3;
		else if (lastst == id) {
			goto xit1;
		}
		//needpunc(comma,24);
		if (IsDeclBegin(lastst) == false)
			goto xit1;//break;
		head = dhead;
		continue;
		goto xit1;
	}
	NextToken();
xit1:
	if (sp)
		sp->storage_endpos = ofs.tellp();
	if (decl_level == 1) {
		if (sp && sp->tp->IsStructType()) {
			TYP* tp;
			tp = sp->tp->Copy(sp->tp);
			sp->tp = tp;
			FigureStructOffsets(0, sp);
		}
	}
	dfs.printf("</declare>\n");
	decl_level--;
	if (symo) {
		*symo = sp;
	}
	if (cf)
		currentFn = cf;
	return (nbytes);
}

/* Dead code
int Declaration::declare(Symbol *parent,TABLE *table,e_sc al,int ilc,int ztype, Symbol** symo, bool local)
{ 
	TABLE* temp;
	int i;

	temp = itable;
	itable = table;
	istorage_class = al;
	i = declare(parent, table, ilc, ztype, symo, local);
	//itable = temp;
	return (i);
}
*/

void Declaration::ParseCoroutine()
{
	ENODE* node;

	node = nullptr;
	isCoroutine = true;
	sp_init = 0LL;
	NextToken();
	if (lastst == openpa) {
		NextToken();
		expression(&node, nullptr);
		sp_init = (int)node;// GetIntegerExpression(&node, nullptr, 0);
		needpunc(closepa, 49);
	}
}

void GlobalDeclaration::Parse()
{
	Symbol* symo;

	dfs.puts("<ParseGlobalDecl>\n");
	isPascal = defaultcc==1;
	isFar = false;
	isInline = false;
	isLeaf = false;
	isCoroutine = false;
	head = tail = nullptr;
	symo = nullptr;
	declid = nullptr;
	dcltp = dt_global;
	for(;;) {
		lc_auto = 0;
		bool notVal = false;
		isFuncPtr = false;
		currentClass = nullptr;
		currentFn = compiler.programFn;
		isFuncBody = false;
		worstAlignment = 0;
		funcdecl = 0;

		switch(lastst) {
		/* Under Construction
		case kw_namespace:
			NextToken();
			GetNamespace();
			if (lastst == id)
				strcpy_s(nmspace[0],sizeof(nmspace[0]), compiler.lastid);
			break;
		*/
		case kw_leaf:
			NextToken();
			isLeaf = true;
			break;
		case kw_pascal:
		  NextToken();
		  isPascal = TRUE;
		  break;
		case kw_cdecl:
			NextToken();
			isPascal = FALSE;
			break;
		case kw_inline:
			ParseInline();
		  break;
		case kw_far:
			NextToken();
			isFar = true;
			break;
		case kw_user:
			NextToken();
			operating_mode = 0;
			break;
		case kw_supervisor:
			NextToken();
			operating_mode = 1;
			break;
		case kw_hypervisor:
			NextToken();
			operating_mode = 2;
			break;
		case kw_machine:
			NextToken();
			operating_mode = 3;
			break;

		case kw_try:
			NextToken();
			continue;

		case id:
			compiler.lc_static += declare(NULL, &gsyms[0], sc_global, compiler.lc_static, bt_struct, &symo, false, 0);
			if (symo) {
				if (symo->fi) {
					symo->fi->inline_threshold = inline_threshold;
					symo->fi->IsInline = inline_threshold > 0 && !symo->fi->IsPrototype;
					symo->fi->depth = 0;
					symo->segment = codeseg;
				}
				else {
					if (symo->segment == noseg)
						symo->segment = bssseg;
				}
			}
			inline_threshold = compiler.autoInline;
			isCoroutine = false;
			break;

		case kw_declspec:
		case ellipsis:
		case kw_kernel:
		case kw_interrupt:
		case kw_coroutine:
    case kw_task:
		case kw_naked:
		case kw_nocall:
		case kw_oscall:
		case kw_typedef:
    case kw_virtual:
		case kw_volatile:
    case kw_exception:
		case kw_int8: case kw_int16: case kw_int32: case kw_int64: case kw_int40: case kw_int80:
		case kw_byte: case kw_char: case kw_int: case kw_short: case kw_unsigned: case kw_signed:
        case kw_long: case kw_struct: case kw_union: case kw_class:
				case kw_enum: case kw_void: case kw_bit: case kw_bool:
				case kw_float: case kw_double: case kw_float128: case kw_posit:
		case kw_vector: case kw_vector_mask:
			compiler.lc_static += declare(NULL, &gsyms[0], sc_global, compiler.lc_static, bt_struct, &symo, false, 0);
			if (symo) {
				if (symo->segment == noseg)
					symo->segment = bssseg;
				if (symo->fi) {
					symo->fi->inline_threshold = inline_threshold;
					symo->fi->IsInline = inline_threshold > 0 && !symo->fi->IsPrototype;
					symo->fi->depth = 0;
					symo->segment = codeseg;
				}
			}
			inline_threshold = compiler.autoInline;
			isCoroutine = false;
			break;

		case kw_const:
			compiler.lc_static += declare(NULL, &gsyms[0], sc_global, compiler.lc_static, bt_struct, &symo, false, 0);
			if (symo) {
				symo->segment = use_iprel ? codeseg : rodataseg;
				if (symo->fi) {
					symo->fi->inline_threshold = inline_threshold;
					symo->fi->IsInline = inline_threshold > 0 && !symo->fi->IsPrototype;
					symo->fi->depth = 0;
					symo->segment = codeseg;
				}
			}
			inline_threshold = compiler.autoInline;
			isCoroutine = false;
			break;

		case kw_thread:
				NextToken();
        lc_thread += declare(NULL,&gsyms[0],sc_thread,lc_thread,bt_struct, &symo, false, 0);
				if (symo)
					if (symo->fi) {
						symo->fi->inline_threshold = inline_threshold;
						symo->fi->IsInline = inline_threshold > 0 && !symo->fi->IsPrototype;
						symo->fi->depth = 0;
					}
				inline_threshold = compiler.autoInline;
				isCoroutine = false;
				break;
		case kw_register:
			NextToken();
      error(ERR_ILLCLASS);
      compiler.lc_static += declare(NULL,&gsyms[0],sc_global,compiler.lc_static,bt_struct, &symo, false, 0);
			if (symo)
				if (symo->fi) {
					symo->fi->inline_threshold = inline_threshold;
					symo->fi->IsInline = inline_threshold > 0 && !symo->fi->IsPrototype;
					symo->fi->depth = 0;
				}
			inline_threshold = compiler.autoInline;
			isCoroutine = false;
			break;
		case kw_private:
    case kw_static:
      NextToken();
			compiler.lc_static += declare(NULL,&gsyms[0],sc_static,compiler.lc_static,bt_struct, &symo, false, 0);
			if (symo) {
				if (symo->fi) {
					symo->fi->inline_threshold = inline_threshold;
					symo->fi->IsInline = inline_threshold > 0 && !symo->fi->IsPrototype;
					symo->fi->depth = 0;
				}
				// Not a function, must be a var.
				else {
					// If we don't know the segment assume bss.
					if (symo->segment == noseg)
						symo->segment = bssseg;
				}
			}
			inline_threshold = compiler.autoInline;
			isCoroutine = false;
			break;
    case kw_extern:
j1:
        NextToken();
				if (lastst==kw_pascal) {
					isPascal = TRUE;
					goto j1;
				}
				if (lastst == kw_coroutine) {
					isCoroutine = true;
					goto j1;
				}
				if (lastst == kw_far) {
					isFar = true;
					goto j1;
				}
				if (lastst == kw_leaf) {
					isLeaf = TRUE;
					goto j1;
				}
				if (lastst==kw_kernel) {
					isKernel = TRUE;
					goto j1;
				}
				if (lastst == kw_user) {
					operating_mode = 0;
					goto j1;
				}
				if (lastst == kw_supervisor) {
					operating_mode = 1;
					goto j1;
				}
				if (lastst == kw_hypervisor) {
					operating_mode = 2;
					goto j1;
				}
				if (lastst == kw_machine) {
					operating_mode = 3;
					goto j1;
				}
				if (lastst==kw_oscall || lastst==kw_interrupt || lastst==kw_coroutine || lastst==kw_nocall || lastst==kw_naked)
					NextToken();
          ++global_flag;
          declare(NULL,&gsyms[0],sc_external,0,bt_struct, &symo, false, 0);
					if (symo)
						if (symo->fi) {
							symo->fi->inline_threshold = inline_threshold;
							symo->fi->IsInline = inline_threshold > 0 && !symo->fi->IsPrototype;
							symo->fi->depth = 0;
						}
					inline_threshold = compiler.autoInline;
					isCoroutine = false;
					--global_flag;
          break;
 
    case kw_not:
      NextToken();
      notVal = !notVal;
      break;

    case kw_using:
      NextToken();
      if (lastst==id) {
        if (strcmp(compiler.lastid,"_name")==0) {
          NextToken();
          if (lastst==id) {
            if (strcmp(compiler.lastid, "_mangler")==0) {
              NextToken();
              mangledNames = !notVal;
            }
          }
        }
        else if (strcmp(compiler.lastid,"_real")==0) {
          NextToken();
          if (lastst==id) {
            if (strcmp(compiler.lastid,"_names")==0) {
              NextToken();
              mangledNames = notVal;
            }
          }
        }
				else if (strcmp(compiler.lastid, "___cdecl") == 0) {
					NextToken();
					defaultcc = 0;
				}
				else if (strcmp(compiler.lastid, "_pascal") == 0) {
					NextToken();
					defaultcc = 1;
				}
      }
	  else if (lastst==kw_short) {
		  NextToken();
		  if (lastst==id) {
			  if (strcmp(compiler.lastid,"_pointers")==0)
				  cpu.sizeOfPtr = 4;
		  }
	  }
	  else if (lastst==kw_long) {
		  NextToken();
		  if (lastst==id) {
			  if (strcmp(compiler.lastid,"_pointers")==0)
				  cpu.sizeOfPtr = 12;
		  }
	  }
      break;

    default:
      goto xit;
		}
	}
xit:
	dfs.puts("</ParseGlobalDecl>\n");
	;
}

// return (0) to cause a break
// return (1) to cause xit goto

int AutoDeclaration::ParseId(Symbol* parent, TABLE* ssyms, Statement* st)
{
	Symbol* sp, *symo;
	int nn;

	dfs.printf("Found %s\n", compiler.lastid);
	sp = tagtable.Find(compiler.lastid, false);
	if (sp)
		dfs.printf("Found in tagtable");
	if (sp == nullptr) {
		//sp = gsearch2(compiler.lastid, (__int16)bt_long, nullptr, false);
		sp = gsyms[0].Find(compiler.lastid, false);
		if (TABLE::matchno > 1) {
			for (nn = 0; nn < TABLE::matchno; nn++) {
				sp = TABLE::match[nn];
				if (sp->storage_class == sc_typedef || sp->storage_class == sc_type)
					return(0);
			}
			sp = nullptr;
		}
	}
	if (sp) {
		dfs.printf("sp okay sc=%d\n", sp->storage_class);
		if (sp->storage_class == sc_typedef || sp->storage_class == sc_type) {
			dfs.printf("Declaring var of type\n");
			depth++;
			lc_auto += declare(parent, ssyms, sc_auto, lc_auto, bt_struct, &symo, isLocal, depth-1);
			depth--;
			if (symo) {
				symo->segment = dataseg;
				if (symo->fi) {
					symo->fi->inline_threshold = inline_threshold;
					symo->fi->IsInline = inline_threshold > 0 && !symo->fi->IsPrototype;
					symo->segment = codeseg;
				}
			}
			inline_threshold = compiler.autoInline;
			return(0);
		}
	}
	return (1);
}

void AutoDeclaration::ParseThread(Symbol* parent, TABLE* ssyms, Statement* st)
{
	Symbol* symo;

	NextToken();
	depth++;
	lc_thread += declare(parent, ssyms, sc_thread, lc_thread, bt_struct, &symo, isLocal, depth-1);
	depth--;
	if (symo)
		if (symo->fi) {
			symo->fi->inline_threshold = inline_threshold;
			symo->fi->IsInline = inline_threshold > 0 && !symo->fi->IsPrototype;
		}
	inline_threshold = compiler.autoInline;
}

void AutoDeclaration::ParseStatic(Symbol* parent, TABLE* ssyms, Statement* st)
{
	Symbol* symo;

	NextToken();
	depth++;
	compiler.lc_static += declare(parent, ssyms, sc_static, compiler.lc_static, bt_struct, &symo, isLocal, depth-1);
	depth--;
	if (symo) {
		if (symo->fi) {
			symo->fi->inline_threshold = inline_threshold;
			symo->fi->IsInline = inline_threshold > 0 && !symo->fi->IsPrototype;
			symo->segment = codeseg;
		}
		else
			symo->segment = symo->isConst ? (use_iprel ? codeseg : rodataseg) : dataseg;
	}
	inline_threshold = compiler.autoInline;
}

void AutoDeclaration::ParseExtern(Symbol* parent, TABLE* ssyms, Statement* st)
{
	Symbol* symo;

	NextToken();
	if (lastst == kw_oscall ||
		lastst == kw_interrupt ||
		lastst == kw_nocall ||
		lastst == kw_naked || 
		lastst == kw_kernel ||
		lastst == kw_user ||
		lastst == kw_supervisor ||
		lastst == kw_hypervisor ||
		lastst == kw_machine)
		NextToken();
	++global_flag;
	depth++;
	declare(nullptr, &gsyms[0], sc_external, 0, bt_struct, &symo, isLocal, depth-1);
	depth--;
	if (symo)
		if (symo->fi) {
			symo->segment = codeseg;
			symo->fi->inline_threshold = inline_threshold;
			symo->fi->IsInline = inline_threshold > 0 && !symo->fi->IsPrototype;
		}
	inline_threshold = compiler.autoInline;
	--global_flag;
}

void AutoDeclaration::ParseIntrinsicType(Symbol* parent, TABLE* ssyms, Statement* st)
{
	Symbol* symo;

	depth++;
	lc_auto += declare(parent, ssyms, sc_auto, lc_auto, bt_struct, &symo, isLocal, depth-1);
	depth--;
	if (symo)
		if (symo->fi) {
			symo->fi->inline_threshold = inline_threshold;
			symo->fi->IsInline = inline_threshold > 0 && !symo->fi->IsPrototype;
		}
	inline_threshold = compiler.autoInline;
}

ENODE *AutoDeclaration::Parse(Symbol *parent, TABLE *ssyms, Statement* st)
{
	Symbol *sp, *symo;
	ENODE* ep1;
	static short depth = 0;

//	printf("Enter ParseAutoDecls\r\n");
	isLocal = true;
	dcltp = dt_auto;
	stmt = st;
  for(;;) {
		funcdecl = 0;
		isFuncPtr = false;
		worstAlignment = 0;
		switch(lastst) {
		case kw_inline:
			ParseInline();
			break;
		case kw_cdecl:
    case kw_kernel:
		case kw_user: case kw_supervisor: case kw_hypervisor: case kw_machine:
		case kw_interrupt:
		case kw_coroutine:
		case kw_naked:
		case kw_nocall:
		case kw_oscall:
		case kw_pascal:
		case kw_typedef:
//      error(ERR_ILLCLASS);
			depth++;
	    lc_auto += declare(parent,ssyms,sc_auto,lc_auto,bt_struct,&symo,isLocal,depth-1);
			depth--;
			if (symo)
				if (symo->fi) {
					symo->fi->inline_threshold = inline_threshold;
					symo->fi->IsInline = inline_threshold > 0 && !symo->fi->IsPrototype;
				}
			inline_threshold = compiler.autoInline;
			break;

		case ellipsis:
		case id: //return;
			if (ParseId(parent, ssyms, st))
				goto xit;
			break;

    case kw_register:
      NextToken();
			break;

    case kw_exception:
		case kw_volatile: case kw_const:
		case kw_bit: case kw_bool:
		case kw_int8: case kw_int16: case kw_int32: case kw_int64: case kw_int40: case kw_int80:
		case kw_byte: case kw_char: case kw_int: case kw_short: case kw_unsigned: case kw_signed:
    case kw_long: case kw_struct: case kw_union: case kw_class:
    case kw_enum: case kw_void:
		case kw_half: case kw_float: case kw_double: case kw_quad: case kw_float128: case kw_posit:
		case kw_vector: case kw_vector_mask:
			ParseIntrinsicType(parent, ssyms, st);
			break;

		case kw_thread:
			ParseThread(parent, ssyms, st);
			break;

		case kw_static:
			ParseStatic(parent, ssyms, st);
			break;

		case kw_extern:
			ParseStatic(parent, ssyms, st);
			break;

		// Not a recognized token? Break loop.
    default:
      goto xit;
		}
	}
xit:
	;
	ep1 = nullptr;
	for (sp = ssyms->headp; sp; sp = sp->nextp) {
		if (sp->initexp) 
			ep1 = makenode(en_void, ep1, sp->initexp);
	}
	return (ep1);
//	printf("Leave ParseAutoDecls\r\n");
}

TABLE* ParameterDeclaration::Parse(int fd, bool throw_away, Function* fn)
{
	int ofd;
  int opascal;
	int oisInline;
	int ofar;
	int oco;
	bool fellip = false;
	TABLE* tbl;

	tbl = new TABLE;
	dfs.puts("<ParseParmDecls>\n");
	dcltp = dt_parameter;
	func = fn;
	ofd = funcdecl;
	opascal = isPascal;
	ofar = isFar;
	oisInline = isInline;
	oco = isCoroutine;
	isPascal = defaultcc==1;
	isFar = false;
	funcdecl = fd;
	parsingParameterList++;
	nparms = 0;
	number = -1;
	ellip = -1;
	for(;;) {
		worstAlignment = 0;
		isFuncPtr = false;
		isAuto = false;
		isRegister = false;
		isInline = false;
		missingArgumentName = FALSE;
		fellip = false;
		dfs.printf("A(%d)",lastst);
j1:
		switch(lastst) {
		case kw_auto:
			NextToken();
			isAuto = true;
			goto j1;
		case kw_pascal:
			NextToken();
		  isPascal = TRUE;
		  goto j1;
		case kw_far:
			NextToken();
			isFar = true;
			goto j1;
		case kw_cdecl:
    case kw_kernel:
		case kw_user: case kw_supervisor: case kw_hypervisor: case kw_machine:
		case kw_interrupt:
		case kw_coroutine:
		case kw_naked:
		case kw_nocall:
		case kw_oscall:
		case kw_typedef:
dfs.printf("B");
      error(ERR_ILLCLASS);
      declare(NULL,throw_away ? &scrap_table : tbl,sc_auto,0,bt_struct,nullptr,false,currentFn->depth);
			number++;
			isAuto = false;
			break;
		// Must be first case as it falls through
		case ellipsis:
			fellip = true;
		case kw_const:
		case kw_volatile:
        case kw_exception:
		case kw_int8: case kw_int16: case kw_int32: case kw_int64: case kw_int40: case kw_int80:
		case kw_byte: case kw_char: case kw_int: case kw_short: case kw_unsigned: case kw_signed:
    case kw_long: case kw_struct: case kw_union: case kw_class:
		case kw_enum: case kw_void: case kw_bit: case kw_bool:
		case kw_float: case kw_double: case kw_float128: case kw_posit:
		case kw_vector: case kw_vector_mask:
dfs.printf("C");
			declare(NULL, throw_away ? &scrap_table : tbl,sc_auto,0,bt_struct,nullptr,false, currentFn->depth);
			number++;
			if (fellip)
				ellip = number;
			isAuto = false;
	    break;
		case kw_inline:
			ParseInline();
			declare(NULL, throw_away ? &scrap_table : tbl, sc_auto, 0, bt_struct, nullptr, false, currentFn->depth);
			number++;
			if (fellip)
				ellip = number;
			isAuto = false;
			break;
		case id:
			declare(NULL, throw_away ? &scrap_table : tbl, sc_auto, 0, bt_struct,nullptr, false, currentFn->depth);
			number++;
			isAuto = false;
			break;
		case kw_thread:
      NextToken();
      error(ERR_ILLCLASS);
			lc_thread += declare(NULL,throw_away ? &scrap_table : &gsyms[0],sc_thread,lc_thread,bt_struct,nullptr, false, currentFn->depth);
			isAuto = false;
			break;
    case kw_static:
      NextToken();
      error(ERR_ILLCLASS);
			compiler.lc_static += declare(NULL, throw_away ? &scrap_table : &gsyms[0],sc_static,compiler.lc_static,bt_struct,nullptr, false, currentFn->depth);
			isAuto = false;
			break;
		// A list of externals could be following a function prototype. This
		// could be confused with a parameter list.
    case kw_extern:
//					push_token();
//					goto xit;
dfs.printf("D");
                NextToken();
                error(ERR_ILLCLASS);
				if (lastst==kw_oscall || lastst==kw_interrupt || lastst==kw_coroutine || lastst == kw_nocall || lastst==kw_naked || lastst==kw_kernel ||
					lastst == kw_user || lastst == kw_supervisor || lastst == kw_hypervisor || lastst == kw_machine)
					NextToken();
                ++global_flag;
                declare(NULL, throw_away ? &scrap_table : &gsyms[0],sc_external,0,bt_struct,nullptr,false, currentFn->depth);
				isAuto = false;
                --global_flag;
                break;
		case kw_register:
			isRegister = true;
			NextToken();
			goto j1;
    default:
			goto xit;
		}
		dfs.printf("E");
		if (lastst == comma)
			NextToken();
	}
xit:
	parsingParameterList--;
	funcdecl = ofd;
	isPascal = opascal;
	isInline = oisInline;
	isFar = ofar;
	isCoroutine = oco;
	dfs.printf("</ParseParmDecls>\n");
	return (tbl);
}

GlobalDeclaration *GlobalDeclaration::Make()
{
  GlobalDeclaration *p = (GlobalDeclaration *)allocx(sizeof(GlobalDeclaration));
  return p;
}

void compile()
{
}

