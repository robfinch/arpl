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
using namespace std;
#include <map>

/*
 *      this module contains all of the code generation routines
 *      for evaluating expressions and conditions.
 */

int hook_predreg=15;

Operand *GenerateExpression();            /* forward ParseSpecifieraration */

extern Operand *GenExpr(ENODE *node);

void GenLoad(Operand *ap3, Operand *ap1, int ssize, int size);

extern int throwlab;
static int nest_level = 0;

static void Enter(char *p)
{
/*
     int nn;
     
     for (nn = 0; nn < nest_level; nn++)
         printf(" ");
     printf("%s: %d ", p, lineno);
     nest_level++;
*/
}
static void Enter(const char* p) {
	Enter((char*)p);
}

static void Leave(char *p, int n)
{
/*
     int nn;
     
     nest_level--;
     for (nn = 0; nn < nest_level; nn++)
         printf(" ");
     printf("%s (%d) ", p, n);
*/
}


Operand *CodeGenerator::MakeDataLabel(int64_t lab, int ndxreg)
{
	return (compiler.of.MakeDataLabel(lab, ndxreg));
}

Operand *CodeGenerator::MakeCodeLabel(int64_t lab)
{
	return (compiler.of.MakeCodeLabel(lab));
}

Operand *CodeGenerator::MakeStringAsNameConst(char *s, e_sg seg)
{
	return (compiler.of.MakeStringAsNameConst(s, seg));
}

Operand *CodeGenerator::MakeString(char *s)
{
	return (compiler.of.MakeString(s));
}

Operand *CodeGenerator::MakeImmediate(int64_t i, int display)
{
	return (compiler.of.MakeImmediate(i, display));
}

Operand* CodeGenerator::MakeImmediate(Int128 i, int display)
{
	return (compiler.of.MakeImmediate(i, display));
}

Operand *CodeGenerator::MakeIndirect(int i)
{
	return (compiler.of.MakeIndirect(i));
}

Operand *CodeGenerator::MakeIndexed(int64_t o, int i)
{
	return (compiler.of.MakeIndexed(o, i));
}

Operand *CodeGenerator::MakeDoubleIndexed(int i, int j, int scale)
{
	return (compiler.of.MakeDoubleIndexed(i, j, scale));
}

Operand *CodeGenerator::MakeDirect(ENODE *node)
{
	return (compiler.of.MakeDirect(node));
}

Operand *CodeGenerator::MakeIndexed(ENODE *node, int rg)
{
	return (compiler.of.MakeIndexed(node, rg));
}

Operand* CodeGenerator::MakeIndexedName(std::string nme, int rg)
{
	return (compiler.of.MakeIndexedName(nme, rg));
}

void CodeGenerator::GenerateHint(int num)
{
	GenerateMonadic(op_hint,0,MakeImmediate(num));
}

void CodeGenerator::GenerateComment(char *cm)
{
	GenerateMonadic(op_rem2,0,MakeStringAsNameConst(cm,codeseg));
}

Operand* CodeGenerator::GetTempRegister() 
{
	return (::GetTempRegister());
}

Operand* CodeGenerator::GetTempFPRegister()
{
	return (::GetTempRegister());
}

//
// Generate code to evaluate a condition operator node (?:)
//
Operand* CodeGenerator::GenerateHook(ENODE* inode, int flags, int64_t size)
{
	Operand* ap1, * ap2, * ap3;
	int false_label, end_label;
	ENODE* node;
	bool voidResult;

	false_label = nextlabel++;
	end_label = nextlabel++;
	//flags = (flags & am_reg) | am_volatile;
	flags |= am_volatile;
	//ip1 = currentFn->pl.tail;
	//ap2 = cg.GenerateExpression(p[1]->p[1], flags, size);
	//n1 = currentFn->pl.Count(ip1);
	//if (opt_nocgo)
	//	n1 = 9999;
	//ReleaseTempReg(ap2);
	//currentFn->pl.tail = ip1;
	//currentFn->pl.tail->fwd = nullptr;
	voidResult = inode->p[0]->etype == bt_void;
	cg.GenerateFalseJump(inode->p[0], false_label, 0);
	node = inode->p[1];
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], flags, size, 0);
	ap3->tp = ap1->tp;
	if (!voidResult)
		switch (ap1->mode) {
		case am_reg:
			GenerateMove(ap3, ap1);
			break;
		case am_imm:
			GenerateLoadConst(ap3, ap1);
			break;
		default:
			GenerateLoad(ap3, ap1, cpu.sizeOfWord, cpu.sizeOfWord);
			break;
		}
	ReleaseTempRegister(ap1);
	GenerateBra(end_label);
	GenerateLabel(false_label);
	ap2 = cg.GenerateExpression(node->p[1], flags, size, 1);
	ap3->tp = ap2->tp;
	if (!Operand::IsSameType(ap1, ap2) && !voidResult)
		error(ERR_MISMATCH);
	if (!voidResult)
		switch (ap2->mode) {
		case am_reg:
			GenerateMove(ap3, ap2);
			break;
		case am_imm:
			GenerateLoadConst(ap3, ap2);	// I think this is backwards, test
			break;
		default:
			GenerateLoad(ap3, ap2, cpu.sizeOfWord, cpu.sizeOfWord);
			break;
		}
	ReleaseTempRegister(ap2);
	GenerateLabel(end_label);
	ap3->MakeLegal(flags, size);
	return (ap3);
}

Operand* CodeGenerator::GenerateMux(ENODE* inode, int flags, int64_t size)
{
	List* lst, *hlst;
	ENODE* pnode;
	Operand* ap1, *ap2, *ap3;
	int nn, kk;
	int64_t lab, labxit;
	bool void_result = false;

	labxit = nextlabel++;
	if (inode->nodetype != en_safe_cond)
		throw new ArplException(ERR_MISSING_MUX, 0);

	// Put the node list in the order we need.
	nn = 0;
	for (pnode = inode->p[1]; pnode; pnode = pnode->p[0])
		nn++;
	hlst = lst = new List[nn];
	kk = nn;
	for (pnode = inode->p[1]; pnode; pnode = pnode->p[0]) {
		--kk;
		lst[kk].node = pnode->p[1];
	}

	ap3 = GetTempRegister();
	ap2 = cg.GenerateExpression(inode->p[0], flags, size, 0);
	void_result = inode->etype == bt_void;
	for (kk = 0; kk < nn; kk++) {
		lab = nextlabel++;
		if (kk < nn - 1)
			GenerateBne(ap2, MakeImmediate(kk), lab);
		ap1 = cg.GenerateExpression(lst[kk].node, flags, size, 0);
		ap3->tp = ap1->tp;
		if (!void_result)
			switch (ap1->mode) {
			case am_reg:
				GenerateMove(ap3, ap1);
				break;
			case am_imm:
				GenerateLoadConst(ap1, ap3);
				break;
			default:
				GenerateLoad(ap3, ap1, cpu.sizeOfWord, cpu.sizeOfWord);
				break;
			}
		ReleaseTempRegister(ap1);
		GenerateBra(labxit);
		GenerateLabel(lab);
	}
	GenerateLabel(labxit);
	ReleaseTempRegister(ap2);
	return (ap3);
}

void CodeGenerator::GenerateLoadAddress(Operand* ap3, Operand* ap1)
{
	GenerateDiadic(op_lda, 0, ap3, ap1);
}

void CodeGenerator::GenerateLoad(Operand *ap3, Operand *ap1, int64_t ssize, int64_t size, Operand* mask)
{
	std::string* str;

	if (ap3->typep==&stdposit) {
		switch (ap3->tp->precision) {
		case 16:
			GenerateTriadic(op_pldw, 0, ap3, ap1, mask);
			break;
		case 32:
			GenerateTriadic(op_pldt, 0, ap3, ap1, mask);
			break;
		default:
			GenerateTriadic(op_pldo, 0, ap3, ap1, mask);
			break;
		}
	}
	else if (ap3->typep==&stdvector) {
		GenerateTriadic(op_loadv,0,ap3,ap1, mask);
	}
	else if (ap3->typep->IsFloatType())
		GenerateLoadFloat(ap3, ap1, ssize, size, mask);
	//else if (ap3->mode == am_fpreg) {
	//	GenerateTriadic(op_fldo, 0, ap3, ap1);
	//}
	else if (ap3->isUnsigned) {
		// If size is zero, probably a pointer to void being processed.
			switch (size) {
			case 0: GenerateTriadic(op_ldbu, 0, ap3, ap1, mask); break;
			case 1:	str = new std::string(""); if (ap3->tp && ap3->tp->rd_cache) str->append(*ap3->tp->rd_cache); GenerateTriadicEx(op_ldbu, str, ap3, ap1, mask); break;
			case 2:	str = new std::string(""); if (ap3->tp && ap3->tp->rd_cache) str->append(*ap3->tp->rd_cache); GenerateTriadicEx(op_ldwu, str, ap3, ap1, mask); break;
			case 4:	str = new std::string(""); if (ap3->tp && ap3->tp->rd_cache) str->append(*ap3->tp->rd_cache); GenerateTriadicEx(op_ldtu, str, ap3, ap1, mask); break;
			case 8: str = new std::string(""); if (ap3->tp && ap3->tp->rd_cache) str->append(*ap3->tp->rd_cache); GenerateTriadicEx(op_ldou, str, ap3, ap1, mask); break;
			case 16:	str = new std::string(""); if (ap3->tp && ap3->tp->rd_cache) str->append(*ap3->tp->rd_cache); GenerateTriadicEx(op_ldh, str, ap3, ap1, mask); break;
			}
    }
    else {
			if (ap1->mode == am_direct && !ap1->offset->i128.IsNBit(20)) {
				Operand* ap4;
				ap4 = GetTempRegister();
				GenerateLoadConst(MakeImmediate(ap1->offset->i128), ap4);
				ap1->mode = am_ind;
				ap1->preg = ap4->preg;
				switch (size) {
				case 0: GenerateTriadic(op_ldb, 0, ap3, ap4, mask); break;
				case 1:	str = new std::string(""); if (ap3->tp && ap3->tp->rd_cache) str->append(*ap3->tp->rd_cache); GenerateTriadicEx(op_ldb, str, ap3, ap1, mask); break;
				case 2:	str = new std::string(""); if (ap3->tp && ap3->tp->rd_cache) str->append(*ap3->tp->rd_cache); GenerateTriadicEx(op_ldw, str, ap3, ap1, mask); break;
				case 4:	str = new std::string(""); if (ap3->tp && ap3->tp->rd_cache) str->append(*ap3->tp->rd_cache); GenerateTriadicEx(op_ldt, str, ap3, ap1, mask); break;
				case 8:	str = new std::string(""); if (ap3->tp && ap3->tp->rd_cache) str->append(*ap3->tp->rd_cache); GenerateTriadicEx(op_ldo, str, ap3, ap1, mask); break;
				case 16: str = new std::string(""); if (ap3->tp && ap3->tp->rd_cache) str->append(*ap3->tp->rd_cache); GenerateTriadicEx(op_ldh, str, ap3, ap1, mask); break;
				}
			}
			else {
				switch (size) {
				case 0: GenerateTriadic(op_ldb, 0, ap3, ap1, mask); break;
				case 1:	str = new std::string(""); if (ap3->tp && ap3->tp->rd_cache) str->append(*ap3->tp->rd_cache); GenerateTriadicEx(op_ldb, str, ap3, ap1, mask); break;
				case 2:	str = new std::string(""); if (ap3->tp && ap3->tp->rd_cache) str->append(*ap3->tp->rd_cache); GenerateTriadicEx(op_ldw, str, ap3, ap1, mask); break;
				case 4:	str = new std::string(""); if (ap3->tp && ap3->tp->rd_cache) str->append(*ap3->tp->rd_cache); GenerateTriadicEx(op_ldt, str, ap3, ap1, mask); break;
				case 8:	str = new std::string(""); if (ap3->tp && ap3->tp->rd_cache) str->append(*ap3->tp->rd_cache); GenerateTriadicEx(op_ldo, str, ap3, ap1, mask); break;
				case 16: str = new std::string(""); if (ap3->tp && ap3->tp->rd_cache) str->append(*ap3->tp->rd_cache); GenerateTriadicEx(op_ldh, str, ap3, ap1, mask); break;
				}
			}
    }
	ap3->memref = true;
	ap3->memop = ap1->Clone();
}

void CodeGenerator::GenerateStore(Operand *ap1, Operand *ap3, int64_t size, Operand* mask)
{
	std::string* str;

	//if (ap1->isPtr) {
	//	GenerateTriadic(op_std, 0, ap1, ap3);
	//}
	//else
	if (ap3->tp && ap3->tp->IsPositType()) {
		switch (ap3->tp->precision) {
		case 16:
			GenerateTriadic(op_pstw, 0, ap1, ap3, mask);
			break;
		case 32:
			GenerateTriadic(op_pstt, 0, ap1, ap3, mask);
			break;
		default:
			GenerateTriadic(op_psto, 0, ap1, ap3, mask);
			break;
		}
	}
	if (ap3->typep==&stdposit) {
		GenerateTriadic(op_sto, 0, ap1, ap3, mask);
	}
	else if (ap1->tp->IsVectorType())//typep==&stdvector)
	    GenerateTriadic(op_sv,0,ap1,ap3, mask);
	else if (ap1->typep == &stdflt) {
		GenerateTriadic(op_sto, 0, ap1, ap3, mask);
	}
	else if (ap1->typep == &stddouble) {
		if (ap1->mode == am_fpreg)
			printf("ho");
		GenerateTriadic(op_sto, 0, ap1, ap3, mask);
	}
	else if (ap1->typep == &stdquad) {
		GenerateTriadic(op_stf, 'q', ap1, ap3, mask);
	}
	else if (ap1->typep == &stdtriple) {
		GenerateTriadic(op_stf, 't', ap1, ap3, mask);
	}
	//else if (ap1->mode==am_fpreg)
	//	GenerateTriadic(op_fsto,0,ap1,ap3, mask);
	else {
		if (ap3->mode == am_direct && ap3->offset && !ap3->offset->i128.IsNBit(20)) {
			Operand* ap4;
			ap4 = GetTempRegister();
			GenerateLoadConst(MakeImmediate(ap3->offset->i128), ap4);
			ReleaseTempReg(ap3);
			ap3->mode = am_ind;
			ap3->preg = ap4->preg;
			switch (size) {
			case 1: str = new std::string(""); if (ap1->tp && ap1->tp->wr_cache) str->append(*ap1->tp->wr_cache); GenerateTriadicEx(op_stb, str, ap1, ap3, mask); break;
			case 2: str = new std::string(""); if (ap1->tp && ap1->tp->wr_cache) str->append(*ap1->tp->wr_cache); GenerateTriadicEx(op_stw, str, ap1, ap3, mask); break;
			case 4: str = new std::string(""); if (ap1->tp && ap1->tp->wr_cache) str->append(*ap1->tp->wr_cache); GenerateTriadicEx(op_stt, str, ap1, ap3, mask); break;
			case 8:	str = new std::string(""); if (ap1->tp && ap1->tp->wr_cache) str->append(*ap1->tp->wr_cache); GenerateTriadicEx(op_sto, str, ap1, ap3, mask); break;
			case 16:	str = new std::string(""); if (ap1->tp && ap1->tp->wr_cache) str->append(*ap1->tp->wr_cache); GenerateTriadicEx(op_sth, str, ap1, ap3, mask); break;
			default:
				;
			}
		}
		else {
			switch (size) {
			case 1: str = new std::string(""); if (ap1->tp && ap1->tp->wr_cache) str->append(*ap1->tp->wr_cache); GenerateTriadicEx(op_stb, str, ap1, ap3, mask); break;
			case 2: str = new std::string(""); if (ap1->tp && ap1->tp->wr_cache) str->append(*ap1->tp->wr_cache); GenerateTriadicEx(op_stw, str, ap1, ap3, mask); break;
			case 4: str = new std::string(""); if (ap1->tp && ap1->tp->wr_cache) str->append(*ap1->tp->wr_cache); GenerateTriadicEx(op_stt, str, ap1, ap3, mask); break;
			case 8:	str = new std::string(""); if (ap1->tp && ap1->tp->wr_cache) str->append(*ap1->tp->wr_cache); GenerateTriadicEx(op_sto, str, ap1, ap3, mask); break;
			case 16:	str = new std::string(""); if (ap1->tp && ap1->tp->wr_cache) str->append(*ap1->tp->wr_cache); GenerateTriadicEx(op_sth, str, ap1, ap3, mask); break;
			default:
				;
			}
		}
	}
}

Operand* CodeGenerator::GenerateBitfieldDereference(ENODE* node, int flags, int64_t size, int opt)
{
	return (node->GenerateBitfieldDereference(flags, size, opt));
}

ENODE* FindAnd(ENODE *node)
{
	if (node->nodetype == en_and) {
		if (node->p[1]->nodetype == en_icon) {
			if (node->p[1]->i == 63) {
				return (node->p[1]);
			}
		}
	}
	return (nullptr);
}

Operand* CodeGenerator::GenerateFieldrefDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size)
{
	Operand* ap1;

	// Not much point calling GenerateExpression() here as the node's p[0], p[1] are null.
	//node->p[0] = node->bit_offset;
	//node->p[1] = node->bit_width;
	/*
	ap3 = GetTempRegister();
	ap4 = GetTempRegister();
	if (node->sym)
		GenerateLoad(ap4, MakeDirect(node), size, size);
	else
		GenerateLoadConst(MakeImmediate(node->i128), ap4);
	ap1 = GenerateExpression(node->bit_offset, am_reg | am_mem, cpu.sizeOfWord, 0);
	ap2 = GenerateExpression(node->bit_width, am_reg | am_mem, cpu.sizeOfWord, 0);
	Generate4adic(tp->isUnsigned ? op_extu : op_ext, 0, ap3, ap4, ap1, ap2);
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	ReleaseTempRegister(ap4);
	return (ap3);
	*/
	
	ap1 = GenerateExpression(node, am_reg | am_mem, cpu.sizeOfWord, 0);
	cg.GenerateBitfieldExtract(ap1, node->bit_offset, node->bit_width);
	/*
	ap1->bit_offset = node->bit_offset;
	ap1->bit_width = node->bit_width;
	ap1->isPtr = isRefType;
	ap1->tp = tp;
	ap1->segment = dataseg;
	ap1->MakeLegal(flags, size);
	*/
	return (ap1);
	
}

Operand* CodeGenerator::GenerateAddDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size, int64_t siz1, int su)
{
	Operand* ap1;

	ap1 = node->GenerateIndex(false);
	ap1->isUnsigned = !su;//node->isUnsigned;
// *** may have to fix for stackseg
	ap1->segment = node->segment;// dataseg;
	ap1->tp = tp;
	ap1->bit_offset = node->bit_offset;
	ap1->bit_width = node->bit_width;
	//		ap2->mode = ap1->mode;
	//		ap2->segment = dataseg;
	//		ap2->offset = ap1->offset;
	//		ReleaseTempRegister(ap1);
	if (!node->isUnsigned) {
		if (size < siz1)
			ap1 = ap1->GenerateSignExtend(siz1, size, flags);
	}
	else
		ap1->MakeLegal(flags, siz1);
//	ap1->MakeLegal(flags, size);
	return (ap1);
}

Operand* CodeGenerator::GenerateAsaddDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size, int64_t siz1, int su, bool neg)
{
	Operand* ap1, * ap2;

	ap2 = GetTempRegister();
	ap1 = GenerateExpression(node, flags, size, 0);
	ap1->mode = am_ind;
	ap2->tp = ap1->tp;
	GenerateLoad(ap2, ap1, size, size);
	ReleaseTempRegister(ap1);
	ap2->MakeLegal(flags, size);
	return (ap2);
}

Operand* CodeGenerator::GenerateAutoconDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size, int64_t siz1, int su)
{
	Operand* ap1, *ap3;
	int nn, ni;

	ap1 = allocOperand();
	ap1->isPtr = isRefType;
	ap1->mode = am_indx;
	ap1->preg = regFP;
	ap1->segment = stackseg;
	ap1->offset = makeinode(en_icon, node->i);
	ap1->offset->sym = node->sym;
	ap1->bit_offset = node->bit_offset;
	ap1->bit_width = node->bit_width;
	ap1->argref = node->sym->IsParameter;
	ap1->isUnsigned = !su;
	ap1->tp = tp;

	ni = nn = (currentFn->depth) - (node->sym->IsParameter?node->sym->depth + 3 : node->sym->depth);
	if (nn > 0) {
		ap3 = GetTempRegister();
		ap3->tp = tp;
		if (nn==1)
			GenerateLoad(ap3, MakeIndirect(regFP), size, size);
		else
			GenerateLoad(ap3, MakeIndirect(regFP), cpu.sizeOfWord, cpu.sizeOfWord);
		for (--nn; nn > 0; nn--) {
			if (nn == 1)
				GenerateLoad(ap3, MakeIndirect(ap3->preg), size, size);
			else
				GenerateLoad(ap3, MakeIndirect(regFP), cpu.sizeOfWord, cpu.sizeOfWord);
		}
		ap1->isPtr = true;// node->etype == bt_pointer;
		ap1->preg = ap3->preg;
		ap1->mode = am_indx;
	}

	//if (!compiler.os_code)
	//	GenerateTriadic(op_base, 0, ap1, ap1, MakeImmediate(10));
	if (!node->isUnsigned)
		ap1 = ap1->GenerateSignExtend(siz1, size, flags);
	else
		ap1->MakeLegal(flags, siz1);
	return (ap1);             /* return reg */
}

Operand* CodeGenerator::GenerateClassconDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size, int64_t siz1, int su)
{
	Operand* ap1;

	ap1 = allocOperand();
	ap1->isPtr = isRefType;
	ap1->mode = am_indx;
	ap1->preg = regCLP;
	ap1->segment = dataseg;
	ap1->offset = makeinode(en_icon, node->i);
	ap1->offset->sym = node->sym;
	ap1->isUnsigned = !su;
	ap1->tp = tp;
	if (!node->isUnsigned)
		ap1 = ap1->GenerateSignExtend(siz1, size, flags);
	else
		ap1->MakeLegal(flags, siz1);
	ap1->MakeLegal(flags, size);
	return (ap1);
}

Operand* CodeGenerator::GenerateAutofconDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size)
{
	Operand* ap1;

	ap1 = allocOperand();
	ap1->isPtr = isRefType;
	ap1->mode = am_indx;
	ap1->preg = regFP;
	ap1->offset = makeinode(en_icon, node->i);
	ap1->offset->sym = node->sym;
	ap1->tp = tp;
	if (node->tp)
		switch (node->tp->precision) {
		case 8: ap1->FloatSize = 'b'; break;
		case 16: ap1->FloatSize = 'h'; break;
		case 32: ap1->FloatSize = 's'; break;
		case 64: ap1->FloatSize = ' '; break;
		case 128: ap1->FloatSize = 'q'; break;
		default: ap1->FloatSize = 'd'; break;
		}
	else
		ap1->FloatSize = ' ';
	ap1->segment = stackseg;
	if (node->tp) {
		switch (node->tp->type) {
		case bt_float:	ap1->typep = &stdflt; break;
		case bt_double:	ap1->typep = &stddouble; break;
		case bt_quad:	ap1->typep = &stdquad; break;
		case bt_posit:	ap1->typep = &stdposit; break;
		}
	}
	else {
		node->tp = TYP::Make(bt_double, cpu.sizeOfFPD);
	}
	//	    ap1->MakeLegal(flags,siz1);
	ap1->MakeLegal(flags, size);
	return (ap1);
}

Operand* CodeGenerator::GenerateAutopconDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size)
{
	Operand* ap1;

	ap1 = allocOperand();
	ap1->isPtr = isRefType;
	ap1->mode = am_indx;
	ap1->preg = regFP;
	ap1->offset = makeinode(en_icon, node->i);
	ap1->offset->sym = node->sym;
	ap1->tp = node->tp;
	if (node->tp)
		switch (node->tp->precision) {
		case 16: ap1->FloatSize = 'h'; break;
		case 32: ap1->FloatSize = 's'; break;
		default: ap1->FloatSize = ' '; break;
		}
	else
		ap1->FloatSize = ' ';
	ap1->segment = stackseg;
	switch (node->tp->type) {
	case bt_posit: ap1->typep = &stdposit; break;
	}
	//	    ap1->MakeLegal(flags,siz1);
	ap1->MakeLegal(flags, size);
	return (ap1);
}

Operand* CodeGenerator::GenerateNaconDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size, int64_t siz1, int su)
{
	Operand* ap1;
	Operand* ap2;

	ap1 = allocOperand();
	ap1->isPtr = isRefType;
	if (use_gp)
	{
		ap1->mode = am_indx;
		ap1->preg = GetSegmentIndexReg((e_sg)node->segment);
		ap1->segment = node->segment;
	}
	else
	{
		ap1->mode = am_direct;
		ap1->preg = 0;
		ap1->segment = dataseg;
	}
	ap1->offset = node;//makeinode(en_icon,node->p[0]->i);
	if (ap1->isPtr) {
		//ap2 = ap1;
		
		ap2 = GetTempRegister();
		GenerateDiadic(op_lda, 0, ap2, ap1);
		ReleaseTempRegister(ap1);
		
	}
	else
		ap2 = ap1;
	ap2->tp = tp;
	ap2->isUnsigned = !su;
	if (!node->isUnsigned)
		ap2 = ap2->GenerateSignExtend(siz1, size, flags);
	else
		ap2->MakeLegal(flags, siz1);
	ap2->isVolatile = node->isVolatile;
	switch (node->tp->type) {
	case bt_float:	ap2->typep = &stdflt; break;
	case bt_double:	ap2->typep = &stddouble; break;
	case bt_quad:	ap2->typep = &stdquad; break;
	case bt_posit:	ap2->typep = &stdposit; break;
	}
	ap2->MakeLegal(flags, size);
	return (ap2);
}

Operand* CodeGenerator::GenerateAutovconDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size)
{
	Operand* ap1, * ap3;
	int ni, nn;

	ap1 = allocOperand();
	ap1->isPtr = isRefType;
	ap1->mode = am_indx;
	ap1->preg = regFP;
	ap1->offset = makeinode(en_icon, node->i);
	ap1->offset->sym = node->sym;
	ap1->tp = tp;
	if (node->tp && node->tp->IsFloatType())
		ap1->FloatSize = (char)node->tp->lane_precision;
	else
		ap1->FloatSize = 'd';
	ap1->segment = stackseg;
	ap1->typep = &stdvector;

	ni = nn = (currentFn->depth) - (node->sym->IsParameter ? node->sym->depth + 3 : node->sym->depth);
	if (nn > 0) {
		ap3 = GetTempRegister();
		ap3->tp = tp;
		if (nn == 1)
			GenerateLoad(ap3, MakeIndirect(regFP), size, size);
		else
			GenerateLoad(ap3, MakeIndirect(regFP), cpu.sizeOfWord, cpu.sizeOfWord);
		for (--nn; nn > 0; nn--) {
			if (nn == 1)
				GenerateLoad(ap3, MakeIndirect(ap3->preg), size, size);
			else
				GenerateLoad(ap3, MakeIndirect(regFP), cpu.sizeOfWord, cpu.sizeOfWord);
		}
		ap1->isPtr = true;// node->etype == bt_pointer;
		ap1->preg = ap3->preg;
		ap1->mode = am_indx;
	}

	//	    ap1->MakeLegal(flags,siz1);
	ap1->MakeLegal(flags, size);
	return (ap1);
}

Operand* CodeGenerator::GenerateAutovmconDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size)
{
	Operand* ap1;

	ap1 = allocOperand();
	ap1->isPtr = isRefType;
	ap1->mode = am_indx;
	ap1->preg = regFP;
	ap1->offset = makeinode(en_icon, node->i);
	ap1->offset->sym = node->sym;
	ap1->tp = tp;
	if (node->tp)
		switch (node->tp->precision) {
		case 32: ap1->FloatSize = 's'; break;
		case 64: ap1->FloatSize = 'd'; break;
		default: ap1->FloatSize = 'd'; break;
		}
	else
		ap1->FloatSize = 'd';
	ap1->segment = stackseg;
	ap1->typep = &stdvectormask;
	//	    ap1->MakeLegal(flags,siz1);
	ap1->MakeLegal(flags, size);
	return (ap1);
}

Operand* CodeGenerator::GenerateRegvarDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size)
{
	Operand* ap1;

	ap1 = allocOperand();
	ap1->isPtr = isRefType;

	// For parameters we want Rn, for others [Rn]
	// This seems like an error earlier in the compiler
	// See setting val_flag in ParseExpressions
	ap1->mode = (IsArgReg(node->rg)) ? am_reg : am_ind;
	//		ap1->mode = node->p[0]->tp->val_flag ? am_reg : am_ind;
	ap1->preg = node->rg;
	ap1->tp = node->tp;
	if (node->tp)
		ap1->isUnsigned = node->tp->isUnsigned;
	ap1->MakeLegal(flags, size);
	Leave((char *)"Genderef", 3);
	return (ap1);
}

// Dead code??? the register file is unified for Thor so register references 
// are strictly to the GPRs.

Operand* CodeGenerator::GenerateFPRegvarDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size)
{
	Operand* ap1;

	/*error(ERR_DEREF)*/;
	ap1 = allocOperand();
	ap1->isPtr = isRefType;
	ap1->mode = (IsArgReg(node->rg)) ? am_reg : am_ind;
	ap1->preg = node->rg;
	ap1->tp = tp;
	switch (node->tp->type) {
	case bt_float:	ap1->typep = &stdflt; break;
	case bt_double:	ap1->typep = &stddouble; break;
	case bt_quad:	ap1->typep = &stdquad; break;
	case bt_posit:	ap1->typep = &stdposit; break;
	}
	ap1->MakeLegal(flags, size);
	Leave((char *)"</Genderef>", 3);
	return (ap1);
}

// Dead code??? as above

Operand* CodeGenerator::GeneratePositRegvarDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size)
{
	Operand* ap1;

	/*error(ERR_DEREF)*/;
	ap1 = allocOperand();
	ap1->isPtr = isRefType;
	ap1->mode = (IsArgReg(node->rg)) ? am_reg : am_ind;
	ap1->preg = node->rg;
	ap1->tp = tp;
	switch (node->tp->type) {
	case bt_posit:	ap1->typep = &stdposit; break;
	}
	ap1->MakeLegal(flags, size);
	Leave((char *)"</Genderef>", 3);
	return (ap1);
}

Operand* CodeGenerator::GenerateLabconDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size, int64_t siz1, int su)
{
	Operand* ap1;

	ap1 = allocOperand();
	ap1->isPtr = isRefType;
	if (use_gp)
	{
		ap1->mode = am_indx;
		ap1->preg = GetSegmentIndexReg((e_sg)node->segment);
		ap1->segment = node->segment;
	}
	else
	{
		ap1->mode = am_direct;
		ap1->preg = 0;
		ap1->segment = node->segment;
	}
	ap1->offset = node;//makeinode(en_icon,node->p[0]->i);
	ap1->tp = tp;
	ap1->isUnsigned = !su;
	if (!node->isUnsigned)
		ap1 = ap1->GenerateSignExtend(siz1, size, flags);
	else
		ap1->MakeLegal(flags, siz1);
	ap1->isVolatile = node->isVolatile;
	ap1->MakeLegal(flags, size);
	return (ap1);
}

Operand* CodeGenerator::GenerateBitoffsetDereference(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size, int opt)
{
	Operand* ap1, * ap2, * ap3, * ap4;
	OCODE* ip;

//	ap5 = GenerateBitfieldExtract(node->p[0], node->p[1], node->p[2]);
//	return (ap5);

	ap4 = GetTempRegister();
	ap1 = GenerateExpression(node->p[0], am_reg, cpu.sizeOfWord,0);
	if (opt) {
		ip = currentFn->pl.tail;
		ap2 = GenerateExpression(node->p[1], am_reg | am_imm | am_imm0, cpu.sizeOfWord,1);
		ap3 = GenerateExpression(node->p[2], am_reg | am_imm | am_imm0, cpu.sizeOfWord,1);
		if (ap2->mode != ap3->mode) {
			ReleaseTempReg(ap3);
			ReleaseTempReg(ap2);
			currentFn->pl.tail = ip;
			ap2 = GenerateExpression(node->p[1], am_reg, cpu.sizeOfWord,1);
			ap3 = GenerateExpression(node->p[2], am_reg, cpu.sizeOfWord,1);
		}
		ReleaseTempReg(ap3);
		ReleaseTempReg(ap2);
		ReleaseTempReg(ap1);
	}
	else
		ap4 = ap1;
	ap4->bit_offset = node->p[1];
	ap4->bit_width = node->p[2];
	//ap4->isPtr = node->IsRefType();

	// For parameters we want Rn, for others [Rn]
	// This seems like an error earlier in the compiler
	// See setting val_flag in ParseExpressions
	//		ap1->mode = node->p[0]->rg < regFirstArg ? am_ind : am_reg;
			//		ap1->mode = node->p[0]->tp->val_flag ? am_reg : am_ind;
	//		ap1->preg = node->p[0]->rg;
	ap4->tp = tp;
	ap4->isUnsigned = node->tp->isUnsigned;
	ap4->MakeLegal(flags, size);
	Leave((char *)"Genderef", 3);
	return (ap4);
}

Operand* CodeGenerator::GenerateDereference2(ENODE* node, TYP* tp, bool isRefType, int flags, int64_t size, int64_t siz1, int su, int opt)
{
	Operand* ap1, * ap2, * ap3;

	if (node == nullptr)
		return (nullptr);
	switch (node->nodetype) {
	case en_fieldref: return (GenerateFieldrefDereference(node, tp, isRefType, flags, size));
	case en_asadd:	return (GenerateAsaddDereference(node, tp, isRefType, flags, size, siz1, su, false));
	case en_assub:	return (GenerateAsaddDereference(node, tp, isRefType, flags, size, siz1, su, true));
	case en_add: return (GenerateAddDereference(node, tp, isRefType, flags, size, siz1, su));
	case en_autocon: return (GenerateAutoconDereference(node, tp, isRefType, flags, size, siz1, su));
	case en_classcon: return (GenerateClassconDereference(node, tp, isRefType, flags, size, siz1, su));
	case en_autofcon: return (GenerateAutofconDereference(node, tp, isRefType, flags, size));
	case en_autopcon: return (GenerateAutopconDereference(node, tp, isRefType, flags, size));
	case en_nacon: return (GenerateNaconDereference(node, tp, isRefType, flags, size, siz1, su));
	case en_autovcon: return (GenerateAutovconDereference(node, tp, isRefType, flags, size));
	case en_autovmcon: return (GenerateAutovmconDereference(node, tp, isRefType, flags, size));
	case en_labcon: return (GenerateLabconDereference(node, tp, isRefType, flags, size, siz1, su));
	case en_regvar: return (GenerateRegvarDereference(node, tp, isRefType, flags, size));
	case en_fpregvar: return (GenerateFPRegvarDereference(node, tp, isRefType, flags, size));
	case en_pregvar: return (GeneratePositRegvarDereference(node, tp, isRefType, flags, size));
	case en_bitoffset: return (GenerateBitoffsetDereference(node, tp, isRefType, flags, size, opt));

	// Dereferencing a reference, I think the two operations cancel out. I tried
	// just a return here, and the compiler seems to work.

	case en_ref:
		return (nullptr);
		// Dead code
		if (false) {
			return (GenerateDereference(node->p[0], flags, size, su, opt, 1));
			ap2 = GetTempRegister();
			ap1 = GenerateExpression(node, am_reg, cpu.sizeOfWord, 0);
			ap1->isPtr = isRefType;
			ap1->tp = tp;
			ap1->segment = dataseg;
			ap2->tp = tp;
			//ap1->MakeLegal(flags, size);
			//ap2->isPtr = TRUE;
			//ap1->mode = am_ind;
			if (ap1->isPtr) {
				ap3 = MakeIndirect(ap1->preg);
				GenerateLoad(ap2, ap3, size, size);
				ReleaseTempRegister(ap3);
				ReleaseTempRegister(ap1);

				//ap2->MakeLegal(flags, size);
				return (ap2);
			}
			return (ap2);
		}
		//		return (GenerateDereference2(node->p[0], tp, isRefType, flags, size, siz1, su, opt));
		return (GenerateDereference(node->p[0], flags, size, su, opt, 0));
	
	// Should not get an en_type as it is just an artifact of typecasting
	// containing just a type and no variable. It should have been processed and
	// removed by optimization.

	case en_type:
		return (nullptr);
	default:
		return (nullptr);
	}
/*
	if (node->nodetype == en_vex) {
		Operand* ap2;
		if (node->nodetype == en_vector_ref) {
			ap1 = GenerateDereference(node->p[0], am_reg, 8, 0, 0);
			ap2 = GenerateExpression(node->p[1], am_reg, 8);
			if (ap1->offset && ap2->offset) {
				GenerateTriadic(op_add, 0, ap1, makereg(0), MakeImmediate(ap2->offset->i));
			}
			ReleaseTempReg(ap2);
			//ap1->mode = node->p[0]->i < 18 ? am_ind : am_reg;
			//ap1->preg = node->p[0]->i;
			ap1->type = stdvector.GetIndex();
			ap1->MakeLegal(flags, size);
			return (ap1);
		}
	}
*/
	return (nullptr);
}

//
//  Return the addressing mode of a dereferenced node.
//
Operand *CodeGenerator::GenerateDereference(ENODE *node,int flags,int64_t size, int su, int opt, int rhs)
{    
	Operand *ap1, *ap2, *ap3;
  int64_t siz1;

	/* level is a cheezy hack to get dereferencing working. It almost works except
		at the top level where it does an extra load. */
	static int level = 0;

  Enter((char *)"<Genderef>");
	level++;
	siz1 = node->GetReferenceSize();
	// When dereferencing a struct or union return a pointer to the struct or
	// union.
	//if (node->tp)
	//	if (node->tp->type==bt_struct || node->tp->type==bt_union) {
	//		return GenerateExpression(node, am_reg | am_mem, size);
	//	}
	if (node->p[0] == nullptr) {
		if (rhs) {
			ap2 = GetTempRegister();
			ap1 = GenerateExpression(node->p[0], am_reg, size, 1);
			ap2->tp = ap1->tp;
			ap1->isPtr = node->IsRefType();
			//ap1->tp = node->tp;
			ap1->segment = dataseg;
			//ap1->MakeLegal(flags, size);
			//ap2->isPtr = TRUE;
			//ap1->mode = am_ind;
			ap3 = MakeIndirect(ap1->preg);
			GenerateLoad(ap2, ap3, size, size);
			ReleaseTempRegister(ap3);
			ReleaseTempRegister(ap1);
			ap1 = ap2;
		}
		else {
			ap1 = GetTempRegister();// GenerateExpression(node->p[0], am_reg, cpu.sizeOfWord, 0);
		}
	}
//	ap1 = GenerateDereference2(node, node->tp, node->IsRefType(), flags, size, siz1, su, opt);
	else
		ap1 = GenerateDereference2(node->p[0], node->tp, node->IsRefType(), flags, size, siz1, su, opt);
	if (ap1) {
		ap1->rhs = rhs;
		if (node->nodetype == en_fieldref) {
			ap1->bit_offset = node->bit_offset;
			ap1->bit_width = node->bit_width;
		}
		level--;
		return(ap1);
	}
	ap1 = GenerateExpression(node->p[0], am_reg | am_imm, size, rhs); // generate address
	if (ap1 == nullptr) {
		level--;
		return (nullptr);
	}
	// Not sure about this. A en_fieldref will point to an en_ref. Field specs
	// were dereferencing twice, so testing en_fieldref is a hack to prevent that.
	// Seems to be some other issue in the dereference logic, other dereferences
	// seem to work.
	ap1->isPtr = node->p[0]->IsRefType() && node->nodetype != en_fieldref;
	ap1->rhs = rhs;
	if(ap1->mode == am_reg)
  {
			// This seems a bit of a kludge. If we are dereferencing and there's a
			// pointer in the register, then we want the value at the pointer location.
			// Makes the ch=*s work in: while (ch = *s) { DBGDisplayChar(ch); s++; }
			// But it breaks: the *su = uc; so we want to do this only for the RHS.
			if (ap1->isPtr && rhs) {// && level > 1){// && !IsLValue(node)) {
						
				int64_t sz = node->GetReferenceSize();
				int rg = ap1->preg;
				TYP* tp = ap1->tp;
				ReleaseTempRegister(ap1);
				ap1 = GetTempRegister();
				ap1->tp = tp;
				GenerateLoad(ap1, MakeIndirect(rg), sz, sz);
				ap1->mode = am_reg;
				ap1->isPtr = node->p[0]->IsRefType();
				
			}
			else
			{
				if (!ap1->argref && FALSE) {
					//        ap1->mode = am_ind;
					if (use_gp) {
						ap1->mode = am_indx;
						ap1->preg = GetSegmentIndexReg((e_sg)node->segment);
					}
					else
						ap1->mode = am_ind;
					if (node->p[0]->constflag == TRUE)
						;// ap1->offset = node->p[0];
					else
						ap1->offset = nullptr;	// ****
					ap1->isUnsigned = !su | ap1->isPtr;
					if (!node->isUnsigned)
						ap1 = ap1->GenerateSignExtend(siz1, size, flags);
					else
						ap1->MakeLegal(flags, siz1);
					ap1->isVolatile = node->isVolatile;
				}
				ap1->MakeLegal(flags, size);
			}
			goto xit;
    }
	// Note sure about this, but immediate were being incorrectly
	// dereferenced as direct addresses because it would fall through
	// to the following dead code.
	
	if (ap1->mode == am_imm) {
		ap1->MakeLegal( flags, size);
		goto xit;
	}
	
	// *********************************************************************
	// I think what follows is dead code.
	// am_reg and am_imm the only codes that should be generated are
	// checked for above.
	// *********************************************************************

	// See segments notes
	//if (node->p[0]->nodetype == en_labcon &&
	//	node->p[0]->etype == bt_pointer && node->p[0]->constflag)
	//	ap1->segment = codeseg;
	//else
	//	ap1->segment = dataseg;
	if (use_gp) {
    ap1->mode = am_indx;
		ap1->preg = GetSegmentIndexReg((e_sg)node->segment);
    ap1->segment = dataseg;
  }
  else {
//    ap1->mode = am_direct;
	  ap1->isUnsigned = !su | ap1->isPtr;
  }
	if (ap1->isPtr) {
//		ap3 = GetTempRegister();
		ap2 = GetTempRegister();
		GenerateDiadic(op_ldo, 0, ap2, ap1);
//		GenLoad(ap3, MakeIndirect(ap2->preg), size, size);
//		ReleaseTempRegister(ap2);
		ap2->MakeLegal(flags, 8);
		level--;
		return (ap2);
	}
//    ap1->offset = makeinode(en_icon,node->p[0]->i);
  ap1->isUnsigned = !su | ap1->isPtr;
	if (!node->isUnsigned)
	    ap1 = ap1->GenerateSignExtend(siz1,size,flags);
	else
		ap1->MakeLegal(flags,siz1);
  ap1->isVolatile = node->isVolatile;
  ap1->MakeLegal(flags,size);
xit:
	level--;
  Leave((char *)"</Genderef>",0);
  return (ap1);
}


void CodeGenerator::GenMemop(int op, Operand *ap1, Operand *ap2, int64_t ssize, int typ)
{
	Operand *ap3;

	if (TYP::IsFloatType(typ)) {
		ap3 = GetTempFPRegister();
		ap3->tp = ap1->tp;
		GenerateLoad(ap3, ap1, ssize, ssize);
		GenerateTriadic(op, 0, ap3, ap3, ap2);
		GenerateStore(ap3, ap1, ssize);
		ReleaseTempReg(ap3);
		return;
	}
	if (typ == bt_posit) {
		ap3 = GetTempPositRegister();
		ap3->tp = ap1->tp;
		GenerateLoad(ap3, ap1, ssize, ssize);
		GenerateTriadic(op, 0, ap3, ap3, ap2);
		GenerateStore(ap3, ap1, ssize);
		ReleaseTempReg(ap3);
		return;
	}
	if (ap1->typep==&stddouble) {
    ap3 = GetTempFPRegister();
		ap3->tp = ap1->tp;
		GenerateLoad(ap3,ap1,ssize,ssize);
		GenerateTriadic(op,ap1->FloatSize,ap3,ap3,ap2);
		GenerateStore(ap3,ap1,ssize);
		ReleaseTempReg(ap3);
		return;
	}
	if (ap1->typep == &stdposit) {
		ap3 = GetTempPositRegister();
		ap3->tp = ap1->tp;
		GenerateLoad(ap3, ap1, ssize, ssize);
		GenerateTriadic(op, ap1->FloatSize, ap3, ap3, ap2);
		GenerateStore(ap3, ap1, ssize);
		ReleaseTempReg(ap3);
		return;
	}
	if (ap1->typep==&stdvector) {
 		ap3 = GetTempVectorRegister();
		ap3->tp = ap1->tp;
		GenerateLoad(ap3,ap1,ssize,ssize);
		GenerateTriadic(op,0,ap3,ap3,ap2);
		GenerateStore(ap3,ap1,ssize);
		ReleaseTempReg(ap3);
		return;
	}
	//if (ap1->mode != am_indx2)
	// Increment / decrement not supported
	if (0) {
		if (op==op_add && ap2->mode==am_imm && ap2->offset->i >= -16 && ap2->offset->i < 16 && ssize==8) {
			GenerateDiadic(op_inc,0,ap1,ap2);
			return;
		}
		if (op==op_sub && ap2->mode==am_imm && ap2->offset->i >= -15 && ap2->offset->i < 15 && ssize==8) {
			GenerateDiadic(op_dec,0,ap1,ap2);
			return;
		}
	}
 	ap3 = GetTempRegister();
	ap3->isPtr = ap1->isPtr;
	ap3->tp = ap1->tp;
	GenerateLoad(ap3,ap1,ssize,ssize);
	if (op == op_add) {
		if (ap2->mode == am_imm)
			GenerateAddImmediate(ap3, ap3, ap2);
		else
			GenerateAdd(ap3, ap3, ap2);
	}
	else
		GenerateTriadic(op,0,ap3,ap3,ap2);
	GenerateStore(ap3,ap1,ssize);
	ReleaseTempReg(ap3);
}

Operand* CodeGenerator::GenerateBitfieldAssignAdd(ENODE* node, int flags, int64_t size, int op)
{
	Operand* ap1, * ap2, * ap3, * ap4;
	int64_t ssize;

	ssize = node->p[0]->GetNaturalSize();
	if (ssize > size)
		size = ssize;
	ap3 = GetTempRegister();
	ap1 = GenerateBitfieldDereference(node->p[0], am_reg | am_mem, size, 1);
	ap3->tp = ap1->tp;
	//		GenerateDiadic(op_mov, 0, ap3, ap1);
	//ap1 = cg.GenerateExpression(p[0], am_reg | am_mem, size);
	ap2 = GenerateExpression(node->p[1], am_reg | am_imm, size, 1);
	if (ap1->mode == am_reg) {
		GenerateTriadic(op, 0, ap1, ap1, ap2);
		//			if (ap1->bit_offset < 0)
		//				GenerateBitfieldInsert(ap3, ap1, ap1->next, MakeImmediate(1));
		//			else
		GenerateBitfieldInsert(ap3, ap1, ap1->bit_offset, ap1->bit_width);
		//cg.GenerateBitfieldInsert(ap3, ap1, ap1->offset->bit_offset, ap1->offset->bit_width);
	}
	else {
		GenerateLoad(ap3, ap1, size, size);
		//Generate4adic(op_bfext, 0, ap4, ap3, MakeImmediate(ap1->offset->bit_offset), MakeImmediate(ap1->offset->bit_width-1));
		ap4 = GenerateBitfieldExtract(ap3, ap1->bit_offset, ap1->bit_width);
		GenerateTriadic(op, 0, ap4, ap4, ap2);
		GenerateBitfieldInsert(ap3, ap4, ap1->bit_offset, ap1->bit_width);
		node->GenStore(ap3, ap1, ssize);
		ReleaseTempReg(ap4);
	}
	ReleaseTempReg(ap2);
	ReleaseTempReg(ap1);
	ap3->MakeLegal(flags, size);
	return (ap3);
}

Operand* CodeGenerator::GenerateVectorBinaryFloat(ENODE* node, int flags, int64_t size, e_op op)
{
	Operand* ap1 = nullptr, * ap2 = nullptr, * ap3, * ap4, *apm;
	bool dup = false;
	bool vec = false;
	int flags2;

	apm = nullptr;
	ap3 = GetTempVectorRegister();
	ap4 = GetTempVectorRegister();
	if (ENODE::IsEqual(node->p[0], node->p[1]))
		dup = !opt_nocgo;
	switch (op) {
	case op_vadds: op = op_vfadds; break;
	case op_vmul: op = op_vfmul; break;
	case op_fadd: op = op_vfadd; break;
	case op_fsub: op = op_vfsub; break;
	case op_fmul: op = op_vfmul; break;
	}
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op)->amclass2, size, 0);
	if (!dup) {
		flags2 = Instruction::Get(op)->amclass3;
		ap2 = cg.GenerateExpression(node->p[1], flags2, size, 1);
	}
	if (node->vmask)
		apm = cg.GenerateExpression(node->vmask, am_reg, size, 0);

	// Generate a convert operation ?
	if (!dup) {
		if (ap1->fpsize() != ap2->fpsize()) {
			if (ap2->fpsize() == 's')
				GenerateDiadic(op_fcvtsq, 0, ap2, ap2);
		}
	}
	// Two immediate operands not supported.
	if (ap1->mode == am_imm && ap2->mode == am_imm) {
		GenerateLoadFloatConst(ap4, ap1);
		ap1 = ap4;
	}
	if (apm) {
		if (dup)
			Generate4adic(op, node->tp->lane_precision, ap3, ap1, ap1, apm);
		else
			Generate4adic(op, node->tp->lane_precision, ap3, ap1, ap2, apm);
	}
	else {
		if (dup)
			GenerateTriadic(op, node->tp->lane_precision, ap3, ap1, ap1);
		else
			GenerateTriadic(op, node->tp->lane_precision, ap3, ap1, ap2);
	}
	ap3->type = ap1->type;

	if (apm)
		ReleaseTempRegister(apm);
	if (ap2)
		ReleaseTempReg(ap2);
	if (ap1)
		ReleaseTempReg(ap1);
	ReleaseTempReg(ap4);
	return (ap3);
}

Operand* CodeGenerator::GenerateVectorBinary(ENODE* node, int flags, int64_t size, e_op op)
{
	Operand* ap1 = nullptr, * ap2 = nullptr, * ap3, * ap4, * apm;
	bool dup = false;
	bool vec = false;
	int flags2;

	if (node->IsFloatType())
		return (GenerateVectorBinaryFloat(node, flags, size, op));

	ap3 = GetTempVectorRegister();
	ap4 = GetTempVectorRegister();
	if (ENODE::IsEqual(node->p[0], node->p[1]))
		dup = !opt_nocgo;
	switch (op) {
	case op_add: op = op_vadd; break;
	case op_sub: op = op_vsub; break;
	case op_mul: op = op_vmul; break;
	}
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op)->amclass2, size, 0);
	if (!dup) {
		flags2 = Instruction::Get(op)->amclass3;
		ap2 = cg.GenerateExpression(node->p[1], flags2, size, 1);
	}
	apm = cg.GenerateExpression(node->vmask, am_reg, size, 0);

	// Generate a convert operation ?
	if (!dup) {
		if (ap1->fpsize() != ap2->fpsize()) {
			if (ap2->fpsize() == 's')
				GenerateDiadic(op_fcvtsq, 0, ap2, ap2);
		}
	}
	// Two immediate operands not supported.
	if (ap1->mode == am_imm && ap2->mode == am_imm) {
		GenerateLoadFloatConst(ap4, ap1);
		ap1 = ap4;
	}
	if (dup)
		Generate4adic(op, 0, ap3, ap1, ap1, apm);
	else
		Generate4adic(op, 0, ap3, ap1, ap2, apm);
	ap3->type = ap1->type;

	if (ap2)
		ReleaseTempReg(ap2);
	if (ap1)
		ReleaseTempReg(ap1);
	ReleaseTempReg(ap4);
	return (ap3);
}

Operand* CodeGenerator::GenerateBinaryFloat(ENODE* node, int flags, int64_t size, e_op op)
{
	Operand* ap1 = nullptr, * ap2 = nullptr, * ap3, * ap4;
	bool dup = false;
	bool vec = false;

	if (node->IsVectorType())
		return (GenerateVectorBinary(node, flags, size, op));

	ap3 = GetTempRegister();
	ap4 = GetTempRegister();
	if (ENODE::IsEqual(node->p[0], node->p[1]))
		dup = !opt_nocgo;
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op)->amclass2, size, 0);
	if (!dup) {
		ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op)->amclass3, size, 1);
	}
	if (ap1->mode == am_vreg || (ap2 && ap2->mode == am_vreg)) {
		vec = true;
		op = op_vfmul;
	}
	// Generate a convert operation ?
	if (!dup) {
		if (ap1->fpsize() != ap2->fpsize()) {
			if (ap2->fpsize() == 's')
				GenerateDiadic(op_fcvtsq, 0, ap2, ap2);
		}
	}
	// Two immediate operands not supported.
	if (ap1->mode == am_imm && ap2->mode == am_imm) {
		GenerateLoadFloatConst(ap4, ap1);
		ap1 = ap4;
	}
	if (dup)
		GenerateTriadic(op, 0, ap3, ap1, ap1);
	else
		GenerateTriadic(op, 0, ap3, ap1, ap2);
	ap3->type = ap1->type;

	if (ap2)
		ReleaseTempReg(ap2);
	if (ap1)
		ReleaseTempReg(ap1);
	ap3->MakeLegal(flags, size);
	return (ap3);
}

Operand* CodeGenerator::GenerateAssignAdd(ENODE* node, int flags, int64_t size, int op)
{
	Operand* ap1, * ap2, * ap3;
	int64_t ssize;
	bool negf = false;
	bool intreg = false;
	MachineReg* mr;

	if (node->p[0]->IsBitfield())
		return (GenerateBitfieldAssignAdd(node, flags, size, op));

	ssize = node->p[0]->GetNaturalSize();
	if (ssize > size)
		size = ssize;

	// Select which opcode is being used.
	if (node->IsFloatType()) {
		if (op == op_add)
			op = op_fadd;
		else if (op == op_sub)
			op = op_fsub;
	}
	else if (node->etype == bt_vector) {
		if (op == op_add)
			op = op_vadd;
		else if (op == op_sub)
			op = op_vsub;
	}
	else {
		intreg = true;
	}

	ap1 = GenerateExpression(node->p[0], am_all,/*Instruction::Get(op)->amclass1,*/ ssize, 0);	// must allow memory here
	ap2 = GenerateExpression(node->p[1], Instruction::Get(op)->amclass3, size, 1);
	if (ap1->mode == am_reg) {
		// Check if an immediate value will fit into the 21-bit immediate field. If
		// not it needs to be loaded into a register.
		if (ap2->mode == am_imm) {
			if (ap2->offset) {
				if (!ap2->offset->i128.IsNBit(21)) {
					ap3 = GetTempRegister();
					cg.GenerateLoadConst(ap2, ap3);
					if (op == op_add)
						GenerateAddImmediate(ap1, ap1, ap2);
					else
						GenerateTriadic(op, 0, ap1, ap1, ap3);
					ReleaseTempRegister(ap3);
					goto j1;
				}
			}
		}
		if (ap2->mode == am_imm && op == op_add)
			GenerateAddImmediate(ap1, ap1, ap2);
		else
			GenerateTriadic(op, 0, ap1, ap1, ap2);
j1:
		if (intreg) {
			mr = &regs[ap1->preg];
			if (mr->assigned)
				mr->modified = true;
			mr->assigned = true;
			mr->isConst = ap1->isConst && ap2->isConst;
		}
	}
	//else if (ap1->mode == am_fpreg) {
	//	GenerateTriadic(op, ap1->fpsize(), ap1, ap1, ap2);
	//	ReleaseTempReg(ap2);
	//	ap1->MakeLegal( flags, size);
	//	return (ap1);
	//}
	else {
		GenMemop(op, ap1, ap2, ssize, node->etype);
	}
	ReleaseTempReg(ap2);
	//if (ap1->type != stddouble.GetIndex() && !ap1->isUnsigned)
	//	ap1 = ap1->GenSignExtend(ssize, size, flags);
	ap1->MakeLegal(flags, size);
	return (ap1);
}

//
//      generate a *= node.
//
Operand *CodeGenerator::GenerateAssignMultiply(ENODE *node,int flags, int64_t size, int op)
{
	Operand *ap1, *ap2, *ap3;
  int64_t ssize;
	MachineReg *mr;
	int typ;

	typ = node->etype;
  ssize = node->p[0]->GetNaturalSize();
  if( ssize > size )
    size = ssize;
	if (node->p[0]->IsBitfield()) {
		ap3 = GetTempRegister();
		ap1 = GenerateBitfieldDereference(node->p[0], am_reg | am_mem, size, 1);
		ap3->tp = ap1->tp;
		if (ap1->mode == am_reg)
			GenerateMove(ap3, ap1);
		else
			GenerateLoad(ap3, ap1, size, size);
		ap2 = GenerateExpression(node->p[1], am_reg | am_imm, size, 1);
		GenerateTriadic(op, 0, ap1, ap1, ap2);
//		ap4 = GenerateExpression(ap1->offset->bit_offset, am_reg | am_imm | am_imm0, cpu.sizeOfWord, 1);
//		if (ap4 < 0)
//			GenerateBitfieldInsert(ap3, ap1, ap1->next, MakeImmediate(1));
//		else
			GenerateBitfieldInsert(ap3, ap1, ap1->offset->bit_offset, ap1->offset->bit_width);
		GenerateStore(ap3, ap1->next, ssize);
		ReleaseTempReg(ap2);
		ReleaseTempReg(ap1->next);
		ReleaseTempReg(ap1);
		ap3->MakeLegal( flags, size);
		return (ap3);
	}
	if (node->IsFloatType()) {
    ap1 = GenerateExpression(node->p[0],am_reg | am_mem,ssize,0);
    ap2 = GenerateExpression(node->p[1],am_reg,size,1);
    op = op_fmul;
  }
	else if (node->IsPositType()) {
		ap1 = GenerateExpression(node->p[0], am_reg | am_mem, ssize,0);
		ap2 = GenerateExpression(node->p[1], am_reg, size,1);
		op = op_pmul;
	}
	else if (node->etype==bt_vector) {
    ap1 = GenerateExpression(node->p[0],am_reg | am_mem,ssize,0);
    ap2 = GenerateExpression(node->p[1],am_reg,size,1);
		//op = ap2->type==stdvector.GetIndex() ? op_vmul : op_vmuls;
		op = ap2->typep == &stdvector ? op_vmul : op_vmuls;
	}
  else {
    ap1 = GenerateExpression(node->p[0],am_all & ~am_imm & ~am_fpreg,ssize,0);
    ap2 = GenerateExpression(node->p[1],am_reg | am_imm,size,1);
  }
	if (ap1->mode==am_reg) {
		// Check if an immediate value will fit into the 21-bit immediate field. If
		// not it needs to be loaded into a register.
		if (ap2->mode == am_imm) {
			if (ap2->offset) {
				if (!ap2->offset->i128.IsNBit(21)) {
					ap3 = GetTempRegister();
					cg.GenerateLoadConst(ap2, ap3);
					GenerateTriadic(op, 0, ap1, ap1, ap3);
					ReleaseTempRegister(ap3);
					goto j1;
				}
			}
		}
		GenerateTriadic(op,0,ap1,ap1,ap2);
j1:
		if (op == op_mulu || op == op_mul) {
			mr = &regs[ap1->preg];
			if (mr->assigned)
				mr->modified = true;
			mr->assigned = true;
			mr->isConst = ap1->isConst && ap2->isConst;
		}
	}
	else if (ap1->tp->IsFloatType()) {
		ap3 = GetTempRegister();
		ap3->tp = ap1->tp;
		GenerateLoad(ap3, ap1, ssize, ssize);
	  GenerateTriadic(op,0, ap3,ap3,ap2);
		GenerateStore(ap3, ap1, ssize);
		ReleaseTempRegister(ap3);
	  ReleaseTempReg(ap2);
	  ap1->MakeLegal(flags,size);
		return (ap1);
	}
	else if (ap1->tp->IsPositType()) {
		GenerateTriadic(op, ssize == 4 ? 's' : ssize == 8 ? ' ' : ssize == 2 ? 'h' : ' ', ap1, ap1, ap2);
		ReleaseTempReg(ap2);
		ap1->MakeLegal(flags, size);
		return (ap1);
	}
	else {
		GenMemop(op, ap1, ap2, ssize, typ);
	}
    ReleaseTempReg(ap2);
    ap1 = ap1->GenerateSignExtend(ssize,size,flags);
    ap1->MakeLegal(flags,size);
    return (ap1);
}

/*
 *      generate /= and %= nodes.
 */
Operand *CodeGenerator::GenerateAssignModiv(ENODE *node,int flags,int64_t size,int op)
{
	Operand *ap1, *ap2, *ap3;
  int64_t siz1;
  int isFP;
	bool isPosit;
	MachineReg *mr;
	bool cnst = false;
 
    siz1 = node->p[0]->GetNaturalSize();
	if (node->p[0]->IsBitfield()) {
		ap3 = GetTempRegister();
		ap1 = GenerateBitfieldDereference(node->p[0], am_reg | am_mem, size, 1);
		ap3->tp = ap1->tp;
		if (ap1->mode == am_reg)
			GenerateMove(ap3, ap1);
		else
			GenerateLoad(ap3, ap1, size, size);
		ap2 = GenerateExpression(node->p[1], am_reg | am_imm, size,1);
		GenerateTriadic(op, 0, ap1, ap1, ap2);
//		if (ap1->offset->bit_offset < 0)
//			GenerateBitfieldInsert(ap3, ap1, ap1->next, MakeImmediate(1));
//		else
			GenerateBitfieldInsert(ap3, ap1, ap1->offset->bit_offset, ap1->offset->bit_width);
		GenerateStore(ap3, ap1->next, siz1);
		ReleaseTempReg(ap2);
		ReleaseTempReg(ap1->next);
		ReleaseTempReg(ap1);
		ap3->MakeLegal( flags, size);
		return (ap3);
	}
	isFP = node->etype==bt_double || node->etype==bt_float || node->etype==bt_quad;
	isPosit = node->etype == bt_posit;
    if (isFP) {
        if (op==op_div || op==op_divu)
           op = op_fdiv;
        ap1 = GenerateExpression(node->p[0],am_reg,siz1,1);
        ap2 = GenerateExpression(node->p[1],am_reg,size,1);
		GenerateTriadic(op,siz1==4?'s':siz1==8?' ':siz1==12?'t':siz1==16?'q':'d',ap1,ap1,ap2);
	    ReleaseTempReg(ap2);
		ap1->MakeLegal(flags,size);
	    return (ap1);
//        else if (op==op_mod || op==op_modu)
//           op = op_fdmod;
    }
		else if (isPosit) {
			if (op == op_div || op == op_divu)
				op = op_pdiv;
			ap1 = GenerateExpression(node->p[0], am_reg, siz1,1);
			ap2 = GenerateExpression(node->p[1], am_reg, size,1);
			GenerateTriadic(op, siz1 == 4 ? 's' : siz1 == 8 ? ' ' : siz1 == 16 ? 'q' : 'd', ap1, ap1, ap2);
			ReleaseTempReg(ap2);
			ap1->MakeLegal(flags, size);
			return (ap1);
			//        else if (op==op_mod || op==op_modu)
			//           op = op_fdmod;
		}
		else {
        ap1 = GetTempRegister();
        ap2 = GenerateExpression(node->p[0],am_all & ~am_imm & ~am_fpreg,siz1,1);
    }
	if (ap2->mode==am_reg && ap2->preg != ap1->preg)
		GenerateMove(ap1,ap2);
	//else if (ap2->mode==am_fpreg && ap2->preg != ap1->preg)
	//	GenerateDiadic(op_mov,0,ap1,ap2);
	else
        GenerateLoad(ap1,ap2,siz1,siz1);
    //GenerateSignExtend(ap1,siz1,2,flags);
    if (isFP)
        ap3 = GenerateExpression(node->p[1],am_reg,cpu.sizeOfWord, 1);
		else {
			// modu doesn't support immediate mode
			ap3 = GenerateExpression(node->p[1], op==op_modu ? am_reg : am_reg | am_imm, cpu.sizeOfWord, 1);
		}
	if (op==op_fdiv) {
		GenerateTriadic(op,siz1==4?'s':siz1==8?' ':siz1==12?'t':siz1==16?'q':'d',ap1,ap1,ap3);
	}
	else {
		GenerateTriadic(op, 0, ap1, ap1, ap3);
		cnst = ap1->isConst && ap3->isConst;
		mr = &regs[ap1->preg];
		if (mr->assigned)
			mr->modified = true;
		mr->assigned = true;
		mr->isConst = cnst;
	}
  ReleaseTempReg(ap3);
  //GenerateDiadic(op_ext,0,ap1,0);
	if (ap2->mode == am_reg) {
		GenerateMove(ap2, ap1);
		mr = &regs[ap2->preg];
		if (mr->assigned)
			mr->modified = true;
		mr->assigned = true;
		mr->isConst = cnst;
	}
	//else if (ap2->mode==am_fpreg)
	//	GenerateDiadic(op_mov,0,ap2,ap1);
	else
	    GenerateStore(ap1,ap2,siz1);
    ReleaseTempReg(ap2);
	if (!isFP)
		ap1->MakeLegal(flags,size);
    return (ap1);
}

// This little bit of code a debugging aid.
// Dumps the expression nodes associated with an aggregate assignment.

void DumpStructEnodes(ENODE *node)
{
	ENODE *head;
	TYP *tp;

	lfs.puts("{");
	head = node;
	while (head) {
		tp = head->tp;
		if (tp)
			tp->put_ty();
		if (head->nodetype==en_aggregate) {
			DumpStructEnodes(head->p[0]);
		}
		if (head->nodetype == en_icon) {
			char buf[50];
			sprintf_s(buf, sizeof(buf), "%I64d", head->i);
			lfs.puts(buf);
		}
		head = head->p[1];
	}
	lfs.printf("}");
}


// Generate an assignment to a structure type. The type passed must be a
// structure type.

void CodeGenerator::GenerateStructAssign(TYP *tp, int64_t offset, ENODE *ep, Operand *base)
{
	Symbol *thead, *first;
	Operand *ap1, *ap2;
	int64_t offset2;

	first = thead = tp->lst.headp;
	ep = ep->p[0];
	while (thead) {
		if (ep == nullptr)
			break;
		if (thead->tp->IsAggregateType()) {
			/*
			if (thead->tp->isArray) {
				if (ep->p[2])
					GenerateArrayAssign(thead->tp, offset, ep->p[2], base);
				else if (ep->p[0])
					GenerateArrayAssign(thead->tp, offset, ep->p[0], base);
			}
			else
			*/
			{
				if (ep->p[2])
					GenerateStructAssign(thead->tp, offset, ep->p[2], base);
				else if (ep->p[0])
					GenerateStructAssign(thead->tp, offset, ep->p[0], base);
			}
/*
			else {
				ap1 = GenerateExpression(ep, am_reg, thead->tp->size);
				if (ap1->mode == am_imm) {
					ap2 = GetTempRegister();
					GenLdi(ap2, ap1);
				}
				else {
					ap2 = ap1;
					ap1 = nullptr;
				}
				if (base->offset)
					offset2 = base->offset->i + offset;
				else
					offset2 = offset;
				switch (thead->tp->size)
				{
				case 1:	GenerateDiadic(op_stb, 0, ap2, MakeIndexed(offset, base->preg)); break;
				case 2:	GenerateDiadic(op_stw, 0, ap2, MakeIndexed(offset, base->preg)); break;
				case 4:	GenerateDiadic(op_stp, 0, ap2, MakeIndexed(offset, base->preg)); break;
				case 512:	GenerateDiadic(op_sv, 0, ap2, MakeIndexed(offset, base->preg)); break;
				default:	GenerateDiadic(op_std, 0, ap2, MakeIndexed(offset, base->preg)); break;
				}
				if (ap2)
					ReleaseTempReg(ap2);
				if (ap1)
					ReleaseTempReg(ap1);
			}
*/
		}
		else {
			ap2 = nullptr;
			if (ep->p[2]==nullptr)
				break;
			ap1 = GenerateExpression(ep->p[2],am_reg,thead->tp->size,1);
			if (ap1->mode==am_imm) {
				ap2 = GetTempRegister();
				GenerateLoadConst(ap2, ap1);
			}
			else {
				ap2 = ap1;
				ap1 = nullptr;
			}
			if (base->offset)
				offset2 = base->offset->i + offset;
			else
				offset2 = offset;
			switch(thead->tp->size)
			{
			case 1:	GenerateDiadic(op_stb,0,ap2,MakeIndexed(offset,base->preg)); break;
			case 2:	GenerateDiadic(op_stw,0,ap2,MakeIndexed(offset,base->preg)); break;
			case 4:	GenerateDiadic(op_stt,0,ap2,MakeIndexed(offset,base->preg)); break;
			case 8:	GenerateDiadic(op_sto, 0, ap2, MakeIndexed(offset, base->preg)); break;
			case 64:	GenerateDiadic(op_sv,0,ap2,MakeIndexed(offset,base->preg)); break;
			default:	GenerateDiadic(op_sto,0,ap2,MakeIndexed(offset,base->preg)); break;
			}
			if (ap2)
				ReleaseTempReg(ap2);
			if (ap1)
				ReleaseTempReg(ap1);
		}
		if (!thead->tp->IsUnion())
			offset += thead->tp->size;
		thead = Symbol::GetPtr(thead->next);
		ep = ep->p[2];
	}
	if (!thead && ep)
		error(ERR_TOOMANYELEMENTS);
}


OCODE* CodeGenerator::GenerateLoadFloatConst(Operand* ap1, Operand* ap2)
{
	float f;
	double d;
	int32_t* pi;
	float* pf = &f;
	int32_t i;
	OCODE* ip = currentFn->pl.tail;

	pi = (int32_t*)pf;
	i = *pi;
	Float128 f128;
	uint16_t h;

	Float128::FloatQuadToHalf(&h, &ap1->offset->f128);
	Float128::Float128ToSingle(&f, &ap1->offset->f128);
	GenerateTriadic(op_orf, 0, ap2, makereg(regZero), MakeImmediate(h));
	if (ap1->offset->f128.IsHalf())
		;
	else if (ap1->offset->f128.IsSingle())
		GenerateMonadic(op_pfx0, 0, MakeImmediate(i));
	else {
		int64_t i;
		int64_t* pi;

		pi = (int64_t*)&d;
		i = *pi;
		if (ap1->offset->f128.IsDouble()) {
			Float128::Float128ToDouble(&d, &ap1->offset->f128);
			GenerateMonadic(op_pfx0, 0, MakeImmediate(0xffffffffLL & i));
			GenerateMonadic(op_pfx1, 0, MakeImmediate((i >> 32LL) & 0xffffffffLL));
		}
		else {
			Float128::Assign(&f128, &ap1->offset->f128);
			f128.Pack(128);
			GenerateMonadic(op_pfx0, 0, MakeImmediate(f128.pack[0]));
			GenerateMonadic(op_pfx1, 0, MakeImmediate(f128.pack[1]));
			GenerateMonadic(op_pfx2, 0, MakeImmediate(f128.pack[2]));
			GenerateMonadic(op_pfx2, 0, MakeImmediate(f128.pack[3]));
		}
	}
	return (ip->fwd);
}

// Generate an assignment to an array.
void CodeGenerator::GenerateLoadConst(Operand *aConst, Operand *ap2)
{
	Operand *ap3;

	if (aConst->isPtr) {
		ap3 = aConst->Clone();
		ap3->mode = am_direct;
		GenerateLoadAddress(ap2, ap3);
		//if (!compiler.os_code) {
		//	switch (aConst->segment) {
		//	case tlsseg:		GenerateTriadic(op_base, 0, ap2, ap2, MakeImmediate(8));	break;
		//	case rodataseg:	GenerateTriadic(op_base, 0, ap2, ap2, MakeImmediate(12));	break;
		//	}
		//}
	}
	else {
		OCODE* ip;
		if (aConst->offset == nullptr)
			;
//		if (aConst->offset->esize <= 8)
//			ip = GenerateDiadic(cpu.ldi_op, 0, ap2, MakeImmediate(aConst->offset->i));
//		else 
		 {
			// Try to compress a float into the smallest representation.
			if (aConst->tp->IsFloatType())
				ip = GenerateLoadFloatConst(aConst, ap2);
			else {
				ip = GenerateDiadic(op_loadi, 0, ap2, MakeImmediate(aConst->offset->i128.low & 0xffffLL));
				if (!aConst->offset->i128.IsNBit(16))
					GenerateMonadic(op_pfx0, 0, MakeImmediate(aConst->offset->i128.low & 0xfffffffffLL));
				if (!aConst->offset->i128.IsNBit(32))
					GenerateMonadic(op_pfx1, 0, MakeImmediate(aConst->offset->i128.low >> 32LL));
				if (!aConst->offset->i128.IsNBit(64))
					GenerateMonadic(op_pfx2, 0, MakeImmediate(aConst->offset->i128.high & 0xffffffffLL));
				if (!aConst->offset->i128.IsNBit(96))
					GenerateMonadic(op_pfx3, 0, MakeImmediate(aConst->offset->i128.high >> 32LL));
			}
		}
		if (ip->oper2)
			if (ip->oper2->offset)
				ip->oper2->offset->constflag = true;
		regs[ap2->preg].isConst = false;// true;
			if (ap2->tp) {
//				ap2->tp->type = bt_long;
//				ap2->tp->size = 16;
			}
	}
	// ap2 inherits type from aConst
//	ap2->tp = aConst->tp;
	regs[ap2->preg].offset = aConst->offset;
}

void CodeGenerator::GenerateArrayAssign(TYP *tp, ENODE *node1, ENODE *node2, Operand *base)
{
	ENODE *ep1;
	Operand *ap1, *ap2;
	int64_t size = tp->size;
	int64_t offset, offset2;

	offset = 0;
	if (node1->tp)
		tp = node1->tp->btpp;
	else
		tp = nullptr;
	if (tp==nullptr)
		tp = &stdlong;
	if (tp->IsStructType()) {
		ep1 = nullptr;
		ep1 = node2->p[0];
		while (ep1 && offset < size) {
			GenerateStructAssign(tp, offset, ep1->p[2], base);
			if (!tp->IsUnion())
				offset += tp->size;
			ep1 = ep1->p[2];
		}
	}
	else if (tp->IsAggregateType()){
		GenerateAggregateAssign(node1->p[0],node2->p[0]);
	}
	else {
		ep1 = node2->p[0];
		offset = 0;
		if (base->offset)
			offset = base->offset->i;
		ep1 = ep1->p[2];
		while (ep1) {
			ap1 = GenerateExpression(ep1,am_reg|am_imm,cpu.sizeOfWord,1);
			ap2 = GetTempRegister();
			if (ap1->mode == am_imm)
				GenerateLoadConst(ap1, ap2);
			else {
				if (ap1->offset)
					offset2 = ap1->offset->i;
				else
					offset2 = 0;
				GenerateMove(ap2,ap1);
			}
			switch(tp->GetElementSize())
			{
			case 1:	GenerateDiadic(op_stb,0,ap2,MakeIndexed(offset,base->preg)); break;
			case 2:	GenerateDiadic(op_stw,0,ap2,MakeIndexed(offset,base->preg)); break;
			case 4:	GenerateDiadic(op_stt,0,ap2,MakeIndexed(offset,base->preg)); break;
			case 8:	GenerateDiadic(op_sto, 0, ap2, MakeIndexed(offset, base->preg)); break;
			case 64:	GenerateDiadic(op_sv,0,ap2,MakeIndexed(offset,base->preg)); break;
			default:	GenerateDiadic(op_sto,0,ap2,MakeIndexed(offset,base->preg)); break;
			}
			offset += tp->GetElementSize();
			ReleaseTempReg(ap2);
			ReleaseTempReg(ap1);
			ep1 = ep1->p[2];
		}
	}
}

Operand *CodeGenerator::GenerateAggregateAssign(ENODE *node1, ENODE *node2)
{
	Operand *base, *base2;
	TYP *tp;
	int64_t offset = 0;

	if (node1==nullptr || node2==nullptr)
		return nullptr;
	//DumpStructEnodes(node2);
	base = GenerateExpression(node1,am_reg,cpu.sizeOfWord,0);
	base2 = GenerateExpression(node2, am_reg, cpu.sizeOfWord,1);
	GenerateMove(makereg(cpu.argregs[0]), base);
	GenerateMove(makereg(cpu.argregs[1]), base2);
	GenerateLoadConst(MakeImmediate(node2->esize), makereg(cpu.argregs[2]));
//	GenerateDiadic(op_loadi, 0, makereg(regFirstArg + 2), MakeImmediate(node1->esize));
#ifdef RISCV
		GenerateMonadic(op_call, 0, MakeStringAsNameConst((char *)"__aacpy", codeseg));
#endif
#ifdef THOR
	GenerateDiadic(op_jsr, 0, makereg(regLR), MakeStringAsNameConst((char *)"__aacpy",codeseg));
#endif
#ifdef QUPLS
	GenerateDiadic(op_jsr, 0, makereg(regLR), MakeStringAsNameConst((char*)"__aacpy", codeseg));
#endif
#ifdef LB650
	GenerateDiadic(op_jsr, 0, makereg(regLR), MakeStringAsNameConst((char*)"__aacpy", codeseg));
#endif
#ifdef STARK
	GenerateDiadic(op_jsr, 0, makereg(regLR), MakeStringAsNameConst((char*)"__aacpy", codeseg));
#endif
	ReleaseTempReg(base2);
	currentFn->IsLeaf = false;
	return (base);
	//base = GenerateDereference(node1,am_mem,cpu.sizeOfWord,0);
	tp = node1->tp;
	if (tp==nullptr)
		tp = &stdlong;
	if (tp->IsStructType()) {
		if (base->offset)
			offset = base->offset->i;
		else
			offset = 0;
		GenerateStructAssign(tp,offset,node2->p[0],base);
		//GenerateStructAssign(tp,offset2,node2->p[0]->p[0],base);
	}
	// Process Array
	else {
		GenerateArrayAssign(tp, node1, node2, base);
	}
	return base;
}

Operand* CodeGenerator::GenerateBigAssign(Operand* ap1, Operand* ap2, int64_t size, int64_t ssize)
{
	Operand* ap3;

	if (ap1->typep == &stdvector && (ap2->typep == &stdvector || ap2->mode == am_vreg)) {
		if (ap2->mode == am_vreg)
			GenerateStore(ap2, ap1, ssize);
		else {
			ap3 = GetTempVectorRegister();
			ap3->tp = ap2->tp;
			GenerateLoad(ap3, ap2, ssize, ssize);
			ap3->tp = ap1->tp;
			GenerateStore(ap3, ap1, ssize);
			ReleaseTempRegister(ap3);
		}
	}
	else {
		GenerateTriadic(op_sub, 0, makereg(regSP), makereg(regSP), MakeImmediate(3 * cpu.sizeOfWord));
		ap3 = GetTempRegister();
		ap3->tp = ap2->tp;
		GenerateDiadic(cpu.ldi_op, 0, ap3, MakeImmediate(size));
		GenerateDiadic(cpu.sto_op, 0, ap3, MakeIndexed(2 * cpu.sizeOfWord, regSP));
		if (ap2->mode != am_reg) {
			GenerateLoad(ap3, ap2, ssize, ssize);
			GenerateDiadic(cpu.sto_op, 0, ap3, MakeIndexed(1 * cpu.sizeOfWord, regSP));
		}
		else
			GenerateDiadic(cpu.sto_op, 0, ap2, MakeIndexed(1 * cpu.sizeOfWord, regSP));
		if (ap1->mode != am_reg) {
			GenerateLoad(ap3, ap1, ssize, ssize);
			GenerateDiadic(cpu.sto_op, 0, ap3, MakeIndirect(regSP));
		}
		else
			GenerateDiadic(cpu.sto_op, 0, ap1, MakeIndirect(regSP));
		GenerateCall(MakeStringAsNameConst((char*)"_aacpy", codeseg));
	}
	return (ap1);
}

Operand* CodeGenerator::GenerateImmToMemAssign(Operand* ap1, Operand* ap2, int64_t ssize)
{
	Operand* ap3;

	if (ap2->tp->IsFloatType()) {
		if (Float128::IsEqual(&ap2->offset->f128, Float128::Zero())) {
			GenerateStore(makereg(regZero), ap1, ssize);
			return (ap1);
		}
		ap3 = GetTempRegister();
		GenerateLoadFloatConst(ap2,ap3);
		GenerateStore(ap3, ap1, ssize);
		ReleaseTempRegister(ap3);
		return (ap1);
	}
	if (ap2->offset->i == 0 && ap2->offset->nodetype != en_labcon) {
		GenerateStore(makereg(regZero), ap1, ssize);
	}
	else {
		//if (ap2->offset->nodetype == en_icon && ap2->offset->i >= -32 && ap2->offset->i < 32) {
		//	GenerateStore(ap2, ap1, ssize);
		//}
		//else
		if (ap2->offset->nodetype == en_icon && ap2->offset->i == 0)
			GenerateStore(makereg(regZero), ap1, ssize);
		else
		{
			ap3 = GetTempRegister();
			GenerateLoadConst(ap2, ap3);
			GenerateStore(ap3, ap1, ssize);
			ReleaseTempReg(ap3);
		}
	}
	return (ap1);
}

Operand* CodeGenerator::GenerateRegToMemAssign(Operand* ap1, Operand* ap2, int64_t ssize)
{
	GenerateStore(ap2, ap1, ssize);
	return (ap1);
}

Operand* CodeGenerator::GenerateVregToMemAssign(Operand* ap1, Operand* ap2, int64_t ssize)
{
	GenerateStore(ap2, ap1, ssize);
	return (ap1);
}

Operand* CodeGenerator::GenerateRegToRegAssign(ENODE* node, Operand* ap1, Operand* ap2, int64_t ssize)
{
	Operand* ap3;

	GenerateHint(2);
	if (node->p[0]->IsRefType() && node->p[1]->IsRefType()) {
		ap3 = GetTempRegister();
		ap3->tp = ap2->tp;
		GenerateLoad(ap3, MakeIndirect(ap2->preg), ssize, node->p[1]->GetReferenceSize());
		GenerateStore(ap3, MakeIndirect(ap1->preg), ssize);
		ReleaseTempRegister(ap3);
	}
	else if (node->p[1]->IsRefType()) {
		ap3 = GetTempRegister();
		ap3->tp = ap2->tp;
		GenerateLoad(ap3, MakeIndirect(ap2->preg), ssize, node->p[1]->GetReferenceSize());
		GenerateMove(ap1, ap3);
		ReleaseTempRegister(ap3);
		//GenerateZeradic(op_setwb);
		ap1->isPtr = TRUE;
	}
	else if (node->p[0]->IsRefType()) {
		GenerateStore(ap2, MakeIndirect(ap1->preg), ssize);
	}
	else {
		GenerateMove(ap1, ap2);
	}
	if (ap1->preg < cpu.nregs)
		cpu.regs[ap1->preg].containsValue = true;
	return (ap1);
}

Operand* CodeGenerator::GenerateVregToVregAssign(ENODE* node, Operand* ap1, Operand* ap2, int64_t ssize)
{
	Operand* ap3, *mask;

	GenerateHint(2);
	mask = nullptr;
	if (node->mask)
		mask = GenerateExpression(node->vmask, am_reg, cpu.sizeOfWord, 0);
	if (node->p[0]->IsRefType() && node->p[1]->IsRefType()) {
		ap3 = GetTempVectorRegister();
		ap3->tp = ap2->tp;
		GenerateLoad(ap3, MakeIndirect(ap2->preg), ssize, node->p[1]->GetReferenceSize(), mask);
		GenerateStore(ap3, MakeIndirect(ap1->preg), ssize, mask);
		ReleaseTempRegister(ap3);
	}
	else if (node->p[1]->IsRefType()) {
		ap3 = GetTempVectorRegister();
		ap3->tp = ap2->tp;
		GenerateLoad(ap3, MakeIndirect(ap2->preg), ssize, node->p[1]->GetReferenceSize(), mask);
		GenerateMove(ap1, ap3, mask);
		ReleaseTempRegister(ap3);
		//GenerateZeradic(op_setwb);
		ap1->isPtr = TRUE;
	}
	else if (node->p[0]->IsRefType()) {
		GenerateStore(ap2, MakeIndirect(ap1->preg), ssize, mask);
	}
	else {
		GenerateMove(ap1, ap2, mask);
	}
	ReleaseTempRegister(mask);
	return (ap1);
}

Operand* CodeGenerator::GenerateImmToRegAssign(Operand* ap1, Operand* ap2, int64_t ssize)
{
	//if (ap2->isPtr)
//	GenerateZeradic(op_setwb);
	GenerateLoadConst(ap2, ap1);
	ap1->isPtr = ap2->isPtr;
	if (ap1->preg < cpu.nregs)
		cpu.regs[ap1->preg].containsValue = true;
	return (ap1);
}


Operand* CodeGenerator::GenerateMemToRegAssign(Operand* ap1, Operand* ap2, int64_t size, int64_t ssize)
{
	Operand* ap3;

	if (ap1->isPtr) {
		ap3 = GetTempRegister();
		ap3->tp = ap2->tp;
		GenerateLoad(ap3, ap2, ssize, size);
		ap3->tp = ap1->tp;
		GenerateStore(ap3, MakeIndirect(ap1->preg), ssize);
	}
	else {
		//if (ap1->preg >= 0x20 && ap1->preg <= 0x3f)
		//	ap1->mode = am_fpreg;
		GenerateLoad(ap1, ap2, ssize, size);
	}
	ap1->isPtr = ap2->isPtr;
	if (ap1->preg < cpu.nregs)
		cpu.regs[ap1->preg].containsValue = true;
	return (ap1);
}


// ----------------------------------------------------------------------------
// Generate code for an assignment node. If the size of the assignment
// destination is larger than the size passed then everything below this node
// will be evaluated with the assignment size.
// ----------------------------------------------------------------------------
Operand *CodeGenerator::GenerateAssign(ENODE *node, int flags, int64_t size)
{
	Operand *ap1, *ap2 ,*ap3;
	TYP *tp;
  int64_t ssize;
	int64_t RHsize;
	MachineReg *mr;
	int flg;

  Enter((char *)"<GenerateAssign>");

  if (node->p[0]->IsBitfield()) {
    Leave((char *)"</GenerateAssign>",0);
		return (node->GenerateBitfieldAssign(flags|am_bf_assign, size));
  }

	ssize = node->p[0]->GetReferenceSize();
//	if( ssize > size )
//			size = ssize;
/*
    if (node->tp->type==bt_struct || node->tp->type==bt_union) {
		ap1 = GenerateExpression(node->p[0],am_reg,ssize);
		ap2 = GenerateExpression(node->p[1],am_reg,size);
		GenerateMonadic(op_push,0,MakeImmediate(node->tp->size));
		GenerateMonadic(op_push,0,ap2);
		GenerateMonadic(op_push,0,ap1);
		GenerateMonadic(op_bsr,0,MakeStringAsNameConst("memcpy_"));
		GenerateTriadic(op_addui,0,makereg(regSP),makereg(regSP),MakeImmediate(24));
		ReleaseTempReg(ap2);
		return ap1;
    }
*/
	tp = node->p[0]->tp;
	if (tp) {
		if (tp->size > cpu.sizeOfWord) {
			if (node->p[0]->tp->IsAggregateType() || node->p[1]->nodetype == en_list || node->p[1]->nodetype == en_end_aggregate)
				return GenerateAggregateAssign(node->p[0], node->p[1]);
		}
	}
	//if (size > 8) {
	//	ap1 = GenerateExpression(node->p[0],am_mem,ssize);
	//	ap2 = GenerateExpression(node->p[1],am_mem,size);
	//}
	//else {
		ap1 = GenerateExpression(node->p[0], am_reg | am_mem | am_vreg, ssize, 1);
		flg = am_all;
		flg = am_reg | am_fpreg | am_mem | am_imm | am_vreg;
		/*
		if (ap1->typep == &stddouble)
			flg = am_fpreg;
		else 
		if (ap1->typep == &stdposit)
			flg = am_preg;
		*/

		// We want the size of the RHS to be its natural size.
		ap2 = GenerateExpression(node->p[1], flg, RHsize = node->p[1]->GetNaturalSize(), 0);// size);
		//if (node->p[0]->isUnsigned && !node->p[1]->isUnsigned)
		//    ap2->GenZeroExtend(RHsize,ssize);
		// Supposed to be handled in parse
		//if (RHsize != ssize)
		//	forcefit(&(node->p[1]), ap1->tp, &(node->p[0]), ap2->tp, true, true);
//	}
	if (ap1->mode == am_reg || ap1->mode==am_fpreg) {
		mr = &regs[ap1->preg];
		if (mr->assigned)
			mr->modified = true;
		mr->assigned = true;
		switch(ap2->mode) {

		case am_reg:
		case am_fpreg:
			ap1 = GenerateRegToRegAssign(node, ap1, ap2, ssize);
			mr->val = regs[ap2->preg].val;
			mr->val128 = regs[ap2->preg].val128;
			mr->isConst = ap2->isConst;
			break;

		case am_imm:
			ap1 = GenerateImmToRegAssign(ap1, ap2, ssize);
			if (ap2->tp->IsPositType()) {
				mr->val = ap2->offset->posit.val;
				mr->val128 = Int128::Convert(ap2->offset->posit.val);
			}
			else if (ap2->tp->IsFloatType()) {
				mr->val = CopyRawDouble(ap2->offset->f);
				mr->val128 = Int128::Convert(CopyRawDouble(ap2->offset->f));
			}
			else {
				mr->val = ap2->offset->i;
				mr->val128 = ap2->offset->i128;
			}
			mr->offset = ap2->offset;
			mr->isConst = false;	//true;
			break;

		default:
			ap1 = GenerateMemToRegAssign(ap1, ap2, node->p[1]->GetReferenceSize(), ssize);
			mr->modified = true;
			break;
		}
	}
	else if (ap1->mode == am_vreg) {
		mr = &vregs[ap1->preg & 0x1f];
		if (ap2->mode==am_vreg) {
			ap1 = GenerateVregToVregAssign(node, ap1, ap2, ssize);
			mr->val = vregs[ap2->preg & 0x1f].val;
			mr->val128 = vregs[ap2->preg & 0x1f].val128;
			mr->isConst = ap2->isConst;
		}
		else {
			ap1->tp = ap2->tp;
			GenerateLoad(ap1, ap2, ssize, size);
		}
	}
	// ap1 is memory
	else {
		if (ap2->mode == am_reg || ap2->mode==am_fpreg) {
			ap1 = GenerateRegToMemAssign(ap1, ap2, ssize);
    }
		else if (ap2->mode == am_vreg) {
			ap1 = GenerateVregToMemAssign(ap1, ap2, ssize);
		}
		else if (ap2->mode == am_imm) {
			ap1 = GenerateImmToMemAssign(ap1, ap2, ssize);
		}
		else {
			// Generate a memory to memory move? (struct assignments)
			if (ssize > cpu.sizeOfWord)
				ap1 = GenerateBigAssign(ap1, ap2, size, ssize);
			else {
				ap3 = GetTempRegister();
				ap3->tp = ap1->tp;
				ap3->isPtr = ap2->isPtr;
				GenerateLoad(ap3, ap2, ssize, size);
//				GenLoad(ap3,ap2, node->p[0]->GetReferenceSize(),node->p[1]->GetReferenceSize());
				//if (ap1->isPtr) {
				//	ap4 = GetTempRegister();
				//	GenerateLoad(ap4, ap1, ssize, size);
				//	GenerateStore(ap3, ap4, ssize);
				//	ReleaseTempRegister(ap4);
				//}
				//else
					GenerateStore(ap3,ap1,ssize);
				ReleaseTempRegister(ap3);
			}
		}
	}
/*
	if (ap1->mode == am_reg) {
		if (ap2->mode==am_imm)	// must be zero
			GenerateDiadic(op_mov,0,ap1,makereg(0));
		else
			GenerateDiadic(op_mov,0,ap1,ap2);
	}
	else {
		if (ap2->mode==am_imm)
		switch(size) {
		case 1:	GenerateDiadic(op_stb,0,makereg(0),ap1); break;
		case 2:	GenerateDiadic(op_stw,0,makereg(0),ap1); break;
		case 4: GenerateDiadic(op_stp,0,makereg(0),ap1); break;
		case 8:	GenerateDiadic(op_std,0,makereg(0),ap1); break;
		}
		else
		switch(size) {
		case 1:	GenerateDiadic(op_stb,0,ap2,ap1); break;
		case 2:	GenerateDiadic(op_stw,0,ap2,ap1); break;
		case 4: GenerateDiadic(op_stp,0,ap2,ap1); break;
		case 8:	GenerateDiadic(op_std,0,ap2,ap1); break;
		// Do structure assignment
		default: {
			ap3 = GetTempRegister();
			GenerateDiadic(op_loadi,0,ap3,MakeImmediate(size));
			GenerateTriadic(op_push,0,ap3,ap2,ap1);
			GenerateDiadic(op_jal,0,makereg(LR),MakeStringAsNameConst("memcpy"));
			GenerateTriadic(op_addui,0,makereg(SP),makereg(SP),MakeImmediate(24));
			ReleaseTempRegister(ap3);
		}
		}
	}
*/
	ReleaseTempReg(ap2);
  ap1->MakeLegal(flags,size);
  Leave((char *)"</GenerateAssign>",1);
	return (ap1);
}

// autocon and autofcon nodes

Operand *CodeGenerator::GenerateAutocon(ENODE *node, int flags, int64_t size, TYP* typ)
{
	Operand *ap1, *ap2, *ap3;
	short nn, ni;

	// We always want an address register (GPR) for lea
	ap1 = GetTempRegister();
	ap2 = allocOperand();
	ap2->isPtr = node->etype == bt_pointer;
	ap2->mode = am_indx;
	ap2->preg = regFP;          // frame pointer
	ap2->offset = node;     /* use as constant node */
	ap2->bit_offset = node->bit_offset;
	ap2->bit_width = node->bit_width;
	/* offset->i is negative here. Why are data labels being updated? 
	* Autocon nodes should be SP relative.... I think its a copu error.
	*/
	//DataLabels[ap2->offset->i]++;
//	ap2->type = type;
	ap2->tp = node->tp;
//	ap1->type = &stdint;
	ap1->typep = typ;
	ap1->tp = node->tp;
	ni = nn = node->sym->depth - currentFn->depth;
	if (nn > 0) {
		ap3 = GetTempRegister();
		ap3->tp = ap1->tp;
		if (nn == 1)
			GenerateLoad(ap3, MakeIndirect(regFP), size, size);
		else
			GenerateLoad(ap3, MakeIndirect(regFP), cpu.sizeOfWord, cpu.sizeOfWord);	
		for (--nn; nn > 0; nn--) {
			if (nn == 1)
				GenerateLoad(ap3, MakeIndirect(ap3->preg), size, size);
			else
				GenerateLoad(ap3, MakeIndirect(ap3->preg), cpu.sizeOfWord, cpu.sizeOfWord);
		}
		ReleaseTempRegister(ap3);
		ap3->isPtr = node->etype == bt_pointer;
		ap3->mode = am_indx;
		//ap3->preg = regFP;          // frame pointer
		ap3->offset = node;     /* use as constant node */
		ap3->bit_offset = node->bit_offset;
		ap3->bit_width = node->bit_width;
		//	ap2->type = type;
		ap3->tp = node->tp;
		GenerateLoadAddress(ap1, ap3);
	}
	else
		GenerateLoadAddress(ap1, ap2);
	//if (!compiler.os_code)
	//	GenerateTriadic(op_base, 0, ap1, ap1, MakeImmediate(10));
	ap1->MakeLegal(flags,size);
	return (ap1);             /* return reg */
}

Operand* CodeGenerator::GenerateFloatcon(ENODE* node, int flags, int64_t size)
{
	Operand* ap1;

#ifdef STARK
	ap1 = allocOperand();
	ap1->mode = am_imm;
	ap1->offset = node;
	ap1->tp = node->tp;
	return (ap1);
#endif
#ifdef LB650
	ap1 = allocOperand();
	ap1->mode = am_imm;
	ap1->offset = node;
	ap1->tp = node->tp;
	return (ap1);
#endif
#ifdef QUPLS
	ap1 = allocOperand();
	ap1->mode = am_imm;
	ap1->offset = node;
	ap1->tp = node->tp;
	return (ap1);
#endif
#ifdef QUPLS40
	ap1 = allocOperand();
	ap1->mode = am_imm;
	ap1->offset = node;
	ap1->tp = node->tp;
	return (ap1);
#endif
#ifdef THOR
	ap1 = allocOperand();
	ap1->mode = am_imm;
	ap1->offset = node;
	ap1->tp = node->tp;
	return (ap1);
#endif
#ifdef RISCV
	// Code for generating a reference to the constant which is 
	// stored in rodata. This is not needed by Thor since Thor can use immediates
	// with floats, but some other architectures cannot.
	ap1 = allocOperand();
	ap1->isPtr = node->IsPtr();
	if (node->constflag && node->f128.IsZero()) {
		ap1->mode = am_reg;
		ap1->isConst = true;
		ap1->preg = regZero;
	}
	else {
		if (use_gp) {
			ap1->mode = am_indx;
			switch (node->segment) {
			case bssseg: ap1->preg = regGP; break;
			case rodataseg: ap1->preg = regGP1; break;
			case dataseg: ap1->preg = regGP; break;
			default:	ap1->preg = regGP;
						}
		}
		else
			ap1->mode = am_direct;
		ap1->offset = node;
		if (node)
			DataLabels[node->i]++;
	}
	ap1->typep = &stddouble;
	if (node)
		ap1->tp = node->tp;
	// Don't allow the constant to be loaded into an integer register.
	ap1->MakeLegal(flags, size);
	return (ap1);
#endif
#ifdef BIGFOOT
	ap1 = allocOperand();
	ap1->mode = am_imm;
	ap1->offset = node;
	ap1->tp = node->tp;
	return (ap1);
#endif
#ifdef I386
	ap1 = allocOperand();
	ap1->mode = am_imm;
	ap1->offset = node;
	ap1->tp = node->tp;
	return (ap1);
#endif
}

Operand* CodeGenerator::GenPositcon(ENODE* node, int flags, int64_t size)
{
	Operand* ap1;

	ap1 = allocOperand();
	ap1->isPtr = node->IsPtr();
	if (use_gp) {
		ap1->mode = am_indx;
		ap1->preg = GetSegmentIndexReg((e_sg)node->segment);
	}
	else
		ap1->mode = am_direct;
	ap1->offset = node;
	if (node)
		DataLabels[node->i]++;
	ap1->typep = &stdposit;
	if (node)
		ap1->tp = node->tp;
	// Don't allow the constant to be loaded into an integer register.
	ap1->MakeLegal(flags & ~am_reg, size);
	return (ap1);
}

Operand* CodeGenerator::GenLabelcon(ENODE* node, int flags, int64_t size)
{
	Operand* ap1, * ap2;

	if (use_gp) {
		ap1 = GetTempRegister();
		ap2 = allocOperand();
		ap2->mode = am_indx;
		ap2->preg = GetSegmentIndexReg((e_sg)node->segment);
		ap2->offset = node;     // use as constant node
		GenerateLoadAddress(ap1, ap2);
		//if (!compiler.os_code) {
		//	switch (node->segment) {
		//	case tlsseg:		GenerateTriadic(op_base, 0, ap1, ap1, MakeImmediate(8));	break;
		//	case rodataseg:	GenerateTriadic(op_base, 0, ap1, ap1, MakeImmediate(12));	break;
		//	}
		//}
		ap1->MakeLegal(flags, size);
		return (ap1);
	}
	ap1 = allocOperand();
	ap1->isPtr = node->IsPtr();
	ap1->mode = am_imm;
	ap1->offset = node;
	ap1->isUnsigned = node->isUnsigned;
	ap1->tp = node->tp;
	ap1->MakeLegal(flags, size);
	return (ap1);
}

Operand* CodeGenerator::GenNacon(ENODE* node, int flags, int64_t size)
{
	Operand* ap1, * ap2;

	if (use_gp) {
		ap1 = GetTempRegister();
		ap2 = allocOperand();
		ap2->mode = am_indx;
		ap2->preg = node->segment == tlsseg ? regTP : regGP;
		ap2->offset = node;     // use as constant node
		if (node)
			DataLabels[node->i]++;
		GenerateLoadAddress(ap1, ap2);
		ap1->MakeLegal(flags, size);
		Leave((char*)"GenNacon", 6);
		return (ap1);
	}
	ap1 = allocOperand();
	ap1->isPtr = node->IsPtr();
	ap1->mode = am_imm;
	ap1->offset = node;
	if (node->i == 0)
		node->i = -1;
	ap1->isUnsigned = node->isUnsigned;
	ap1->MakeLegal(flags, size);
	Leave((char*)"GenNacon", 7);
	return (ap1);
}

//
// General expression evaluation. returns the addressing mode
// of the result.
//
Operand *CodeGenerator::GenerateExpression(ENODE *node, int flags, int64_t size, int rhs)
{   
	Operand *ap1, *ap2, *ap3;
  int64_t natsize;
	static char buf[4][20];
	static int ndx;
	static int numDiags = 0;
	OCODE* ip;
	int ndxreg;
	size_t tpsz;
	Symbol* sym;
	char nmbuf[200];

  Enter((char *)"<GenerateExpression>"); 

	compiler.expr_depth++;
	if (compiler.expr_depth > 150) {
		error(ERR_EXPRTOOCOMPLEX);
		throw new ArplException(ERR_EXPRTOOCOMPLEX, 100);
	}
	if( node == (ENODE *)NULL )
  {
		throw new ArplException(ERR_NULLPOINTER, 'G');
		numDiags++;
        printf("DIAG - null node in GenerateExpression.\n");
		if (numDiags > 100)
			exit(0);
        Leave((char *)"</GenerateExpression>",2); 
				compiler.expr_depth--;
        return (Operand *)NULL;
    }
	//size = node->esize;
  switch( node->nodetype )
  {
	case en_aggregate:
	case en_end_aggregate:
		if (pass == 1) {
			char* pns;
			sym = Symbol::alloc();
			pns = GetPrivateNamespace();
			//sprintf_s(nmbuf, sizeof(nmbuf), "__aggregate_%s", sym->acnt, pns);
			sym->tp = node->tp;
			if (node->constflag)
				sym->storage_class = sc_const;
			else
				sym->storage_class = sc_global;
			node->AssignTypeToList(sym->tp);
			ofs.puts("\n");
			sprintf_s(nmbuf, sizeof(nmbuf), "__aggregate_%s_%d", pns, sym->acnt);
//			put_label(ofs, sym->acnt, nmbuf, GetNamespace(), 'D', sym->tp->size, sym->segment);
			put_label(ofs, -1, nullptr, nmbuf, 'D', sym->tp->size, sym->segment);
			sym->SetName(std::string((char*)nmbuf));
			sym->Initialize(ofs, node, sym->tp, 1);
			ofs.puts("\n\n");
			node->sp = sym->name;
		}
//		GenerateReference(sym, 0);
		ap1 = GetTempRegister();
		GenerateLoadAddress(ap1, 
			MakeStringAsNameConst((char *)node->sp->c_str(),
//			MakeStringAsNameConst(nmbuf,
					use_iprel ? codeseg : rodataseg));
		ap1->isPtr = true;
		sym->acnt++;
		/*
		ap2 = allocOperand();
		ap2->mode = am_reg;
		ap2->preg = ap1->preg;
		if (node->tp->IsScalar())
			GenerateLoad(ap1, ap2, size, size);
		else
			ap1->isPtr = true;
		*/
		goto retpt;
		//ap1 = allocOperand();
		//ap1->offset = node;
		//ap1->type = 9999;
		goto retpt;
	
	case en_fcon:
		ap1 = GenerateFloatcon(node, flags, size);
		goto retpt;
/*
	case en_pcon:
		ap1 = GenPositcon(node, flags, size);
		goto retpt;
*/
	case en_pcon:
		ap1 = allocOperand();
		ap1->mode = am_imm;
		ap1->offset = node;
		ap1->offset->i = ap1->offset->posit.val;
		ap1->tp = node->tp;
		Leave((char *)"GenExpression", 3);
		goto retpt;
	case en_icon:
    ap1 = allocOperand();
    ap1->mode = am_imm;
    ap1->offset = node;
		ap1->tp = node->tp;
		ap1->isConst = true;
		Leave((char *)"GenExpression",3);
		goto retpt;

	case en_pfx0:
		ap1 = allocOperand();
		ap1->mode = am_imm;
		ap1->offset = node;
		ap1->tp = node->tp;
		GenerateMonadic(op_pfx0, 0, ap1);
		goto retpt2;
	case en_pfx1:
		ap1 = allocOperand();
		ap1->mode = am_imm;
		ap1->offset = node;
		ap1->tp = node->tp;
		GenerateMonadic(op_pfx1, 0, ap1);
		goto retpt2;
	case en_pfx2:
		ap1 = allocOperand();
		ap1->mode = am_imm;
		ap1->offset = node;
		ap1->tp = node->tp;
		GenerateMonadic(op_pfx2, 0, ap1);
		goto retpt2;
	case en_pfx3:
		ap1 = allocOperand();
		ap1->mode = am_imm;
		ap1->offset = node;
		ap1->tp = node->tp;
		GenerateMonadic(op_pfx3, 0, ap1);
		goto retpt2;

	case en_labcon:
		ap1 = GenLabelcon(node, flags, size);
		goto retpt;

  case en_nacon:
		ap1 = GenNacon(node, flags, size);
		goto retpt;

	case en_cnacon:
      ap1 = allocOperand();
			ap1->isPtr = node->IsPtr();
			ap1->mode = am_imm;
      ap1->offset = node;
			if (node->i==0)
				node->i = -1;
			ap1->isUnsigned = node->isUnsigned;
            ap1->MakeLegal(flags,size);
			Leave((char *)"GenExpression",7); 
			goto retpt;
	case en_clabcon:
    ap1 = allocOperand();
    ap1->mode = am_imm;
    ap1->offset = node;
		ap1->isUnsigned = node->isUnsigned;
    ap1->MakeLegal(flags,size);
		Leave((char *)"GenExpression",7); 
		goto retpt;
	case en_autocon:
		ap1 = GenerateAutocon(node, flags, size, &stdint);
		goto retpt;
  case en_autofcon:	
		switch (node->tp->type)
		{
		case bt_float:
			ap1 = GenerateAutocon(node, flags, size, &stdflt);
			goto retpt;
		case bt_double:
			ap1 = GenerateAutocon(node, flags, size, &stddouble);
			goto retpt;
		case bt_quad:	ap1 = GenerateAutocon(node, flags, size, &stdquad); goto retpt;
		case bt_posit: ap1 = GenerateAutocon(node, flags, size, &stdposit); goto retpt;
		case bt_pointer:
			ap1 = GenerateAutocon(node, flags, size, &stdint);
			goto retpt;
		}
		break;

	case en_autopcon:
		switch (node->tp->type)
		{
		case bt_float:
			ap1 = GenerateAutocon(node, flags, size, &stdflt);
			goto retpt;
		case bt_double:
			ap1 = GenerateAutocon(node, flags, size, &stddouble);
			goto retpt;
		case bt_quad:	ap1 = GenerateAutocon(node, flags, size, &stdquad); goto retpt;
		case bt_posit: ap1 = GenerateAutocon(node, flags, size, &stdposit); goto retpt;
		case bt_pointer:
			ap1 = GenerateAutocon(node, flags, size, &stdint);
			goto retpt;
		}
		break;

	case en_autovcon:	ap1 = GenerateAutocon(node, flags, size, &stdvector); goto retpt;
  case en_autovmcon:	ap1 = GenerateAutocon(node, flags, size, &stdvectormask); goto retpt;
  case en_classcon:
    ap1 = GetTempRegister();
    ap2 = allocOperand();
    ap2->mode = am_indx;
    ap2->preg = regCLP;     /* frame pointer */
    ap2->offset = node;     /* use as constant node */
		GenerateLea(ap1, ap2);
		goto retpt;
	case en_addrof:
		ap1 = GetTempRegister();
		ap2 = GenerateExpression(node->p[0], flags, cpu.sizeOfPtr, rhs);
		GenerateLea(ap1, ap2);
		ReleaseTempReg(ap2);
		goto retpt;

  case en_ref:
		if (node->tp == nullptr)
			tpsz = cpu.sizeOfWord;
		else
			tpsz = node->tp->size;
		ap1 = GenerateDereference(node, flags, size, !node->isUnsigned, (flags & am_bf_assign) ? 0 : 1, rhs);
		if (ap1 == nullptr)
			goto retpt;
		ap1->isPtr = TRUE;
		ap1->rhs = rhs;
		size = ap1->tp->size;
		goto retpt;

	case en_fieldref:
		ap1 = (flags & am_bf_assign) ? GenerateDereference(node,flags & ~am_bf_assign,node->tp->size,!node->isUnsigned, (flags & am_bf_assign) != 0, rhs)
			: GenerateBitfieldDereference(node, flags, node->tp->size, (flags & am_bf_assign) != 0);//!node->isUnsigned);
		goto retpt;
	case en_regvar:
    ap1 = allocOperand();
		ap1->isPtr = node->IsPtr();
		ap1->tp = node->tp;
    ap1->mode = am_reg;
    ap1->preg = node->rg;
    ap1->tempflag = 0;      /* not a temporary */
    ap1->MakeLegal(flags,size);
		ap1->tp = node->tp;
		goto retpt;

	case en_type:
		if (node->tp)
			tpsz = node->tp->size;
		else
			tpsz = cpu.sizeOfWord;
		ap1 = GenerateDereference(node, flags & ~am_bf_assign, tpsz, !node->isUnsigned, (flags & am_bf_assign) != 0, rhs);
		ap1->isPtr = true;
		goto retpt;

	// ToDo: dereference tempref nodes
	case en_tempfpref:
		ap1 = allocOperand();
		ap1->isPtr = node->IsPtr();
		ap1->mode = node->IsPtr() ? am_reg : am_reg;
		ap1->preg = node->rg;
		ap1->tempflag = 0;      /* not a temporary */
		if (node->tp)
			switch (node->tp->type) {
			case bt_float:	ap1->typep = &stdflt; break;
			case bt_double:	ap1->typep = &stddouble; break;
			case bt_quad:	ap1->typep = &stdquad; break;
			case bt_posit: ap1->typep = &stdposit; break;
			default: ap1->typep = &stdint; break;
			}
		else
			ap1->typep = &stddouble;
		ap1->tp = node->tp;
		goto retpt;

	case en_temppref:
		ap1 = allocOperand();
		ap1->isPtr = node->IsPtr();
		ap1->mode = node->IsPtr() ? am_reg : am_preg;
		ap1->preg = node->rg;
		ap1->tempflag = 0;      /* not a temporary */
		if (node->tp)
			switch (node->tp->type) {
			case bt_posit: ap1->typep = &stdposit; break;
			default: ap1->typep = &stdint; break;
			}
		else
			ap1->typep = &stdposit;
		ap1->tp = node->tp;
		goto retpt;

	case en_fpregvar:
//    case en_fptempref:
    ap1 = allocOperand();
		ap1->isPtr = node->IsPtr();
		ap1->mode = node->IsPtr() ? am_reg : am_reg;
    ap1->preg = node->rg;
		ap1->tempflag = 0;      /* not a temporary */
		if (node->tp)
			switch (node->tp->type) {
			case bt_float:	ap1->typep = &stdflt; break;
			case bt_double:	ap1->typep = &stddouble; break;
			case bt_quad:	ap1->typep = &stdquad; break;
			case bt_posit: ap1->typep = &stdposit; break;
			default: ap1->typep = &stdint; break;
			}
		else
			ap1->typep = &stddouble;
		ap1->MakeLegal(flags,size);
		goto retpt;

	case en_pregvar:
		//    case en_fptempref:
		ap1 = allocOperand();
		ap1->isPtr = node->IsPtr();
		ap1->mode = node->IsPtr() ? am_reg : am_preg;
		ap1->preg = node->rg;
		ap1->tempflag = 0;      /* not a temporary */
		if (node->tp)
			switch (node->tp->type) {
			case bt_posit: ap1->typep = &stdposit; break;
			default: ap1->typep = &stdint; break;
			}
		else
			ap1->typep = &stdposit;
		ap1->MakeLegal(flags, size);
		goto retpt;

	case en_abs:	return node->GenerateUnary(flags,size,op_abs);
    case en_uminus: 
			ap1 = node->GenerateUnary(flags, size, op_neg);
			goto retpt;
    case en_compl:
			ap1 = node->GenerateUnary(flags,size,op_com);
			goto retpt;
	case en_not:	
		ap1 = (node->GenerateUnary(flags, cpu.sizeOfWord, op_not));
		goto retpt;
	case en_i2p:
		ap1 = GetTempFPRegister();
		ap2 = GenerateExpression(node->p[0], am_reg, 8, rhs);
		GenerateDiadic(op_itop, 0, ap1, ap2);
		ReleaseTempReg(ap2);
		goto retpt;
	case en_i2d:
    ap1 = GetTempFPRegister();	
    ap2=GenerateExpression(node->p[0],am_reg,8,rhs);
    GenerateDiadic(op_itof,' ',ap1,ap2);
    ReleaseTempReg(ap2);
		goto retpt;
  case en_i2q:
    ap1 = GetTempFPRegister();	
    ap2 = GenerateExpression(node->p[0],am_reg,8,rhs);
		//GenerateTriadic(op_csrrw,0,makereg(regZero),MakeImmediate(0x18),ap2);
		//GenerateZeradic(op_nop);
		//GenerateZeradic(op_nop);
    GenerateDiadic(op_itof,'q',ap1,makereg(63));
    ReleaseTempReg(ap2);
		goto retpt;
  case en_d2i:
    ap1 = GetTempRegister();	
    ap2 = GenerateExpression(node->p[0],am_reg,cpu.sizeOfWord,rhs);
    GenerateDiadic(op_ftoi,' ',ap1,ap2);
    ReleaseTempReg(ap2);
		goto retpt;
	case en_q2i:
		ap1 = GetTempRegister();
		ap2 = GenerateExpression(node->p[0],am_reg,cpu.sizeOfWord,rhs);
		GenerateDiadic(op_ftoi,'q',makereg(63),ap2);
		//GenerateZeradic(op_nop);
		//GenerateZeradic(op_nop);
		//GenerateTriadic(op_csrrw,0,ap1,MakeImmediate(0x18),makereg(0));
		ReleaseTempReg(ap2);
		goto retpt;
	case en_t2i:
		ap1 = GetTempRegister();
		ap2 = GenerateExpression(node->p[0],am_reg,cpu.sizeOfWord,rhs);
		GenerateDiadic(op_ftoi,'t',makereg(63),ap2);
		//GenerateZeradic(op_nop);
		//GenerateZeradic(op_nop);
		//GenerateTriadic(op_csrrw,0,ap1,MakeImmediate(0x18),makereg(0));
    ReleaseTempReg(ap2);
		goto retpt;
	case en_s2q:
		ap1 = GetTempFPRegister();
    ap2 = GenerateExpression(node->p[0],am_reg,cpu.sizeOfFPQ,rhs);
    GenerateDiadic(op_fcvtsq,0,ap1,ap2);
		ap1->typep = &stdquad;
		ReleaseTempReg(ap2);
		goto retpt;
	case en_d2q:
		ap1 = GetTempFPRegister();
		ap2 = GenerateExpression(node->p[0], am_reg, cpu.sizeOfFPQ,rhs);
		GenerateFcvtdq(ap1, ap2);
		ap1->typep = &stdquad;
		ReleaseTempReg(ap2);
		goto retpt;
	case en_t2q:
		ap1 = GetTempFPRegister();
		ap2 = GenerateExpression(node->p[0], am_reg, cpu.sizeOfFPQ,rhs);
		GenerateDiadic(op_fcvttq, 0, ap1, ap2);
		ap1->typep = &stdquad;
		ReleaseTempReg(ap2);
		goto retpt;
	case en_sync:
		ap1 = GenerateExpression(node->p[0], am_imm, cpu.sizeOfWord, rhs);
		GenerateMonadic(op_sync, 0, ap1);
		goto retpt;

	case en_vadd:	  ap1 = GenerateBinary(node, flags, size, node->tp->IsFloatType() ? op_fadd : op_add); goto retpt;
	case en_vsub:	  ap1 = node->GenerateBinary(flags,size,op_vsub); goto retpt;
	case en_vmul:	  ap1 = node->GenerateBinary(flags,size,op_vmul); goto retpt;
	case en_vadds:	  ap1 = node->GenerateBinary(flags,size,op_vadds); goto retpt;
	case en_vsubs:	  ap1 = node->GenerateBinary(flags,size,op_vsubs); goto retpt;
	case en_vmuls:	 ap1 = node->GenerateBinary(flags,size,op_vmuls); goto retpt;
	case en_vex:      ap1 = node->GenerateBinary(flags,size,op_vex); goto retpt;
	case en_veins:    ap1 = node->GenerateBinary(flags,size,op_veins); goto retpt;

	case en_fadd:	  ap1 = node->GenerateBinary(flags, size, op_fadd); goto retpt;
	case en_fsub:	  ap1 = node->GenerateBinary(flags, size, op_fsub); goto retpt;
	case en_fmul:	  ap1 = node->GenerateBinary(flags, size, op_fmul); goto retpt;
	case en_fdiv:	  ap1 = node->GenerateBinary(flags, size, op_fdiv); goto retpt;

	case en_padd:	  ap1 = node->GenerateBinary(flags, size, op_padd); goto retpt;
	case en_psub:	  ap1 = node->GenerateBinary(flags, size, op_psub); goto retpt;
	case en_pmul:	  ap1 = node->GenerateBinary(flags, size, op_pmul); goto retpt;
	case en_pdiv:	  ap1 = node->GenerateBinary(flags, size, op_pdiv); goto retpt;

	case en_fdadd:    ap1 = node->GenerateBinary(flags, size, op_fdadd); goto retpt;
  case en_fdsub:    ap1 = node->GenerateBinary(flags,size,op_fdsub); goto retpt;
  case en_fsadd:    ap1 = node->GenerateBinary(flags,size,op_fsadd); goto retpt;
  case en_fssub:    ap1 = node->GenerateBinary(flags,size,op_fssub); goto retpt;
  case en_fdmul:    ap1 = node->GenerateMultiply(flags,size,op_fmul); goto retpt;
  case en_fsmul:    ap1 = node->GenerateMultiply(flags,size,op_fmul); goto retpt;
  case en_fddiv:    ap1 = node->GenerateMultiply(flags,size,op_fddiv); goto retpt;
  case en_fsdiv:    ap1 = node->GenerateMultiply(flags,size,op_fsdiv); goto retpt;
	case en_ftadd:    ap1 = node->GenerateBinary(flags,size,op_ftadd); goto retpt;
  case en_ftsub:    ap1 = node->GenerateBinary(flags,size,op_ftsub); goto retpt;
  case en_ftmul:    ap1 = node->GenerateMultiply(flags,size,op_ftmul); goto retpt;
  case en_ftdiv:    ap1 = node->GenerateMultiply(flags,size,op_ftdiv); goto retpt;

	case en_land:
		/*
		lab0 = nextlabel++;
		lab1 = nextlabel++;
		GenerateFalseJump(node, lab0, 0);
		ap1 = GetTempRegister();
		GenerateDiadic(op_ld, 0, ap1, MakeImmediate(1));
		GenerateMonadic(op_branch, 0, MakeDataLabel(lab1));
		GenerateLabel(lab0);
		GenerateDiadic(op_ld, 0, ap1, MakeImmediate(0));
		GenerateLabel(lab1);
		return (ap1);
		*/
		ap1 = (node->GenerateLand(flags, op_and, !ExpressionHasReference));
		goto retpt;
	case en_lor:
		ap1 = (node->GenerateLand(flags, op_or, !ExpressionHasReference));
		goto retpt;
	case en_land_safe:
		ap1 = (node->GenerateLand(flags, op_and, true));
		goto retpt;
	case en_lor_safe:
		ap1 = (node->GenerateLand(flags, op_or, true));
		goto retpt;

	case en_isnullptr:	ap1 = node->GenerateUnary(flags, size, op_isnullptr); goto retpt;
	case en_add:    ap1 = GenerateBinary(node, flags, size, op_add); goto retpt;
	case en_sub:  ap1 = GenerateBinary(node, flags, size, op_sub); goto retpt;
	case en_ptrdif:  ap1 = GenerateBinary(node, flags, size, op_ptrdif); goto retpt;
	case en_and:    ap1 = GenerateBinary(node, flags, size, op_and); goto retpt;
  case en_or:     ap1 = GenerateBinary(node,flags,size,op_or); goto retpt;
	case en_or_and:     ap1 = GenerateTrinary(node, flags, size, op_or_and); goto retpt;
	case en_and_or:     ap1 = GenerateTrinary(node, flags, size, op_and_or); goto retpt;
	case en_or_or:     ap1 = GenerateTrinary(node, flags, size, op_or_or); goto retpt;
	case en_and_and:     ap1 = GenerateTrinary(node, flags, size, op_and_and); goto retpt;
	case en_xor:	ap1 = GenerateBinary(node, flags,size,op_xor); goto retpt;
	case en_bmap:	ap1 = node->GenerateBinary(flags, size, op_bmap); goto retpt;
	case en_bytendx:	ap1 = node->GenerateBinary(flags, size, op_bytendx); goto retpt;
	case en_wydendx:	ap1 = node->GenerateBinary(flags, size, op_wydendx); goto retpt;
	case en_ext:			ap1 = GenerateTrinary(node, flags, size, op_ext); goto retpt;
	case en_extu:			ap1 = GenerateTrinary(node, flags, size, op_extu); goto retpt;
	case en_mulf:    ap1 = node->GenerateMultiply(flags, size, op_mulf); goto retpt;
	case en_mul:    ap1 = node->GenerateMultiply(flags,size,op_mul); goto retpt;
  case en_mulu:   ap1 = node->GenerateMultiply(flags,size,op_mulu); goto retpt;
	case en_scndx:	ap1 = node->GenerateScaledIndexing(flags, size, rhs); goto retpt;
  case en_div:    ap1 = node->GenerateDivMod(flags,size,op_div); goto retpt;
  case en_udiv:   ap1 = node->GenerateDivMod(flags,size,op_divu); goto retpt;
  case en_mod:    ap1 = node->GenerateDivMod(flags,size,op_rem); goto retpt;
  case en_umod:   ap1 = node->GenerateDivMod(flags,size,op_remu); goto retpt;
  case en_asl:    ap1 = node->GenerateShift(flags,size,op_sll); goto retpt;
  case en_shl:    ap1 = GenerateShift(node,flags,size,op_sll); goto retpt;
  case en_shlu:   ap1 = GenerateShift(node,flags,size,op_sll); goto retpt;
  case en_asr:	ap1 = GenerateShift(node,flags,size,op_sra); goto retpt;
  case en_shr:	ap1 = GenerateShift(node,flags,size,op_sra); goto retpt;
  case en_shru:   ap1 = GenerateShift(node,flags,size,op_srl); goto retpt;
	case en_rol:   ap1 = GenerateShift(node,flags,size,op_rol); goto retpt;
	case en_ror:   ap1 = GenerateShift(node,flags,size,op_ror); goto retpt;
	case en_bitoffset:
		ap1 = GetTempRegister();
		ip = currentFn->pl.tail;
		ap2 = GenerateExpression(node->p[1], am_reg | am_imm | am_imm0, cpu.sizeOfWord, rhs);
		ap3 = GenerateExpression(node->p[2], am_reg | am_imm | am_imm0, cpu.sizeOfWord, rhs);
		if (ap2->mode != ap3->mode) {
			ReleaseTempReg(ap3);
			ReleaseTempReg(ap2);
			currentFn->pl.tail = ip;
			ap2 = GenerateExpression(node->p[1], am_reg, cpu.sizeOfWord,rhs);
			ap3 = GenerateExpression(node->p[2], am_reg, cpu.sizeOfWord,rhs);
		}
		GenerateTriadic(op_ext, 0, ap1, ap2, ap3);
		ReleaseTempReg(ap3);
		ReleaseTempReg(ap2);
		/*
		ip = currentFn->pl.tail;
		ap1 = GenerateExpression(node->p[1], am_reg|am_imm|am_imm0, size);
		ap2 = GenerateExpression(node->p[2], am_reg|am_imm|am_imm0, size);
		if (ap1->mode != ap2->mode) {
			ReleaseTempReg(ap2);
			ReleaseTempReg(ap1);
			currentFn->pl.tail = ip;
			ap1 = GenerateExpression(node->p[1], am_reg, size);
			ap2 = GenerateExpression(node->p[2], am_reg, size);
		}
		//GenerateTriadic(op_and, 0, ap3, ap1, MakeImmediate(63));
		//GenerateTriadic(op_lsr, 0, ap1, ap1, ap2);
		ReleaseTempReg(ap2);
		ap1->next = ap3;
		ap1->preserveNextReg = true;
		*/
		goto retpt;
		/*
	case en_asfadd: return GenerateAssignAdd(node,flags,size,op_fadd);
	case en_asfsub: return GenerateAssignAdd(node,flags,size,op_fsub);
	case en_asfmul: return GenerateAssignAdd(node,flags,size,op_fmul);
	case en_asfdiv: return GenerateAssignAdd(node,flags,size,op_fdiv);
	*/
  case en_asadd:  ap1 = GenerateAssignAdd(node, flags, size, op_add);	goto retpt;
  case en_assub:  ap1 = node->GenerateAssignAdd(flags,size,op_sub); goto retpt;
  case en_asand:  ap1 = node->GenerateAssignLogic(flags,size,op_and); goto retpt;
  case en_asor:   ap1 = node->GenerateAssignLogic(flags,size,op_or); goto retpt;
	case en_asxor:  ap1 = node->GenerateAssignLogic(flags,size,op_xor); goto retpt;
  case en_aslsh:  ap1 = (node->GenerateAssignShift(flags,size,op_asl)); goto retpt;
  case en_asrsh:  ap1 = (node->GenerateAssignShift(flags,size,op_asr)); goto retpt;
	case en_asrshu: ap1 = (node->GenerateAssignShift(flags,size,op_lsr)); goto retpt;
  case en_asmul: ap1 = GenerateAssignMultiply(node,flags,size,op_mul); goto retpt;
  case en_asmulu: ap1 = GenerateAssignMultiply(node,flags,size,op_mulu); goto retpt;
  case en_asdiv: ap1 = GenerateAssignModiv(node,flags,size,op_div); goto retpt;
  case en_asdivu: ap1 = GenerateAssignModiv(node,flags,size,op_divu); goto retpt;
  case en_asmod: ap1 = GenerateAssignModiv(node,flags,size,op_mod); goto retpt;
  case en_asmodu: ap1 = GenerateAssignModiv(node,flags,size,op_modu); goto retpt;
  case en_assign:
		ap1 = GenerateAssign(node, flags, size);
		goto retpt;

	case en_chk:
		compiler.expr_depth--;
    return (GenExpr(node));
         
  case en_eq:     case en_ne:
  case en_lt:     case en_le:
  case en_gt:     case en_ge:
  case en_ult:    case en_ule:
  case en_ugt:    case en_uge:
  case en_feq:    case en_fne:
  case en_flt:    case en_fle:
  case en_fgt:    case en_fge:
  case en_veq:    case en_vne:
  case en_vlt:    case en_vle:
  case en_vgt:    case en_vge:
		ap1 = GenExpr(node);
		ap1->isBool = true;
		goto retpt;

	case en_cond:
		ap1 = GenerateHook(node, flags, size);
		goto retpt;
	case en_safe_cond:
		ap1 = GenerateMux(node, flags, size);
		goto retpt;

	case en_void:
    natsize = node->p[0]->GetNaturalSize();
		ap1 = GenerateExpression(node->p[0], am_all | am_novalue, natsize, rhs);
		ReleaseTempRegister(GenerateExpression(node->p[1], flags, size, rhs));
		ap1->isPtr = node->IsPtr();
		goto retpt;

	// A cast is represented as a node containing only the type and a second node
	// containing the expression tree. There is nothing to evaluate for the type,
	// it is simply transferred to the expression tree result.

	case en_cast:
		natsize = node->p[0]->GetNaturalSize();
		//ReleaseTempRegister(GenerateExpression(node->p[0], am_all | am_novalue, natsize, rhs));
		ap1 = GenerateExpression(node->p[1], flags, natsize, rhs);
		ap1->tp = node->p[0]->tp;
		ap1->isPtr = node->p[0]->IsPtr();
		goto retpt;

  case en_fcall:
		ap1 = (cg.GenerateFunctionCall(node,flags));
		goto retpt;

	case en_sxb:
		ap1 = GetTempRegister();
		ap2 = GenerateExpression(node->p[0], am_reg, 1, rhs);
		GenerateSignExtendByte(ap1, ap2);
		ReleaseTempReg(ap2);
		goto retpt;

	case en_sxc:
		ap1 = GetTempRegister();
		ap2 = GenerateExpression(node->p[0], am_reg, 2, rhs);
		GenerateSignExtendWyde(ap1, ap2);
		ReleaseTempReg(ap2);
		goto retpt;

	case en_sxh:
		ap1 = GetTempRegister();
		ap2 = GenerateExpression(node->p[0], am_reg|am_imm, 4, rhs);
		if (ap2->mode == am_imm) {
			Int128 j = Int128::Convert(0xffffffffLL);
			if (Int128::IsLT(&ap2->offset->i128, &j)) {
				ReleaseTempRegister(ap1);
				ap1 = ap2;
				goto retpt;
			}
		}
		if (regs[ap2->preg].isConst) {
			Int128 j = Int128::Convert(0xffffffffLL);
			if (Int128::IsLT(&regs[ap2->preg].val128, &j)) {
				ap1 = MakeImmediate(regs[ap2->preg].val128);
				goto retpt;
			}
		}
		ap2->MakeLegal(am_reg, 4);
		GenerateSignExtendTetra(ap1, ap2);
		//GenerateDiadic(op_sxt, 0, ap1, ap2);
		ReleaseTempReg(ap2);
		goto retpt;

	case en_ubyt2hexi:
	case en_ubyt2octa:
	case en_ubyt2tetra:
		ap1 = GenerateExpression(node->p[0],am_reg|am_imm,1,rhs);
		if (ap1->mode == am_imm) {
			ap1->offset->i &= 0xffLL;
			ap1->offset->i128.low &= 0xffLL;
			ap1->offset->i128.high = 0;
		}
		else
			GenerateDiadic(op_movzxb, 0, ap1, ap1);
		goto retpt;

	case en_uwyde2hexi:
	case en_uwyde2octa:
	case en_uwyde2tetra:
		ap1 = GenerateExpression(node->p[0],am_reg,2,rhs);
		GenerateZeroExtendWyde(ap1, ap1);
		goto retpt;
	case en_wyde2ptr:
	case en_ccwp:
		ap1 = GenerateExpression(node->p[0], am_reg, 2,rhs);
		ap1->isPtr = TRUE;
		GenerateSignExtendWyde(ap1, ap1);
		goto retpt;
	case en_uwyde2ptr:
	case en_cucwp:
		ap1 = GenerateExpression(node->p[0], am_reg, 2,rhs);
		ap1->isPtr = TRUE;
		GenerateZeroExtendWyde(ap1, ap1);
		goto retpt;
	case en_utetra2hexi:
	case en_utetra2octa:
		ap1 = GenerateExpression(node->p[0],am_reg,4,rhs);
		GenerateZeroExtendTetra(ap1, ap1);
		goto retpt;
	case en_byt2hexi:
	case en_byt2octa:
		ap1 = GetTempRegister();
		ap2 = GenerateExpression(node->p[0],am_reg,1,rhs);
		if (ap2->mode != am_imm) {
			GenerateSignExtendByte(ap1, ap2);
			ReleaseTempRegister(ap2);
		}
		else {
			ReleaseTempRegister(ap1);
			ap1 = ap2;
		}
		//GenerateDiadic(op_sxb,0,ap1,ap1);
		//GenerateDiadic(op_sxb,0,ap1,ap1);
		goto retpt;
	case en_wyde2hexi:
	case en_wyde2octa:
		ap1 = GetTempRegister();
		ap2 = GenerateExpression(node->p[0],am_reg|am_imm,2,rhs);
		if (ap2->mode != am_imm) {
			GenerateSignExtendWyde(ap1, ap2);
			ReleaseTempRegister(ap2);
		}
		else {
			ReleaseTempRegister(ap1);
			ap1 = ap2;
		}
		//		GenerateDiadic(op_sxw,0,ap1,ap1);
		goto retpt;
	case en_tetra2hexi:
	case en_tetra2octa:
		ap1 = GetTempRegister();
		ap2 = GenerateExpression(node->p[0],am_reg|am_imm,4,rhs);
		if (ap2->mode != am_imm) {
			GenerateSignExtendTetra(ap1, ap2);
			ReleaseTempRegister(ap2);
		}
		else {
			ReleaseTempRegister(ap1);
			ap1 = ap2;
		}
		//		GenerateDiadic(op_sxt,0,ap1,ap1);
		goto retpt;
	case en_octa2hexi:
		/*
		ap1 = GetTempRegister();
		ap2 = GenerateExpression(node->p[0], am_reg | am_imm, 8, rhs);
		if (ap2->mode == am_imm) {
			Int128 j = Int128::Convert(0xffffffffLL);
			if (Int128::IsLT(&ap2->offset->i128, &j)) {
				ReleaseTempRegister(ap1);
				ap1 = ap2;
				goto retpt;
			}
		}
		if (regs[ap2->preg].isConst) {
			Int128 j = Int128::Convert(0xffffffffLL);
			if (Int128::IsLT(&regs[ap2->preg].val128, &j)) {
				ap1 = MakeImmediate(regs[ap2->preg].val128);
				goto retpt;
			}
		}
		Generate4adic(op_sbx, 0, ap1, ap2, MakeImmediate(31), MakeImmediate(95));
		//GenerateDiadic(op_sxt, 0, ap1, ap2);
		ReleaseTempReg(ap2);
		goto retpt;
		*/
		ap1 = GenerateExpression(node->p[0], am_reg | am_imm, 8, rhs);
		compiler.expr_depth--;
		return (ap1);
		ap1 = GetTempRegister();
		ap2 = GenerateExpression(node->p[0], am_reg|am_imm, 8, rhs);
		if (ap2->mode != am_imm) {
//			Generate4adic(op_sbx, 0, ap1, ap2, MakeImmediate(63), MakeImmediate(127));
			ReleaseTempRegister(ap2);
		}
		else {
			ReleaseTempRegister(ap1);
			ap1 = ap2;
		}
		goto retpt;
	case en_uocta2hexi:
		ap1 = GenerateExpression(node->p[0], am_reg | am_imm, 8, rhs);
		//GenerateDiadic(op_zxo, 0, ap1, ap1);
		goto retpt;

	case en_list:
		ap1 = GetTempRegister();
		if (use_gp) {
			ndxreg = GetSegmentIndexReg((e_sg)node->segment);
			GenerateLoadAddress(ap1, MakeDataLabel(node->i, ndxreg));
		}
		else
			GenerateLoadAddress(ap1, MakeDataLabel(node->i, regZero));
		//if (!compiler.os_code) {
		//	switch (node->segment) {
		//	case tlsseg:		GenerateTriadic(op_base, 0, ap1, ap1, MakeImmediate(8));	break;
		//	case rodataseg:	GenerateTriadic(op_base, 0, ap1, ap1, MakeImmediate(12));	break;
		//	}
		//}
		ap1->isPtr = true;
		goto retpt;
	case en_object_list:
		ap1 = GetTempRegister();
		GenerateLoadAddress(ap1,MakeIndexed(-8,regFP));
		//if (!compiler.os_code) {
		//	switch (node->segment) {
		//	case tlsseg:		GenerateTriadic(op_base, 0, ap1, ap1, MakeImmediate(8));	break;
		//	case rodataseg:	GenerateTriadic(op_base, 0, ap1, ap1, MakeImmediate(12));	break;
		//	}
		//}
		goto retpt;

	case en_switch:
		ap1 = StatementGenerator::GenerateSwitch(node);
		goto retpt;

	default:
    printf("DIAG - uncoded node (%d) in GenerateExpression.\n", node->nodetype);
		compiler.expr_depth--;
    return (0);
  }
	ap1 = nullptr;
	goto retpt2;
	compiler.expr_depth--;
	return(0);
retpt:
	ap1->MakeLegal(flags, size);
retpt2:
	if (node->pfl) {
		ReleaseTempRegister(cg.GenerateExpression(node->pfl, flags, size, rhs));
	}
	compiler.expr_depth--;
	return (ap1);
}

//
// Generate a jump to label if the node passed evaluates to
// a true condition.
//
void CodeGenerator::GenerateTrueJump(ENODE *node, int64_t label, unsigned int prediction)
{ 
	Operand  *ap1, *ap2;
	int64_t lab0;
	int64_t siz1;

	if( node == 0 )
		return;
	switch( node->nodetype )
	{
	case en_bchk:	break;
	case en_eq:	GenerateBranch(node, op_eq, label, 0, prediction, false); break;
	case en_ne: GenerateBranch(node, op_ne, label, 0, prediction, false); break;
	case en_lt: GenerateBranch(node, op_lt, label, 0, prediction, false); break;
	case en_le:	GenerateBranch(node, op_le, label, 0, prediction, false); break;
	case en_gt: GenerateBranch(node, op_gt, label, 0, prediction, false); break;
	case en_ge: GenerateBranch(node, op_ge, label, 0, prediction, false); break;
	case en_ult: GenerateBranch(node, op_ltu, label, 0, prediction, false); break;
	case en_ule: GenerateBranch(node, op_leu, label, 0, prediction, false); break;
	case en_ugt: GenerateBranch(node, op_gtu, label, 0, prediction, false); break;
	case en_uge: GenerateBranch(node, op_geu, label, 0, prediction, false); break;
	case en_feq: GenerateBranch(node, op_feq, label, 0, prediction, false); break;
	case en_fne: GenerateBranch(node, op_fne, label, 0, prediction, false); break;
	case en_flt: GenerateBranch(node, op_flt, label, 0, prediction, false); break;
	case en_fle: GenerateBranch(node, op_fle, label, 0, prediction, false); break;
	case en_fgt: GenerateBranch(node, op_fgt, label, 0, prediction, false); break;
	case en_fge: GenerateBranch(node, op_fge, label, 0, prediction, false); break;
	case en_veq: GenerateBranch(node, op_vseq, label, 0, prediction, false); break;
	case en_vne: GenerateBranch(node, op_vsne, label, 0, prediction, false); break;
	case en_vlt: GenerateBranch(node, op_vslt, label, 0, prediction, false); break;
	case en_vle: GenerateBranch(node, op_vsle, label, 0, prediction, false); break;
	case en_vgt: GenerateBranch(node, op_vsgt, label, 0, prediction, false); break;
	case en_vge: GenerateBranch(node, op_vsge, label, 0, prediction, false); break;
	case en_lor_safe:
		if (GenerateBranch(node, op_or, label, 0, prediction, true))
			break;
	case en_lor:
		GenerateTrueJump(node->p[0], label, prediction);
		GenerateTrueJump(node->p[1], label, prediction);
		break;
	case en_land_safe:
		if (GenerateBranch(node, op_and, label, 0, prediction, true))
			break;
	case en_land:
		lab0 = nextlabel++;
		GenerateFalseJump(node->p[0], lab0, prediction);
		GenerateTrueJump(node->p[1], label, prediction ^ 1);
		GenerateLabel(lab0);
		break;
	default:
		siz1 = node->GetNaturalSize();
		ap1 = GenerateExpression(node,am_reg,siz1,1);
		//                        GenerateDiadic(op_tst,siz1,ap1,0);
		ReleaseTempRegister(ap1);
		if (ap1->tp->IsFloatType()) {
			ap2 = GetTempRegister();
			GenerateTriadic(op_fcmp, 0, ap2, ap1, makereg(regZero));
			GenerateTriadic(op_bbs, 0, ap2, MakeImmediate(0), MakeCodeLabel(label));	// bit 0 is eq
			ReleaseTempReg(ap2);
		}
		else {
			ap2 = MakeBoolean(ap1);
			ReleaseTempReg(ap1);
			GenerateBranchTrue(ap2, label);
		}
		break;
	}
}

//
// Generate code to execute a jump to label if the expression
// passed is false.
//
void CodeGenerator::GenerateFalseJump(ENODE *node, int64_t label, unsigned int prediction)
{
	Operand *ap, *ap1;
	int64_t siz1;
	int64_t lab0;

	if( node == (ENODE *)NULL )
		return;
	switch( node->nodetype )
	{
	case en_bchk:	break;
	case en_eq:	GenerateBranch(node, op_ne, label, 0, prediction, false); break;
	case en_ne: GenerateBranch(node, op_eq, label, 0, prediction, false); break;
	case en_lt: GenerateBranch(node, op_ge, label, 0, prediction, false); break;
	case en_le: GenerateBranch(node, op_gt, label, 0, prediction, false); break;
	case en_gt: GenerateBranch(node, op_le, label, 0, prediction, false); break;
	case en_ge: GenerateBranch(node, op_lt, label, 0, prediction, false); break;
	case en_ult: GenerateBranch(node, op_geu, label, 0, prediction, false); break;
	case en_ule: GenerateBranch(node, op_gtu, label, 0, prediction, false); break;
	case en_ugt: GenerateBranch(node, op_leu, label, 0, prediction, false); break;
	case en_uge: GenerateBranch(node, op_ltu, label, 0, prediction, false); break;
	case en_feq: GenerateBranch(node, op_fne, label, 0, prediction, false); break;
	case en_fne: GenerateBranch(node, op_feq, label, 0, prediction, false); break;
	case en_flt: GenerateBranch(node, op_fge, label, 0, prediction, false); break;
	case en_fle: GenerateBranch(node, op_fgt, label, 0, prediction, false); break;
	case en_fgt: GenerateBranch(node, op_fle, label, 0, prediction, false); break;
	case en_fge: GenerateBranch(node, op_flt, label, 0, prediction, false); break;
	case en_veq: GenerateBranch(node, op_vsne, label, 0, prediction, false); break;
	case en_vne: GenerateBranch(node, op_vseq, label, 0, prediction, false); break;
	case en_vlt: GenerateBranch(node, op_vsge, label, 0, prediction, false); break;
	case en_vle: GenerateBranch(node, op_vsgt, label, 0, prediction, false); break;
	case en_vgt: GenerateBranch(node, op_vsle, label, 0, prediction, false); break;
	case en_vge: GenerateBranch(node, op_vslt, label, 0, prediction, false); break;
	case en_land_safe:
		if (GenerateBranch(node, op_nand, label, 0, prediction, true))
			break;
	case en_land:
		GenerateFalseJump(node->p[0],label,prediction^1);
		GenerateFalseJump(node->p[1],label,prediction^1);
		break;
	case en_lor_safe:
		if (GenerateBranch(node, op_nor, label, 0, prediction,true))
			break;
	case en_lor:
		lab0 = nextlabel++;
		GenerateTrueJump(node->p[0],lab0,prediction);
		GenerateFalseJump(node->p[1],label,prediction^1);
		GenerateLabel(lab0);
		break;
	case en_not:
		GenerateTrueJump(node->p[0],label,prediction);
		break;
	default:
		siz1 = node->GetNaturalSize();
		ap = GenerateExpression(node,am_reg,siz1,1);
		//                        GenerateDiadic(op_tst,siz1,ap,0);
		ReleaseTempRegister(ap);
		//if (ap->mode == am_fpreg) {
		//	GenerateTriadic(op_fseq, 0, makecreg(1), ap, makefpreg(0));
		//	GenerateDiadic(op_bt, 0, makecreg(1), MakeCodeLabel(label));
		//}
		//else
		{
			GenerateDiadic(op_beqz, 0, ap, MakeCodeLabel(label));
			if (false) {
//				if (ap->offset->nodetype==en_icon && ap->offset->i != 0)
//					GenerateMonadic(op_branch, 0, MakeCodeLabel(label));
//				else
				{
					ap1 = MakeBoolean(ap);
					ReleaseTempReg(ap);
					GenerateBranchFalse(ap1, label);
				}
			}
		}
		break;
	}
}

void CodeGenerator::SaveTemporaries(Function *sym, int *sp, int *fsp, int* psp, int* vsp)
{
	if (sym) {
		if (sym->UsesTemps) {
			*sp = TempInvalidate(fsp, psp, vsp);
			//*fsp = TempFPInvalidate();
		}
	}
	else {
		*sp = TempInvalidate(fsp, psp, vsp);
		//*fsp = TempFPInvalidate();
	}
}

void CodeGenerator::RestoreTemporaries(Function *sym, int sp, int fsp, int psp,  int vsp)
{
	if (sym) {
		if (sym->UsesTemps) {
			//TempFPRevalidate(fsp);
			TempRevalidate(sp, fsp, psp, vsp);
		}
	}
	else {
		//TempFPRevalidate(fsp);
		TempRevalidate(sp, fsp, psp, vsp);
	}
}

void CodeGenerator::RestoreRegisterVars(Function* func)
{
	if (!func->prolog) {
		func->RestorePositRegisterVars();
		func->RestoreFPRegisterVars();
		GenerateHint(begin_restore_regvars);
		func->RestoreGPRegisterVars();
		GenerateHint(end_restore_regvars);
	}
}

int CodeGenerator::RestoreGPRegisterVars(CSet* save_mask)
{
	int cnt2 = 0, cnt;
	int nn;
	int64_t mask;

	if (save_mask == nullptr)
		return (0);
	if (save_mask->NumMember()) {
		if (cpu.SupportsLDM && save_mask->NumMember() > 3) {
			mask = 0;
			for (nn = 0; nn < 64; nn++)
				if (save_mask->isMember(nn))
					mask = mask | (1LL << (nn - 1));
			//GenerateMonadic(op_reglist, 0, cg.MakeImmediate(mask, 16));
			GenerateDiadic(op_loadm, 0, cg.MakeImmediate(mask, 16), cg.MakeIndirect(regSP));
		}
		else {
			cnt2 = cnt = save_mask->NumMember() * cpu.sizeOfWord;
			cnt = 0;
			save_mask->resetPtr();
			for (nn = 0; nn < save_mask->NumMember(); nn++) {
				if (nn == 0)
					cg.GenerateLoad(makereg(cpu.saved_regs[0]), MakeIndirect(regSP), cpu.sizeOfWord, cpu.sizeOfWord);
				else
					cg.GenerateLoad(makereg(cpu.saved_regs[nn]), MakeIndexed(cpu.sizeOfWord * nn, regSP), cpu.sizeOfWord, cpu.sizeOfWord);
			}
			/*
			if (save_mask->NumMember() == 1)
				cg.GenerateLoad(makereg(cpu.saved_regs[0]), MakeIndirect(regSP), cpu.sizeOfWord, cpu.sizeOfWord);
			else if (save_mask->NumMember() == 2) {
				cg.GenerateLoad(makereg(cpu.saved_regs[0]), MakeIndirect(regSP), cpu.sizeOfWord, cpu.sizeOfWord);
				cg.GenerateLoad(makereg(cpu.saved_regs[1]), MakeIndexed(cpu.sizeOfWord,regSP), cpu.sizeOfWord, cpu.sizeOfWord);
			}
			else {
				sprintf_s(buf, sizeof(buf), "__load_s0s%d", save_mask->NumMember() - 1);
				cg.GenerateMillicodeCall(MakeStringAsNameConst(buf, codeseg));
			}
			*/
			/*
			for (nn = save_mask->nextMember(); nn >= 0; nn = save_mask->nextMember()) {
				cg.GenerateLoad(makereg(nn), MakeIndexed(cnt, regSP), cpu.sizeOfWord, cpu.sizeOfWord);
				cnt += cpu.sizeOfWord;
			}
			*/
		}
	}
	return (cnt2);
}



// Store entire argument list onto stack
//
int CodeGenerator::GenerateInlineArgumentList(Function *sym, ENODE *plist)
{
	Operand *ap;
	TypeArray *ta = nullptr;
	int i, sum;
	ENODE *p;
	ENODE *pl[100];
	int nn, maxnn;
	struct slit *st;
	char *cp;

	cp = nullptr;
	sum = 0;
	if (sym)
		ta = sym->GetProtoTypes();

	// Capture the parameter list. It is needed in the reverse order.
	for (nn = 0, p = plist; p != NULL; p = p->p[1], nn++) {
		pl[nn] = p->p[0];
	}
	maxnn = nn;
	for (--nn, i = 0; nn >= 0; --nn, i++)
	{
		if (pl[nn]->etype == bt_pointer) {
			if (pl[nn]->tp->btpp == nullptr)
				continue;
			if (pl[nn]->tp->btpp->type == bt_ichar || pl[nn]->tp->btpp->type == bt_iuchar) {
				for (st = strtab; st; st = st->next) {
					if (st->label == pl[nn]->i) {
						cp = st->str;
						break;
					}
				}
				ap = MakeString(cp);
				GenerateMonadic(op_string, 0, ap);
			}
		}
	}
	if (ta)
		delete ta;
	return (sum);
}

/* Generates the code for a trinary operation. (Needs to be improved).
* Try and keep this routine generic so it should not need to be present in
* derived classes.
*
* Parameters:
*		node: (input) pointer to expression tree
*		flags: (input) legal addressing modes
*		size: (input) the operation size
*		op:	(input) the operation to perform
* Returns:
*		pointer to operand containing the result
*/

Operand* CodeGenerator::GenerateTrinary(ENODE* node, int flags, int64_t size, int op)
{
	Operand* ap1 = nullptr, * ap2 = nullptr, * ap3, * ap4;
	bool dup = false;

	if (node->IsFloatType())
	{
		ap3 = GetTempFPRegister();
		if (node->IsEqual(node->p[0], node->p[1]))
			dup = !opt_nocgo;
		ap1 = cg.GenerateExpression(node->p[0], am_reg, size,1);
		if (!dup)
			ap2 = cg.GenerateExpression(node->p[1], am_reg, size,1);
		// Generate a convert operation ?
		if (!dup) {
			if (ap1->fpsize() != ap2->fpsize()) {
				if (ap2->fpsize() == 's')
					GenerateDiadic(op_fcvtsq, 0, ap2, ap2);
			}
		}
		if (dup)
			GenerateTriadic(op, ap1->fpsize(), ap3, ap1, ap1);
		else
			GenerateTriadic(op, ap1->fpsize(), ap3, ap1, ap2);
		ap3->type = ap1->type;
	}
	else if (op == op_vex) {
		ap3 = GetTempRegister();
		ap1 = cg.GenerateExpression(node->p[0], am_reg, size,1);
		ap2 = cg.GenerateExpression(node->p[1], am_reg, size,1);
		GenerateTriadic(op, 0, ap3, ap1, ap2);
	}
	else if (node->IsVectorType()) {
		ap3 = GetTempVectorRegister();
		if (ENODE::IsEqual(node->p[0], node->p[1]) && !opt_nocgo) {
			ap1 = cg.GenerateExpression(node->p[0], am_vreg, size,1);
			ap2 = cg.GenerateExpression(node->vmask, am_vmreg, size,1);
			Generate4adic(op, 0, ap3, ap1, ap1, ap2);
		}
		else {
			ap1 = cg.GenerateExpression(node->p[0], am_vreg, size,1);
			ap2 = cg.GenerateExpression(node->p[1], am_vreg, size,1);
			ap4 = cg.GenerateExpression(node->vmask, am_vmreg, size,1);
			Generate4adic(op, 0, ap3, ap1, ap2, ap4);
			ReleaseTempReg(ap4);
		}
		// Generate a convert operation ?
		//if (fpsize(ap1) != fpsize(ap2)) {
		//	if (fpsize(ap2)=='s')
		//		GenerateDiadic(op_fcvtsq, 0, ap2, ap2);
		//}
	}
	else {
		ap3 = GetTempRegister();
		if (flags & am_bf_assign)
			return (ap3);
		{
			OCODE* ip;

			ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op)->amclass2, size,1);
			// modu/ptrdif does not have an immediate mode
			ip = currentFn->pl.tail;
			ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op)->amclass3, size,1);
			if (Instruction::Get(op)->amclass4) {	// op_ptrdif
				ap4 = cg.GenerateExpression(node->p[2], Instruction::Get(op)->amclass4, size,1);
				Generate4adic(op, 0, ap3, ap1, ap2, ap4);
				if (ap2->mode != ap4->mode) {
					currentFn->pl.tail = ip;
					ap2 = cg.GenerateExpression(node->p[1], am_reg, size,1);
					ap4 = cg.GenerateExpression(node->p[2], am_reg, size,1);
					Generate4adic(op, 0, ap3, ap1, ap2, ap4);
				}
			}
			else {
				if (ap2->mode == am_imm) {
					switch (op) {
					case op_and:
						GenerateTriadic(op, 0, ap3, ap1, MakeImmediate(ap2->offset->i));
						break;
					case op_or:
						GenerateTriadic(op, 0, ap3, ap1, MakeImmediate(ap2->offset->i));
						break;
						// If there is a pointer plus a constant we really wanted an address calc.
					case op_add:
					case op_sub:
						if (ap1->isPtr && ap2->isPtr)
							GenerateTriadic(op, 0, ap3, ap1, ap2);
						else if (ap2->isPtr) {
							GenerateLoadAddress(ap3, op == op_sub ? compiler.of.MakeNegIndexed(ap2->offset, ap1->preg) : MakeIndexed(ap2->offset, ap1->preg));
							//if (!compiler.os_code) {
							//	switch (ap3->segment) {
							//	case tlsseg:		GenerateTriadic(op_base, 0, ap3, ap3, MakeImmediate(8));	break;
							//	case rodataseg:	GenerateTriadic(op_base, 0, ap3, ap3, MakeImmediate(12));	break;
							//	}
							//}
						}
						else
							GenerateTriadic(op, 0, ap3, ap1, ap2);
						break;
					default:
						GenerateTriadic(op, 0, ap3, ap1, ap2);
					}
				}
				else
					GenerateTriadic(op, 0, ap3, ap1, ap2);
			}
		}
	}
	if (ap2)
		ReleaseTempReg(ap2);
	if (ap1)
		ReleaseTempReg(ap1);
	ap3->MakeLegal(flags, size);
	return (ap3);
}

/* Determine if the expression tree has isPascal set anywhere.
* Only needed for indirect function calls.
* Is a recursive algorithm.
* 
* Parameters:
*		ep:	(input) pointer to expression tree
* Returns:
*		Boolean true if isPascal is set
*/
extern int defaultcc;
bool CodeGenerator::IsPascal(ENODE* ep)
{
	static int level = 0;

	level++;
	if (level > 150) {
		error(ERR_EXPRTOOCOMPLEX);
		throw new ArplException(ERR_EXPRTOOCOMPLEX, 99);
	}
	if (ep == nullptr) {
		level--;
		return (defaultcc == 1);
	}
	if (ep->isPascal) {
		level--;
		return (true);
	}
	if (IsPascal(ep->p[0]) || IsPascal(ep->p[1]) || IsPascal(ep->p[2])) {
		level--;
		return (true);
	}
	level--;
	return (false);
}

int64_t CodeGenerator::GeneratePrepareFunctionCall(ENODE* node, Function* sym, int* sp, int* fsp, int* psp, int* vsp)
{
	int64_t i;
	List* lst, *hlst;
	ENODE* en;
	int64_t sz, sum;
	int64_t offs[100];
	int64_t count;

	if (sym)
		sym->SaveTemporaries(sp, fsp, psp, vsp);
	if (currentFn->HasRegisterParameters())
		if (sym)
			sym->SaveRegisterArguments();
	// Go through the list of arguments looking for aggregates. These will need to
	// be allocated on stack and copied, then a pointer to the stack area pushed.
	// We want the area allocated before pushing other values.
	count = sum = 0;
	hlst = nullptr;
	if (node->p[1]) {
		for (hlst = lst = node->p[1]->ReverseList(node->p[1]); lst; lst = lst->nxt) {
			en = lst->node;
			if (en && en->esize > cpu.sizeOfWord) {
				if (en->etype == bt_struct || en->etype == bt_union || en->etype == bt_class) {
					sz = roundWord(en->esize);
					sum += sz;
					offs[count] = sum;
					count++;
				}
			}
		}
	}
	// Allocate stack buffers.
	if (sum > 0)
		GenerateSubtractFrom(makereg(regSP), MakeImmediate(sum, 0));
	count = 0;
	if (hlst)
	for (lst = hlst; lst; lst = lst->nxt) {
		en = lst->node;
		en->stack_offs = offs[count];
		count++;
	}

	i = PushArguments(sym, node->p[1]) + (sum / cpu.sizeOfWord);
	// If the symbol is unknown, assume a throw is present
	if (sym) {
		if (sym->DoesThrow)
			currentFn->DoesThrow = true;
	}
	else
		currentFn->DoesThrow = true;
	return (i);
}

void CodeGenerator::GenerateMillicodeCall(Operand* tgt)
{
	GenerateDiadic(op_bsr, 0, makereg(regLR + 3), tgt);
}

bool CodeGenerator::GenerateInlineCall(ENODE* node, Function* sym)
{
	Function* o_fn;
	CSet* mask, * fmask, * pmask;
	int ps;
	OCODE* ip, * pip, *cip, *hip;
	static int instance = 1024;
	int oldlab;
	std::map<int, int> labelmap;
	bool code_generated = false;

	o_fn = currentFn;
	mask = save_mask;
	fmask = fpsave_mask;
	pmask = psave_mask;
	currentFn = sym;
	ps = pass;
	// Each function has it's own peeplist. The generated peeplist for an
	// inline function must be appended onto the peeplist of the current
	// function.
	//sym->pl.head = sym->pl.tail = nullptr;
	hip = o_fn->pl.head;
	for (ip = sym->pl.head; ip; ip = pip) {
		if (ip->opcode != op_fnname &&
			ip->opcode != op_rts &&
			ip->opcode != op_retd &&
			ip->opcode != op_ret) {
			cip = ip->Clone(ip);
			if (cip->opcode == op_label) {
				labelmap[(int)cip->oper1] = 1;
				oldlab = (int)cip->oper1 + instance;
				cip->oper1 = (Operand*)oldlab;
			}
			o_fn->pl.Add(cip);
			code_generated = true;
		}
		pip = ip->fwd;
	}
	// Go through the list and replace old labels with new ones.
	for (ip = hip; ip; ip = ip->fwd) {
		if (Instruction::Get(ip->opcode)->Instruction::IsFlowControl()) {
			if (ip->oper1)
				if (ip->oper1->offset)
					if (ip->oper1->offset->nodetype == en_clabcon) {
						if (labelmap[(int)ip->oper1->offset->i] == 1) {
							ip->oper1->offset->i = ip->oper1->offset->i + instance;
							DataLabels[ip->oper1->offset->i]++;
						}
					}
			if (ip->oper2)
				if (ip->oper2->offset)
					if (ip->oper2->offset->nodetype == en_clabcon) {
						if (labelmap[(int)ip->oper2->offset->i] == 1) {
							ip->oper2->offset->i = ip->oper2->offset->i + instance;
							DataLabels[ip->oper2->offset->i]++;
						}
					}
			if (ip->oper3)
				if (ip->oper3->offset)
					if (ip->oper3->offset->nodetype == en_clabcon) {
						if (labelmap[(int)ip->oper3->offset->i] == 1) {
							ip->oper3->offset->i = ip->oper3->offset->i + instance;
							DataLabels[ip->oper3->offset->i]++;
						}
					}
			if (ip->oper4)
				if (ip->oper4->offset)
					if (ip->oper4->offset->nodetype == en_clabcon) {
						if (labelmap[(int)ip->oper4->offset->i] == 1) {
							ip->oper4->offset->i = ip->oper4->offset->i + instance;
							DataLabels[ip->oper4->offset->i]++;
						}
					}
			}
	}
	//sym->Generate();
	pass = ps;
	currentFn = o_fn;
	//currentFn->pl.tail->fwd = sym->pl.head;
	//currentFn->pl.tail = sym->pl.tail;
	if (node->isAutonew)
		currentFn->hasAutonew = true;
	fpsave_mask = fmask;
	save_mask = mask;
	psave_mask = pmask;
	instance += 1024;
	return (code_generated);
}

Operand* CodeGenerator::GenerateFunctionCall(ENODE* node, int flags, int lab)
{
	Operand* ap;
	Function* sym;
	Symbol* s;
	int i;
	int sp = 0;
	int fsp = 0;
	int psp = 0;
	int vsp = 0;
	TypeArray* ta = nullptr;
	Expression exp(stmt);

	sym = nullptr;
	ap = nullptr;

	// Call the function, the function will be called directly by name if the node
	// indicates a name constant. Otherwise the function will be called indirectly
	// via a value loaded into a register.

	GenerateHint(begin_func_call);
	i = 0;
	if (node->p[0]->nodetype == en_nacon || node->p[0]->nodetype == en_cnacon) {
		if (node->p[0])
			s = currentSym = node->sym;
		else
			s = exp.gsearch2(*node->p[0]->sp, bt_int, nullptr, false);
		if (s)
			sym = s->fi;
		/*
				if ((sym->tp->btpp->type==bt_struct || sym->tp->btpp->type==bt_union) && sym->tp->btpp->size > 8) {
							nn = tmpAlloc(sym->tp->btpp->size) + lc_auto + roundWord(sym->tp->btpp->size);
							GenerateMonadic(op_pea,0,MakeIndexed(-nn,regFP));
							i = 1;
					}
	*/
		i += GeneratePrepareFunctionCall(node, sym, &sp, &fsp, &psp, &vsp);

		if (sym && sym->IsInline)
			GenerateInlineCall(node, sym);
		else {
			/*
			if (sym->sym->storage_class == sc_external) {
				jop = allocOperand();
				jop->mode = am_iindx2;
				jop->preg = regPC;
				jop->sreg = regGOT;
				jop->offset = node->p[0];
				jop->scale = 1;
				GenerateDiadic(op_jsr, 0, makereg(regLR), jop);
			}
			else
			*/
				GenerateDirectJump(node, ap, sym, flags, lab);
		}
	}
	else
	{
		/*
			if ((node->p[0]->tp->btpp->type==bt_struct || node->p[0]->tp->btpp->type==bt_union) && node->p[0]->tp->btpp->size > 8) {
						nn = tmpAlloc(node->p[0]->tp->btpp->size) + lc_auto + roundWord(node->p[0]->tp->btpp->size);
						GenerateMonadic(op_pea,0,MakeIndexed(-nn,regFP));
						i = 1;
				}
		 */
		ap = GenerateExpression(node->p[0], am_reg, cpu.sizeOfPtr, 0);
		if (ap->offset) {
			if (ap->offset->sym)
				sym = ap->offset->sym->fi;
		}

		i += GeneratePrepareFunctionCall(node, sym, &sp, &fsp, &psp, &vsp);

		ap->mode = am_ind;
		ap->offset = 0;
		if (sym && sym->IsInline)
			GenerateInlineCall(node, sym);
		else
			GenerateIndirectJump(node, ap, sym, flags, lab);
	}

	GenerateInlineArgumentList(sym, node->p[1]);
	PopArguments(sym, i, IsPascal(node));
	if (currentFn->HasRegisterParameters())
		if (sym)
			sym->RestoreRegisterArguments();
	if (sym)
		sym->RestoreTemporaries(sp, fsp, psp, vsp);
	if (ap)
		ReleaseTempRegister(ap);

	// Here it is assumed that the function will return any value in the first
	// argument register. The register file for Thor is unified so it makes no
	// difference as to whether a float type or an integer type is returned.

	if (sym
		&& sym->sym
		&& sym->sym->tp
		&& sym->sym->tp->btpp
		&& sym->sym->tp->btpp->IsVectorType()) {
		GenerateHint(end_func_call);
		if (!(flags & am_novalue))
			return (makevreg(1));
		else
			return (makevreg(0));
	}

	if (sym
		&& sym->sym
		&& sym->sym->tp
		&& sym->sym->tp->btpp
		) {
		if (!(flags & am_novalue)) {
			if (sym->sym->tp->btpp->type != bt_void) {
				ap = makereg(cpu.argregs[0]);
				regs[cpu.argregs[0]].modified = true;
			}
			else
				ap = makereg(regZero);
			ap->isPtr = sym->sym->tp->btpp->type == bt_pointer;
		}
		else {
			GenerateHint(end_func_call);
			return(makereg(regZero));
		}
	}
	// Otherwise returning a int or a void.
	else {
		if (!(flags & am_novalue)) {
			//ap = GetTempRegister();
			ap = makereg(cpu.argregs[0]);
			//GenerateDiadic(cpu.mov_op, 0, ap, makereg(cpu.argregs[0]));
			regs[cpu.argregs[0]].modified = true;
		}
		// If the function has no return value, just return a zero (r0).
		else {
			GenerateHint(end_func_call);
			return(makereg(regZero));
		}
	}
	GenerateHint(end_func_call);
	return (ap);
}

void CodeGenerator::GenerateCoroutineExit(Function* func)
{
	Operand* ap;

	ap = GetTempRegister();
	GenerateLoadConst(MakeStringAsNameConst((char*)MakeConame(*func->sym->mangledName, "first").c_str(), codeseg), ap);
	GenerateStore(ap, MakeIndexedName(MakeConame(*func->sym->mangledName, "target"), regGP), cpu.sizeOfWord);
	GenerateLoad(ap, MakeIndexedName(MakeConame(*func->sym->mangledName, "orig_lr"), regGP), cpu.sizeOfWord, cpu.sizeOfWord);
	GenerateTriadic(op_csrrw, 0, makereg(regZero), ap, MakeImmediate(0x3102));
	ReleaseTempRegister(ap);
	GenerateLoad(makereg(regFP), MakeIndexedName(MakeConame(*func->sym->mangledName, "orig_fp"), regGP), cpu.sizeOfWord, cpu.sizeOfWord);
	GenerateLoad(makereg(regSP), MakeIndexedName(MakeConame(*func->sym->mangledName, "orig_sp"), regGP), cpu.sizeOfWord, cpu.sizeOfWord);
}

// Generate a return statement.
//
void CodeGenerator::GenerateReturn(Function* func, Statement* stmt)
{
	Operand* ap, * ap2;
	int nn;
	int64_t toAdd;
	Symbol* p;
	bool isFloat, isPosit, isVector;
	int64_t sz;
	TYP* tp;

	if (func == nullptr)
		throw new ArplException(ERR_NULLPOINTER, 0);

	// Generate the return expression and force the result into r1.
	if (stmt != NULL && stmt->exp != NULL)
	{
		initstack();
		tp = func->sym->tp->btpp;
		if (tp->type == bt_func)
			tp = tp->btpp;
		isFloat = tp && tp->IsFloatType();
		isPosit = tp && tp->IsPositType();
		isVector = tp && tp->IsVectorType();
		if (isFloat)
			ap = cg.GenerateExpression(stmt->exp, am_reg, cpu.sizeOfFP, 1);
		else if (isPosit)
			ap = cg.GenerateExpression(stmt->exp, am_reg, cpu.sizeOfPosit, 1);
		else if (isVector)
			ap = cg.GenerateExpression(stmt->exp, am_vreg, 64, 1);
		else
			ap = cg.GenerateExpression(stmt->exp, am_reg | am_imm, cpu.sizeOfWord, 1);
		GenerateMonadic(op_hint, 0, MakeImmediate(2));
		if (ap->mode == am_imm)
			GenerateDiadic(cpu.ldi_op, 0, makereg(cpu.argregs[0]), ap);
		else if (ap->mode == am_reg || ap->mode == am_vreg) {
			if (tp && (tp->type == bt_struct || tp->type == bt_union || tp->type == bt_class)) {
				if ((sz = tp->size) > cpu.sizeOfWord) {
					p = func->params.Find("_pHiddenStructPtr", false);
					if (p) {
						if (p->IsRegister)
							GenerateMove(makereg(cpu.argregs[0]), makereg(p->reg));
						else
							GenerateLoad(makereg(cpu.argregs[0]), MakeIndexed(p->value.i, regFP), cpu.sizeOfWord, cpu.sizeOfWord);
						ap2 = GetTempRegister();
						GenerateLoadConst(MakeImmediate(tp->size), ap2);
						if (cpu.SupportsPush) {
							GenerateMonadic(op_push, 0, ap2);
							GenerateMonadic(op_push, 0, ap);
							GenerateMonadic(op_push, 0, makereg(cpu.argregs[0]));
						}
						else {
							GenerateSubtractImmediate(makereg(regSP), makereg(regSP), MakeImmediate(cpu.sizeOfWord * 3));
							//GenerateTriadic(op_subtract, 0, makereg(regSP), makereg(regSP), MakeImmediate(cpu.sizeOfWord * 3));
							GenerateStore(makereg(cpu.argregs[0]), MakeIndirect(regSP), cpu.sizeOfWord);
							GenerateStore(ap, MakeIndexed(cpu.sizeOfWord, regSP), cpu.sizeOfWord);
							GenerateStore(ap2, MakeIndexed(cpu.sizeOfWord * 2, regSP), cpu.sizeOfWord);
						}
						ReleaseTempReg(ap2);
						GenerateCall(MakeStringAsNameConst((char*)"__aacpy", codeseg));
						GenerateMonadic(op_bex, 0, MakeDataLabel(throwlab, regZero));
						if (!func->IsPascal)
							GenerateAddOnto(makereg(regSP), MakeImmediate(cpu.sizeOfWord * 3));
					}
					else
						throw new ArplException(ERR_MISSING_HIDDEN_STRUCTPTR,0);
				}
				else {
					if (ap->isPtr) {
						if (sz > 4)
							GenerateLoad(makereg(cpu.argregs[0]), MakeIndirect(ap->preg), 8, 8);
						else if (sz > 2)
							GenerateLoad(makereg(cpu.argregs[0]), MakeIndirect(ap->preg), 4, 4);
						else if (sz > 1)
							GenerateLoad(makereg(cpu.argregs[0]), MakeIndirect(ap->preg), 2, 2);
						else
							GenerateLoad(makereg(cpu.argregs[0]), MakeIndirect(ap->preg), 1, 1);
					}
					else
						GenerateMove(makereg(cpu.argregs[0]), ap);
				}
			}
			else {
				if (tp->IsFloatType() || tp->IsPositType())
					cg.GenerateMove(makereg(cpu.argregs[0]), ap);
				else if (tp->IsVectorType())
					cg.GenerateMove(makevreg(cpu.vargregs[0]), ap);// , makereg(regZero | rt_invert));
				else
					cg.GenerateMove(makereg(cpu.argregs[0]), ap);
			}
		}
		/* I think this code cannot be reached. am_reg checked above
		else if (ap->mode == am_reg) {
			if (isFloat)
				GenerateDiadic(cpu.mov_op, 0, makereg(cpu.argregs[0]), ap);
			else
				GenerateDiadic(cpu.mov_op, 0, makereg(cpu.argregs[0]), ap);
		}
		else if (ap->mode == am_reg) {
			if (isPosit)
				GenerateDiadic(cpu.mov_op, 0, compiler.of.makepreg(cpu.argregs[0]), ap);
			else
				GenerateDiadic(cpu.mov_op, 0, makereg(cpu.argregs[0]), ap);
		}
		*/
		else if (ap->typep == &stddouble) {
			if (isFloat)
				GenerateDiadic(op_ldf, 'd', makereg(cpu.argregs[0]), ap);
			else
				GenerateLoad(makereg(cpu.argregs[0]), ap, cpu.sizeOfFPD, cpu.sizeOfFPD);
		}
		else {
			if (tp->IsVectorType())
				GenerateLoad(makevreg(cpu.vargregs[0]), ap, cpu.sizeOfWord, cpu.sizeOfWord, makereg(regZero | rt_invert));
			else
				GenerateLoad(makereg(cpu.argregs[0]), ap, cpu.sizeOfWord, cpu.sizeOfWord);
		}
		ReleaseTempRegister(ap);
	}

	// Generate the return code only once. Branch to the return code for all returns.
	if (func->retGenerated) {
		GenerateBra(retlab);
		return;
	}
	func->retGenerated = true;
	GenerateLabel(retlab);

	if (func->IsCoroutine)
		GenerateCoroutineExit(func);

	func->rcode = func->pl.tail;

	// Unreferenced objects are garbage collected by the system. There's no need
	// to manage a list of them.

	//if (currentFn->UsesNew) {
	//	if (cpu.SupportsPush)
	//		GenerateMonadic(op_push, 0, makereg(regFirstArg));
	//	else {
	//		GenerateTriadic(op_sub, 0, makereg(regSP), makereg(regSP), MakeImmediate(8));
	//		GenerateDiadic(op_std, 0, makereg(regFirstArg), MakeIndirect(regSP));
	//	}
	//	GenerateDiadic(op_lea, 0, makereg(regFirstArg), MakeIndexed(-cpu.sizeOfWord, regFP));
	//	GenerateMonadic(op_call, 0, MakeStringAsNameConst("__AddGarbage"));
	//	GenerateDiadic(op_ldd, 0, makereg(regFirstArg), MakeIndirect(regSP));
	//	GenerateTriadic(op_add, 0, makereg(regSP), makereg(regSP), MakeImmediate(8));
	//}

	// Unlock any semaphores that may have been set
	for (nn = lastsph - 1; nn >= 0; nn--)
		GenerateStore(makereg(0), MakeStringAsNameConst(semaphores[nn], dataseg), 1);

	// Restore fp registers used as register variables.
	//if (fpsave_mask->NumMember()) {
	//	cnt2 = cnt = (fpsave_mask->NumMember() - 1)*sizeOfFP;
	//	fpsave_mask->resetPtr();
	//	for (nn = fpsave_mask->lastMember(); nn >= 1; nn = fpsave_mask->prevMember()) {
	//		GenerateDiadic(op_lf, 'd', makefpreg(nregs - 1 - nn), MakeIndexed(cnt2 - cnt, regSP));
	//		cnt -= cpu.sizeOfWord;
	//	}
	//	GenerateTriadic(op_add, 0, makereg(regSP), makereg(regSP), MakeImmediate(cnt2 + sizeOfFP));
	//}
	if (func->IsNocall) {
		RestoreRegisterVars(func);
		if (func->epilog) {
			func->epilog->Generate();
			return;
		}
		return;
	}
	toAdd = 0;
	if (!cpu.SupportsLeave) {
		RestoreRegisterVars(func);
		func->UnlinkStack(0);
		toAdd = func->has_return_block ? compiler.GetReturnBlockSize() : 0;
	}
	if (!func->alstk) {
		// The size of the return block is included in the link instruction, so the
		// unlink instruction will reverse the allocation.
		if (cpu.SupportsLink)
			toAdd = 0;
		else if (cpu.SupportsLeave)
			toAdd = 0;
	}
	//else if (currentFn->IsLeaf)
	//	toAdd = 0;

	if (func->epilog) {
		func->epilog->Generate();
		return;
	}

	// Local variables and the return block must be deallocated before the return instruction.
	// The return address is between these and the parameters. Parameters can be deallocated
	// during the return. For leaf routines, the return address is not present, so it is 
	// safe to combine the de-allocations.
	//if (!currentFn->IsLeaf) {
	//	GenerateTriadic(op_add, 0, makereg(regSP), makereg(regSP), MakeImmediate(toAdd));
	//	toAdd = 0;
	//}

	// If Pascal calling convention remove parameters from stack by adding to stack pointer
	// based on the number of parameters. However if a non-auto register parameter is
	// present, then don't add to the stack pointer for it. (Remove the previous add effect).
	// Also, do not add to the stack pointer for the ellipsis parameter.
	/*
	if (IsPascal) {
		TypeArray *ta;
		int nn;
		ta = GetProtoTypes();
		for (nn = 0; nn < ta->length; nn++) {
			switch (ta->types[nn]) {
			case bt_float:
				if (ta->preg[nn] && (ta->preg[nn] & 0x8000) == 0)
					;
				else
					toAdd += sizeOfFP;
				break;
			case bt_quad:
				if (ta->preg[nn] && (ta->preg[nn] & 0x8000) == 0)
					;
				else
					toAdd += cpu.sizeOfFPQ;
				break;
			case bt_double:
				if (ta->preg[nn] && (ta->preg[nn] & 0x8000) == 0)
					;
				else
					toAdd += cpu.sizeOfFPD;
				break;
			case bt_posit:
				if (ta->preg[nn] && (ta->preg[nn] & 0x8000) == 0)
					;
				else
					toAdd += cpu.sizeOfPosit;
				break;
			case bt_ellipsis:
				break;
			default:
				if (ta->preg[nn] && (ta->preg[nn] & 0x8000) == 0)
					;
				else
					toAdd += cpu.sizeOfWord;
			}
		}
	}
	*/
	if (func->IsPascal)
		toAdd += func->arg_space;

	//	if (toAdd != 0)
	//		GenerateTriadic(op_add,0,makereg(regSP),makereg(regSP),MakeImmediate(toAdd));
	// Generate the return instruction. For the Pascal calling convention pop the parameters
	// from the stack.
	if (func->IsInterrupt) {
		//RestoreRegisterSet(sym);
		GenerateAddOnto(makereg(regSP), MakeImmediate(toAdd));
		GenerateInterruptLoad(func);
		GenerateInterruptReturn(func);
		return;
	}

	if (!func->IsInline) {
		if (cpu.SupportsLeave) {
			if (func->arg_space < 8338600LL)
				func->UnlinkStack(toAdd);
			else {
				GenerateMove(makereg(regSP), makereg(regFP));
				GenerateLoad(makereg(regFP), MakeIndirect(regSP), cpu.sizeOfWord, cpu.sizeOfWord);
				ap = GetTempRegister();
				GenerateLoad(ap, MakeIndexed(2 * cpu.sizeOfWord, regFP), cpu.sizeOfWord, cpu.sizeOfWord);
				GenerateTriadic(op_csrrw, 0, makereg(regZero), ap, MakeImmediate(0x3102));
				ReleaseTempRegister(ap);
				GenerateAddOnto(makereg(regSP), MakeImmediate(toAdd));
			}
		}
		else {
			//RestoreRegisterVars(func);
			if (toAdd > 0) {
				cg.GenerateReturnAndDeallocate(toAdd);
				toAdd = 0;
			}
			else
				GenerateReturnInsn();
		}
	}
	else {
//		RestoreRegisterVars(func);
		GenerateAddOnto(makereg(regSP), MakeImmediate(toAdd));
	}
}


/* Generate code for an immediate 'add' operation. Assumes a constant that is
* too large will need to be loaded into a register. Uses a temp register as a
* result.
*
* Parameters:
*		dst (input) pointer to the destination operand
*		src1 (input) pointer to the first source operand
*		src2 (input) pointer to the immediate operand
* Return:
*		a pointer to the destination operand.
*/
Operand* CodeGenerator::GenerateAddImmediate(Operand* dst, Operand* src1, Operand* src2)
{
	Operand* ap5;

	// Check if an immediate value will fit into the immediate field. If
	// not it needs to be loaded into a register.
	ap5 = nullptr;
	if (src2->offset) {
		if (!src2->offset->i128.IsNBit(cpu.RIimmSize)) {
			ap5 = GetTempRegister();
			cg.GenerateLoadConst(src2, ap5);
		}
	}
	GenerateTriadic(op_add, 0, dst, src1, ap5 ? ap5 : MakeImmediate(src2->offset->i));
	if (ap5)
		ReleaseTempRegister(ap5);
	return (dst);
}

Operand* CodeGenerator::GenerateSubtractImmediate(Operand* dst, Operand* src1, Operand* src2)
{
	Operand* ap5;

	// Check if an immediate value will fit into the immediate field. If
	// not it needs to be loaded into a register.
	ap5 = nullptr;
	if (src2->offset) {
		if (!src2->offset->i128.IsNBit(cpu.RIimmSize)) {
			ap5 = GetTempRegister();
			cg.GenerateLoadConst(src2, ap5);
		}
	}
	GenerateTriadic(op_sub, 0, dst, src1, ap5 ? ap5 : MakeImmediate(src2->offset->i));
	if (ap5)
		ReleaseTempRegister(ap5);
	return (dst);
}

/* Generate code for an immediate 'and' operation. Assumes a constant that is
* too large will need to be loaded into a register. Uses a temp register as a
* result.
*
* Parameters:
*		dst (input) pointer to the destination operand
*		src1 (input) pointer to the first source operand
*		src2 (input) pointer to the immediate operand
* Return:
*		a pointer to the destination operand.
*/
Operand* CodeGenerator::GenerateAndImmediate(Operand* dst, Operand* src1, Operand* src2)
{
	Operand* ap5;

	// Check if an immediate value will fit into the immediate field. If
	// not it needs to be loaded into a register.
	ap5 = nullptr;
	if (src2->offset) {
		if (!src2->offset->i128.IsNBit(cpu.RIimmSize)) {
			ap5 = GetTempRegister();
			cg.GenerateLoadConst(src2, ap5);
		}
	}
	GenerateTriadic(op_and, 0, dst, src1, ap5 ? ap5 : MakeImmediate(src2->offset->i));
	if (ap5)
		ReleaseTempRegister(ap5);
	return (dst);
}

/* Generate code for an immediate 'or' operation. Assumes a constant that is
* too large will need to be loaded into a register. Uses a temp register as a
* result.
*
* Parameters:
*		dst (input) pointer to the destination operand
*		src1 (input) pointer to the first source operand
*		src2 (input) pointer to the immediate operand
* Return:
*		a pointer to the destination operand.
*/
Operand* CodeGenerator::GenerateOrImmediate(Operand* dst, Operand* src1, Operand* src2)
{
	Operand* ap5;

	// Check if an immediate value will fit into the immediate field. If
	// not it needs to be loaded into a register.
	ap5 = nullptr;
	if (src2->offset) {
		if (!src2->offset->i128.IsNBit(cpu.RIimmSize)) {
			ap5 = GetTempRegister();
			cg.GenerateLoadConst(src2, ap5);
		}
	}
	GenerateTriadic(op_or, 0, dst, src1, ap5 ? ap5 : MakeImmediate(src2->offset->i));
	if (ap5)
		ReleaseTempRegister(ap5);
	return (dst);
}

/* Generate code for an immediate 'eor' operation. Assumes a constant that is
* too large will need to be loaded into a register. Uses a temp register as a
* result.
*
* Parameters:
*		dst (input) pointer to the destination operand
*		src1 (input) pointer to the first source operand
*		src2 (input) pointer to the immediate operand
* Return:
*		a pointer to the destination operand.
*/
Operand* CodeGenerator::GenerateEorImmediate(Operand* dst, Operand* src1, Operand* src2)
{
	Operand* ap5;

	// Check if an immediate value will fit into the immediate field. If
	// not it needs to be loaded into a register.
	ap5 = nullptr;
	if (src2->offset) {
		if (!src2->offset->i128.IsNBit(cpu.RIimmSize)) {
			ap5 = GetTempRegister();
			cg.GenerateLoadConst(src2, ap5);
		}
	}
	GenerateTriadic(op_eor, 0, dst, src1, ap5 ? ap5 : MakeImmediate(src2->offset->i));
	if (ap5)
		ReleaseTempRegister(ap5);
	return (dst);
}

/* Generates the code for a binary operation using posit arithmetic.
* 
* Parameters:
*		node: (input) pointer to expression tree
*		flags: (input) legal addressing modes
*		size: (input) the operation size
*		op:	(input) the operation to perform
* Returns:
*		pointer to operand containing the result
*/
Operand* CodeGenerator::GenerateBinaryPosit(ENODE* node, int flags, int64_t size, e_op op)
{
	Operand* ap1, *ap2, * ap3;
	bool dup = false;

	ap1 = nullptr;
	ap2 = nullptr;
	ap3 = GetTempPositRegister();
	if (node->IsEqual(node->p[0], node->p[1]))
		dup = !opt_nocgo;
	ap1 = cg.GenerateExpression(node->p[0], am_preg, size, 0);
	if (!dup)
		ap2 = cg.GenerateExpression(node->p[1], am_preg, size, 1);
	// Generate a convert operation ?
	if (!dup) {
		if (ap1->fpsize() != ap2->fpsize()) {
			if (ap2->fpsize() == 's')
				// ToDo: fix this conversion.
				GenerateDiadic(op_fcvtsq, 0, ap2, ap2);
		}
	}
	if (dup)
		GenerateTriadic(op, 0, ap3, ap1, ap1);
	else
		GenerateTriadic(op, 0, ap3, ap1, ap2);
	ap3->type = ap1->type;
	if (ap2)
		ReleaseTempRegister(ap2);
	if (ap1)
		ReleaseTempRegister(ap1);
	return(ap3);
}

/* Generates the code for a binary operation.
* Try and keep this routine generic so it should not need to be present in 
* derived classes.
*
* Parameters:
*		node: (input) pointer to expression tree
*		flags: (input) legal addressing modes
*		size: (input) the operation size
*		op:	(input) the operation to perform
* Returns:
*		pointer to operand containing the result
*/
Operand* CodeGenerator::GenerateBinary(ENODE* node, int flags, int64_t size, int op)
{
	Operand* ap1 = nullptr, * ap2 = nullptr, * ap3, * ap4, *ap5;
	bool dup = false;

	if (node->IsFloatType())
		return (cg.GenerateBinaryFloat(node, flags, size, (e_op)op));
	else if (node->IsPositType())
		return (cg.GenerateBinaryPosit(node, flags, size, (e_op)op));
	else if (op == op_vex) {
		ap3 = GetTempRegister();
		ap1 = cg.GenerateExpression(node->p[0], am_reg, size, 0);
		ap2 = cg.GenerateExpression(node->p[1], am_reg, size, 1);
		GenerateTriadic(op, 0, ap3, ap1, ap2);
	}
	else if (node->IsVectorType()) {
		ap3 = GetTempVectorRegister();
		if (ENODE::IsEqual(node->p[0], node->p[1]) && !opt_nocgo) {
			ap1 = cg.GenerateExpression(node->p[0], am_vreg, size, 0);
			//ap2 = cg.GenerateExpression(node->vmask, am_vmreg, size, 1);
			Generate4adic(op, 0, ap3, ap1, ap1, makevmreg(node->mask) );
		}
		else {
			ap1 = cg.GenerateExpression(node->p[0], am_vreg|am_reg, size, 0);
			ap2 = cg.GenerateExpression(node->p[1], am_vreg|am_reg, size, 1);
			//ap4 = cg.GenerateExpression(node->vmask, am_vmreg, size, 1);
			Generate4adic(op, ap1->tp->lane_precision, ap3, ap1, ap2, makevmreg(node->mask));
			//ReleaseTempReg(ap4);
		}
		// Generate a convert operation ?
		//if (fpsize(ap1) != fpsize(ap2)) {
		//	if (fpsize(ap2)=='s')
		//		GenerateDiadic(op_fcvtsq, 0, ap2, ap2);
		//}
	}
	else {
		ap3 = GetTempRegister();
		if (ENODE::IsEqual(node->p[0], node->p[1]) && !opt_nocgo) {
			// Duh, subtract operand from itself, result would be zero.
			if (op == op_sub || op==op_subtract || op == op_ptrdif || op == op_eor) {
				GenerateMove(ap3, makereg(0));
				ap3->isConst = true;
				if (ap3->offset == nullptr)
					ap3->offset = makeinode(en_icon, 0);
				else {
					ap3->offset->i128.low = 0;
					ap3->offset->i128.high = 0;
				}
			}
			else {
				ap1 = cg.GenerateExpression(node->p[0], am_reg, size, 0);
				GenerateTriadic(op, 0, ap3, ap1, ap1);
			}
		}
		else {
			ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op)->amclass2, size, 0);
			// modu/ptrdif does not have an immediate mode
			ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op)->amclass3, size, 1);
			if (Instruction::Get(op)->amclass4) {	// op_ptrdif
				ap4 = cg.GenerateExpression(node->p[4], Instruction::Get(op)->amclass4, size, 1);
				Generate4adic(op, 0, ap3, ap1, ap2, ap4);
			}
			else {
				if (ap2->mode == am_imm) {
					// Check if an immediate value will fit into the 24-bit immediate field. If
					// not it needs to be loaded into a register.
					ap5 = nullptr;
					if (op != op_and && op != op_or && op != op_eor && op != op_xor && op != op_add && ap2->offset) {
						if (!ap2->offset->i128.IsNBit(cpu.RIimmSize)) {
							ap5 = GetTempRegister();
							cg.GenerateLoadConst(ap2, ap5);
						}
					}
					switch (op) {
					case op_and:
						GenerateAndImmediate(ap3, ap1, ap2);
						break;
					case op_or:
						GenerateOrImmediate(ap3, ap1, ap2);
						break;
					case op_xor:
					case op_eor:
						GenerateEorImmediate(ap3, ap1, ap2);
						break;
						// If there is a pointer plus a constant we really wanted an address calc.
					case op_add:
						if (ap1->isPtr && ap2->isPtr) {
							GenerateAddImmediate(ap3, ap1, ap2);
							ap3->isPtr = false;
						}
						else if (ap2->isPtr) {
							GenerateLoadAddress(ap3, MakeIndexed(ap2->offset, ap1->preg));
							ap3->isPtr = true;
							//if (!compiler.os_code) {
							//	switch (ap3->segment) {
							//	case tlsseg:		GenerateTriadic(op_base, 0, ap3, ap3, MakeImmediate(8));	break;
							//	case rodataseg:	GenerateTriadic(op_base, 0, ap3, ap3, MakeImmediate(12));	break;
							//	}
							//}
						}
						else {
							GenerateAddImmediate(ap3, ap1, ap2);
						}
						break;
					case op_sub:
						if (ap1->isPtr && ap2->isPtr)
							GenerateTriadic(op, 0, ap3, ap1, ap5 ? ap5 : ap2);
						else if (ap2->isPtr) {
							GenerateLoadAddress(ap3, op == op_subtract || op==op_sub ? compiler.of.MakeNegIndexed(ap2->offset, ap1->preg) : MakeIndexed(ap2->offset, ap1->preg));
							//if (!compiler.os_code) {
							//	switch (ap3->segment) {
							//	case tlsseg:		GenerateTriadic(op_base, 0, ap3, ap3, MakeImmediate(8));	break;
							//	case rodataseg:	GenerateTriadic(op_base, 0, ap3, ap3, MakeImmediate(12));	break;
							//	}
							//}
						}
						else {
							if (op==op_add)
								GenerateAdd(ap3, ap1, ap5 ? ap5 : ap2);
							else if (op == op_sub)
								GenerateSubtract(ap3, ap1, ap5 ? ap5 : ap2);
							else
								GenerateSubtractImmediate(ap3, ap1, ap2);
								//GenerateTriadic(op, 0, ap3, ap1, ap5 ? ap5 : ap2);
						}
						break;
					default:
						if (op == op_add)
							GenerateAdd(ap3, ap1, ap5 ? ap5 : ap2);
						else if (op == op_subtract)
							GenerateSubtract(ap3, ap1, ap5 ? ap5 : ap2);
						else
							GenerateTriadic(op, 0, ap3, ap1, ap5 ? ap5 : ap2);
					}
					if (ap5)
						ReleaseTempRegister(ap5);
				}
				else {
					if (op == op_add)
						GenerateAdd(ap3, ap1, ap2);
					else if (op == op_subtract)
						GenerateSubtract(ap3, ap1, ap2);
					else
						GenerateTriadic(op, 0, ap3, ap1, ap2);
				}
			}
		}
	}
	if (ap2)
		ReleaseTempReg(ap2);
	if (ap1)
		ReleaseTempReg(ap1);
	ap3->MakeLegal(flags, size);
	return (ap3);
}

void CodeGenerator::GenerateReturnAndDeallocate(int64_t amt) {
	GenerateTriadic(op_retd, 0, makereg(regSP), makereg(regSP), MakeImmediate(amt));
}

void CodeGenerator::GenerateLoadDataPointer()
{
	Operand* ap = GetTempRegister();
	//cg.GenerateLoadConst(MakeStringAsNameConst("__data_base", dataseg), ap);
	cg.GenerateLoadAddress(makereg(regGP), MakeStringAsNameConst((char*)"_start_data", dataseg));
	//GenerateTriadic(op_base, 0, makereg(regGP), makereg(regGP), ap);
	ReleaseTempRegister(ap);
}

// Compiler now uses global pointer one addressing for the rodataseg
void CodeGenerator::GenerateLoadRodataPointer()
{
	Operand* ap = GetTempRegister();
	//cg.GenerateLoadConst(MakeStringAsNameConst("__rodata_base", dataseg), ap);
	cg.GenerateLoadAddress(makereg(regGP1), MakeStringAsNameConst((char*)currentFn->sym->name->c_str(), codeseg));
	//cg.GenerateLoadAddress(makereg(regGP1), MakeStringAsNameConst((char *)"_start_rodata", dataseg));
	//if (!compiler.os_code)
	//GenerateTriadic(op_base, 0, makereg(regGP1), makereg(regGP1), ap);
	ReleaseTempRegister(ap);
}

void CodeGenerator::GenerateLoadBssPointer()
{
	Operand* ap = GetTempRegister();
	//cg.GenerateLoadConst(MakeStringAsNameConst("__data_base", dataseg), ap);
	cg.GenerateLoadAddress(makereg(regGP), MakeStringAsNameConst((char*)"_start_bss", bssseg));
	//GenerateTriadic(op_base, 0, makereg(regGP), makereg(regGP), ap);
	ReleaseTempRegister(ap);
}

void CodeGenerator::GenerateSmallDataRegDecl()
{
	switch (syntax) {
	case MOT:
		ofs.printf("\tsdreg\t%d\n", regGP);
		break;
	default:
		ofs.printf("\t.sdreg\t%d\n", regGP);
	}
	/*
	switch (syntax) {
	case MOT:
		ofs.printf("\tsd2reg\t%d\n", regGP1);
		break;
	default:
		ofs.printf("\t.sd2reg\t%d\n", regGP1);
	}
	switch (syntax) {
	case MOT:
		ofs.printf("\tsd3reg\t%d\n", regGP2);
		break;
	default:
		ofs.printf("\t.sd3reg\t%d\n", regGP2);
	}
	*/
}

void CodeGenerator::GenerateSignExtendByte(Operand* tgt, Operand* src)
{
	GenerateDiadic(op_movsxb, 0, tgt, src);
}

void CodeGenerator::GenerateZeroExtendByte(Operand* tgt, Operand* src)
{
	GenerateDiadic(op_movzxb, 0, tgt, src);
}

void CodeGenerator::GenerateSignExtendWyde(Operand* tgt, Operand* src)
{
	GenerateDiadic(op_movsxw, 0, tgt, src);
}

void CodeGenerator::GenerateZeroExtendWyde(Operand* tgt, Operand* src)
{
	GenerateDiadic(op_movzxw, 0, tgt, src);
}

void CodeGenerator::GenerateSignExtendTetra(Operand* tgt, Operand* src)
{
	GenerateDiadic(op_movsxt, 0, tgt, src);
}

void CodeGenerator::GenerateZeroExtendTetra(Operand* tgt, Operand* src)
{
	GenerateDiadic(op_movzxt, 0, tgt, src);
}

void CodeGenerator::GenerateReturnAndDeallocate(Operand* ap1)
{
	GenerateDiadic(op_retd, 0, ap1, MakeImmediate(0));
}

int CodeGenerator::GetSegmentIndexReg(e_sg segment)
{
	switch (segment) {
	case bssseg: return (regGP);
	case dataseg:	return (regGP);
	case rodataseg: return (regGP);
	case tlsseg:	return (regTP);
	case codeseg: return (regGP);
	default:	return (regPP);
	}
	return (regZero);
}

// For a leaf routine don't bother to store the link register.
OCODE* CodeGenerator::GenerateReturnBlock(Function* fn)
{
	Operand* ap;
	int n;
	char buf[300];
	OCODE* ip;

	ip = nullptr;
	fn->alstk = false;
	if (!cpu.SupportsEnter)
		GenerateMonadic(op_hint, 0, MakeImmediate(begin_return_block));
	if (cpu.SupportsEnter)
	{
		if (fn->stkspace < 32767) {
			GenerateMonadic(op_enter, 0, MakeImmediate(-fn->tempbot));
			ip = currentFn->pl.tail;
			//			GenerateMonadic(op_link, 0, MakeImmediate(stkspace));
						//spAdjust = pl.tail;
			fn->alstk = true;
		}
		else {
			GenerateMonadic(op_enter, 0, MakeImmediate(32760));
			ip = currentFn->pl.tail;
			GenerateTriadic(op_sub, 0, makereg(regSP), makereg(regSP), MakeImmediate(-fn->tempbot - 32760));
			//GenerateMonadic(op_link, 0, MakeImmediate(SizeofReturnBlock() * cpu.sizeOfWord));
			fn->alstk = true;
		}
	}
	else if (cpu.SupportsLink) {
		if (fn->stkspace < 32767 - Compiler::GetReturnBlockSize()) {
			GenerateMonadic(op_link, 0, MakeImmediate(Compiler::GetReturnBlockSize() + fn->stkspace));
			//			GenerateMonadic(op_link, 0, MakeImmediate(stkspace));
						//spAdjust = pl.tail;
			fn->alstk = true;
		}
		else {
			GenerateMonadic(op_link, 0, MakeImmediate(32760));
			//GenerateTriadic(op_subtract, 0, makereg(regSP), makereg(regSP), MakeImmediate(Compiler::GetReturnBlockSize() + fn->stkspace - 32760));
			GenerateSubtractImmediate(makereg(regSP), makereg(regSP), MakeImmediate(Compiler::GetReturnBlockSize() + fn->stkspace - 32760));
			//GenerateMonadic(op_link, 0, MakeImmediate(SizeofReturnBlock() * cpu.sizeOfWord));
			fn->alstk = true;
		}
	}
	else {
		GenerateSubtractImmediate(makereg(regSP), makereg(regSP), MakeImmediate(Compiler::GetReturnBlockSize()));
		//GenerateTriadic(op_sub, 0, makereg(regSP), makereg(regSP), MakeImmediate(Compiler::GetReturnBlockSize()));
		cg.GenerateStore(makereg(regFP), MakeIndirect(regSP), cpu.sizeOfWord);
		cg.GenerateMove(makereg(regFP), makereg(regSP));
		cg.GenerateStore(makereg(regLR), cg.MakeIndexed(cpu.sizeOfWord * 1, regFP), cpu.sizeOfWord);	// Store link register on stack
		GenerateSubtractImmediate(makereg(regSP), makereg(regSP), MakeImmediate(fn->stkspace));
		//GenerateTriadic(op_sub, 0, makereg(regSP), makereg(regSP), MakeImmediate(fn->stkspace));
		fn->alstk = true;
		fn->has_return_block = true;
	}
	// Put this marker here so that storing the link register relative to the
	// frame pointer counts as a frame pointer reference.
	if (!cpu.SupportsEnter)
		GenerateMonadic(op_hint, 0, MakeImmediate(end_return_block));
	//	GenerateTriadic(op_stdp, 0, makereg(regFP), makereg(regZero), MakeIndirect(regSP));
	n = 0;
	if (!currentFn->IsLeaf && fn->doesJAL) {
		n |= 2;
		/*
		if (alstk) {
			GenerateDiadic(op_sto, 0, makereg(regLR), MakeIndexed(1 * cpu.sizeOfWord + stkspace, regSP));
		}
		else
		*/
		if (!cpu.SupportsEnter) {
			//if (IsFar)
			//	GenerateMonadic(op_di, 0, MakeImmediate(2));
			//ap = GetTempRegister();
			//GenerateTriadic(op_csrrd, 0, ap, makereg(regZero), MakeImmediate(0x3102));
			//GenerateDiadic(op_mflk, 0, makereg(regLR), ap);
			//cg.GenerateStore(makereg(regLR), MakeIndexed(2 * cpu.sizeOfWord, regFP), cpu.sizeOfWord);
			//ReleaseTempRegister(ap);
			if (fn->IsFar) {
				ap = GetTempRegister();
				GenerateTriadic(op_csrrd, 0, ap, makereg(regZero), MakeImmediate(0x3103));
				cg.GenerateStore(ap, cg.MakeIndexed(3 * cpu.sizeOfWord, regFP), cpu.sizeOfWord);
				ReleaseTempRegister(ap);
			}
		}
	}
	/*
	switch (n) {
	case 0:	break;
	case 1:	GenerateDiadic(op_std, 0, makereg(regXLR), MakeIndexed(2 * cpu.sizeOfWord, regSP)); break;
	case 2:	GenerateDiadic(op_std, 0, makereg(regLR), MakeIndexed(3 * cpu.sizeOfWord, regSP)); break;
	case 3:	GenerateTriadic(op_stdp, 0, makereg(regXLR), makereg(regLR), MakeIndexed(2 * cpu.sizeOfWord, regSP)); break;
	}
	*/
	retlab = nextlabel++;
	ap = MakeDataLabel(retlab, regZero);
	ap->mode = am_imm;
	//if (!cpu.SupportsLink)
	//	GenerateDiadic(op_mov, 0, makereg(regFP), makereg(regSP));
	//if (!alstk) {
	//	GenerateTriadic(op_sub, 0, makereg(regSP), makereg(regSP), MakeImmediate(stkspace));
		//spAdjust = pl.tail;
//	}
	// Store the catch handler address at 16[$FP]
	if (compiler.exceptions) {
		ap = GetTempRegister();
		sprintf_s(buf, sizeof(buf), ".%05lld", fn->defCatchLabel);
		DataLabels[fn->defCatchLabel]++;
		fn->defCatchLabelPatchPoint = currentFn->pl.tail;
		GenerateDiadic(cpu.ldi_op, 0, ap, cg.MakeStringAsNameConst(buf, codeseg));
		if (fn->IsFar)
			GenerateMonadic(op_di, 0, MakeImmediate(2));
		cg.GenerateStore(ap, cg.MakeIndexed((int64_t)32, regFP), cpu.sizeOfWord);
		ReleaseTempRegister(ap);
		if (fn->IsFar) {
			ap = GetTempRegister();
			GenerateTriadic(op_csrrd, 0, ap, makereg(regZero), MakeImmediate(0x311F));	// CS
			cg.GenerateStore(ap, cg.MakeIndexed((int64_t)40, regFP), cpu.sizeOfWord);
			ReleaseTempRegister(ap);
		}
		//		GenerateDiadic(cpu.mov_op, 0, makereg(regAFP), makereg(regFP));
		GenerateMonadic(op_bex, 0, cg.MakeCodeLabel(currentFn->defCatchLabel));
	}
	fn->tryCount = 0;
	return (ip);
}

Operand* CodeGenerator::GenerateLand(ENODE* node, int flags, int op, bool safe)
{
	Operand* ap1, * ap2;
	int lab0, lab1;

	if (safe)
		return (cg.GenerateSafeLand(node, flags, op));
	lab0 = nextlabel++;
	lab1 = nextlabel++;
	ap1 = GetTempRegister();
	ap2 = cg.GenerateExpression(node, flags, cpu.sizeOfWord, 1);
	ap1 = cg.MakeBoolean(ap2);
	ReleaseTempReg(ap2);
	/*
	GenerateDiadic(op_loadi, 0, ap1, MakeImmediate(1));
	cg.GenerateFalseJump(this, lab0, 0);
	GenerateDiadic(op_loadi, 0, ap1, MakeImmediate(0));
	GenerateLabel(lab0);
	*/
	ap1->MakeLegal(flags, 8);
	ap1->isBool = true;
	return (ap1);
}

/* Under construction
* Sign extend from an arbitrary bit position using only primitive instructions.
*
*/
void CodeGenerator::GenerateSignBitExtend(Operand* dst, Operand* bit)
{
	int64_t mask;
	Operand* ap;

	bit->MakeLegal(am_reg|am_imm, bit->tp->size);
	if (bit->isConst) {
		mask = 2LL << bit->offset->i128.low;
		if (mask < 0xffffffLL) {
			if (dst->mode == am_reg) {
				GenerateTriadic(op_and, 0, dst, dst, MakeImmediate(~mask));	// clear the bits beyond the sign bit
				GenerateAddImmediate(dst, dst, MakeImmediate(mask >> 1));
				GenerateTriadic(op_eor, 0, dst, dst, MakeImmediate(~mask));
			}
			else {
				Operand* ap2 = GetTempRegister();

				GenerateLoad(ap2, dst, dst->tp->size, dst->tp->size);
				GenerateTriadic(op_and, 0, dst, dst, MakeImmediate(~mask));	// clear the bits beyond the sign bit
				GenerateAddImmediate(dst, dst, MakeImmediate(mask >> 1));
				GenerateTriadic(op_eor, 0, dst, dst, MakeImmediate(~mask));
				GenerateStore(dst, ap2, dst->tp->size);
				ReleaseTempRegister(ap2);
			}
		}
		else {
			Operand* ap2;
			ap2 = GetTempRegister();
			GenerateLoadConst(MakeImmediate(mask), ap2);

			if (bit->mode == am_reg) {
				Operand* ap2, * ap3;
				ap = GetTempRegister();
				ap2 = GetTempRegister();
				ap3 = GetTempRegister();
				GenerateLoadConst(MakeImmediate(2LL), ap2);
				GenerateTriadic(op_shl, 0, ap2, ap2, bit);
				GenerateTriadic(op_eor, 0, ap3, ap2, MakeImmediate(-1LL));
				GenerateLoad(ap, bit, bit->tp->size, bit->tp->size);
			}
			ap = GetTempRegister();
			GenerateAdd(dst, dst, ap);
		}
	}
	else {
	}
}

Operand* CodeGenerator::GenerateAdd(Operand* dst, Operand* src1, Operand* src2)
{
	GenerateTriadic(op_add, 0, dst, src1, src2);
	return (dst);
}

Operand* CodeGenerator::GenerateSubtract(Operand* dst, Operand* src1, Operand* src2)
{
	GenerateTriadic(op_sub, 0, dst, src1, src2);
	return (dst);
}

Operand* CodeGenerator::GenerateAnd(Operand* dst, Operand* src1, Operand* src2)
{
	GenerateTriadic(op_and, 0, dst, src1, src2);
	return (dst);
}

Operand* CodeGenerator::GenerateOr(Operand* dst, Operand* src1, Operand* src2)
{
	GenerateTriadic(op_or, 0, dst, src1, src2);
	return (dst);
}

/* Generate code for a shift operation. A warning message is displayed if the
* shift is known to be by too many bits.
*
* Parameters:
*		flags (input) the legal address modes to use for the result.
*		size (input) the size of the result in bytes.
*		op (input) the shift opcode.
*
* Returns:
*		an operand referencing the shift value.
*/
// ToDo: ShiftBitfield
Operand* CodeGenerator::GenerateShift(ENODE*node, int flags, int64_t size, int op)
{
	Operand* ap1, * ap2, * ap3;
	Int128 val;

	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], am_reg, size, 0);
	ap2 = cg.GenerateExpression(node->p[1], am_reg | am_ui6, cpu.sizeOfWord, 1);
	if (ap2->GetConstValue(&val))
	{
		Int128 sz, eight;
		eight = Int128::Convert(8LL);
		sz = Int128::Convert(TYP::GetSize(ap1->tp->type));
		Int128::Mul(&sz, &sz, &eight);
		if (Int128::IsGT(&val, &sz))
			error(ERR_SHIFT_TOOMANYBITS);
	}
	GenerateTriadic(op, size == cpu.sizeOfWord ? 0 : (int)size, ap3, ap1, ap2);
	// Rotates automatically sign extend
	if ((op == op_rol || op == op_ror) && ap2->isUnsigned && !ap1->tp->IsVectorType())
		switch (size) {
		case 1:	ap1 = ap2->GenerateBitfieldClear(8, cpu.sizeOfWord * 8 - 9); break;
		case 2:	ap1 = ap2->GenerateBitfieldClear(16, cpu.sizeOfWord * 8 - 17); break;
		case 4:
			if (cpu.sizeOfWord > 4)
				ap1 = ap2->GenerateBitfieldClear(32, cpu.sizeOfWord * 8 - 33); break;
		case 8:
			if (cpu.sizeOfWord > 8)
				ap1 = ap2->GenerateBitfieldClear(64, cpu.sizeOfWord * 8 - 65); break;
		default:;
		}
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	ap3->MakeLegal(flags, size);
	return (ap3);
}

// ----------------------------------------------------------------------------
// Generate code to evaluate an index node (^+) and return the addressing mode
// of the result. This routine takes no flags since it always returns either
// am_ind or am_indx.
//
// No reason to ReleaseTempReg() because the registers used are transported
// forward.
// 
// If we have nested indexes we really want the value from the inner index
// array to use. Hence it is loaded.
// ----------------------------------------------------------------------------
Operand* CodeGenerator::GenerateIndex(ENODE* node, bool neg)
{
	Operand* ap1, * ap2;
	static int ndxlvl = 0;

	ndxlvl++;
	//	if ((p[0]->nodetype == en_type || p[0]->nodetype == en_regvar)
	//		&& (p[1]->nodetype == en_type || p[1]->nodetype == en_regvar))
		/* Both nodes are registers? */
	if (node->p[0]->nodetype == en_regvar && node->p[1]->nodetype == en_regvar) {
		ap1 = node->GenerateRegRegIndex();
		if (ndxlvl > 1) {
			GenerateLoad(makereg(ap1->preg), ap1, node->esize, node->esize);
			ap1->mode = am_reg;
		}
		ndxlvl--;
		return (ap1);
	}

	GenerateHint(8);
	//	GenerateHint(begin_index);
	ap1 = cg.GenerateExpression(node->p[0], am_reg | am_imm, cpu.sizeOfInt, 1);
	if (ap1->mode == am_imm) {
		ap1 = node->GenerateImmExprIndex(ap1, neg);
		if (ndxlvl > 1) {
			GenerateLoad(makereg(ap1->preg), ap1, node->esize, node->esize);
			ap1->mode = am_reg;
		}
		ndxlvl--;
		return (ap1);
	}

	ap2 = cg.GenerateExpression(node->p[1], am_all, cpu.sizeOfWord, 1);   /* get right op */
	//	GenerateHint(end_index);
	GenerateHint(9);

	// Do we have reg+imm? If so make am_indx
	if (ap2->mode == am_imm) {
		ap1 = node->GenerateRegImmIndex(ap1, ap2, neg);
		if (ndxlvl > 1) {
			GenerateLoad(makereg(ap1->preg), ap1, node->esize, node->esize);
			ap1->mode = am_reg;
		}
		ndxlvl--;
		return (ap1);
	}

	if (ap2->mode == am_ind && ap1->mode == am_reg) {
		if (cpu.SupportsIndexed && ndxlvl == 1) {
			ap2->mode = am_indx2;
			ap2->sreg = ap1->preg;
			ap2->deep2 = ap1->deep;
		}
		else {
			//GenerateTriadic(op_mulu, 0, ap2, ap2, MakeImmediate(this->esize));
			cg.GenerateAdd(ap2, ap1, ap2);
			ap2->mode = am_indx;
			ap2->deep2 = ap1->deep;
			ap2->offset = makeinode(en_icon, 0);
			if (ndxlvl > 1) {
				GenerateLoad(makereg(ap2->preg), ap2, node->esize, node->esize);
				ap2->mode = am_reg;
			}
		}
		ap2->scale = 1;
		ndxlvl--;
		return (ap2);
	}

	// Direct plus index register equals register indirect addressing.
	if (ap2->mode == am_direct && ap1->mode == am_reg) {
		ap2->mode = am_indx;
		ap2->preg = ap1->preg;
		ap2->deep = ap1->deep;
		if (ndxlvl > 1) {
			GenerateLoad(makereg(ap2->preg), ap2, node->esize, node->esize);
			ap2->mode = am_reg;
		}
		ndxlvl--;
		return (ap2);
	}

	// ap1->mode must be am_reg
	ap2->MakeLegal(am_reg, ap2->tp ? ap2->tp->size : cpu.sizeOfWord);
	if (cpu.SupportsIndexed && ndxlvl == 1) {
		ap1->mode = am_indx2;            /* make indexed */
		ap1->sreg = ap2->preg;
		ap1->deep2 = ap2->deep;
		ap1->offset = makeinode(en_icon, 0);
		ap1->scale = node->scale;
	}
	else {
		ap1->mode = am_indx2;            /* make indexed */
		ap1->sreg = ap2->preg;
		ap1->deep2 = ap2->deep;
		ap1->offset = makeinode(en_icon, 0);
		//ap1->scale = 1;
		/*
		ap4 = GetTempRegister();
		ap3 = GetTempRegister();
		ap2->MakeLegal(am_reg, this->esize);
		GenerateTriadic(op_mulu, 0, ap3, ap2, MakeImmediate(this->esize));
		GenerateTriadic(op_add, 0, ap4, ap1, ap3);
		ReleaseTempRegister(ap3);
		*/
		if (ndxlvl > 1) {
			GenerateLoad(makereg(ap1->preg), ap1, node->esize, node->esize);
			ap1->mode = am_reg;
		}
		//		ap1->mode = am_indx;            /* make indexed */
		//		ap1->deep2 = ap2->deep;
		//		ap1->offset = makeinode(en_icon, 0);
		ndxlvl--;
		return (ap1);                     /* return indexed */
	}
	ndxlvl--;
	return (ap1);                     /* return indexed */
}

void CodeGenerator::GenerateUnlinkStack(Function* func, int64_t amt)
{
	Operand* ap;
	/* auto news are garbage collected
	if (hasAutonew) {
		GenerateMonadic(op_call, 0, MakeStringAsNameConst("__autodel",codeseg));
		GenerateMonadic(op_bex, 0, MakeDataLabel(throwlab));
	}
	*/
	if (!cpu.SupportsLeave)
		GenerateMonadic(op_hint, 0, MakeImmediate(begin_stack_unlink));
	if (cpu.SupportsLeave) {
	}
	else if (!func->IsLeaf) {
		//		if (doesJAL) {	// ??? Not a leaf, so it must be transferring control
		if (func->alstk) {
			ap = GetTempRegister();
			cg.GenerateLoad(ap, MakeIndexed(1 * cpu.sizeOfWord, regFP), cpu.sizeOfWord, cpu.sizeOfWord);
			GenerateDiadic(op_move, 0, makereg(regLR), ap);
			ReleaseTempRegister(ap);
			//GenerateTriadic(op_csrrw, 0, makereg(regZero), ap, MakeImmediate(0x3102));
			if (func->IsFar) {
				ap = GetTempRegister();
				cg.GenerateLoad(ap, MakeIndexed(3 * cpu.sizeOfWord, regFP), cpu.sizeOfWord, cpu.sizeOfWord);
				GenerateTriadic(op_csrrw, 0, makereg(regZero), ap, MakeImmediate(0x3103));
				ReleaseTempRegister(ap);
			}
			cg.GenerateMove(makereg(regSP), makereg(regFP));
			cg.GenerateLoad(makereg(regFP), MakeIndirect(regSP), cpu.sizeOfWord, cpu.sizeOfWord);
		}
		//		}
	}
	// Else leaf routine, reverse any stack allocation but do not pop link register
	else {
		if (func->alstk) {
			cg.GenerateMove(makereg(regSP), makereg(regFP));
			cg.GenerateLoad(makereg(regFP), MakeIndirect(regSP), cpu.sizeOfWord, cpu.sizeOfWord);
		}
	}
	cg.GenerateUnlink(amt);
	/*
	if (cpu.SupportsLeave) {
	}
	else if (!IsLeaf && doesJAL) {
		if (alstk) {
			cg.GenerateLoad(makereg(regLR), MakeIndexed(2 * cpu.sizeOfWord, regFP), cpu.sizeOfWord, cpu.sizeOfWord);
			//GenerateTriadic(op_csrrw, 0, makereg(regZero), ap, MakeImmediate(0x3102));
			if (IsFar) {
				ap = GetTempRegister();
				cg.GenerateLoad(ap, MakeIndexed(3 * cpu.sizeOfWord, regFP), cpu.sizeOfWord, cpu.sizeOfWord);
				GenerateTriadic(op_csrrw, 0, makereg(regZero), ap, MakeImmediate(0x3103));
				ReleaseTempRegister(ap);
			}
			cg.GenerateMove(makereg(regSP), makereg(regFP));
			cg.GenerateLoad(makereg(regFP), MakeIndirect(regSP), cpu.sizeOfWord, cpu.sizeOfWord);
		}
	}
	//	GenerateTriadic(op_add,0,makereg(regSP),makereg(regSP),MakeImmediate(3*cpu.sizeOfWord));
	*/
	if (!cpu.SupportsLeave)
		GenerateMonadic(op_hint, 0, MakeImmediate(end_stack_unlink));
}

int64_t CodeGenerator::GetSavedRegisterList(CSet* rmask)
{
	int nn;
	int64_t mask = 0;

	if (rmask->NumMember()) {
		for (nn = 0; nn < nregs; nn++)
			if (rmask->isMember(nn))
				mask = mask | (1LL << nn);
	}
	return (mask);
}

Operand* CodeGenerator::PatchEnter(OCODE* pe, CSet* rmask)
{
	if (pe)
		return(pe->oper1 = MakeImmediate((int64_t)rmask->NumMember()));
	return (nullptr);
}

