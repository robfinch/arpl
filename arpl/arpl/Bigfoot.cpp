// ============================================================================
//        __
//   \\__/ o\    (C) 2023-2024  Robert Finch, Waterloo
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

extern TYP              stdfunc;

extern void DumpCSETable();
extern void scan(Statement *);
extern void GenerateComment(char *);
int TempFPInvalidate();
int TempInvalidate();
void TempRevalidate(int,int);
void TempFPRevalidate(int);
void ReleaseTempRegister(Operand *ap);
Operand *GetTempRegister();
Operand* GetTempCrRegister();
extern void GenLoad(Operand *ap1, Operand *ap3, int ssize, int size);

BigfootCPU::BigfootCPU() {
	sizeOfWord = 8;
	sizeOfPtr = 8;
	sizeOfFP = 8;
	sizeOfFPS = 4;
	sizeOfFPT = 12;
	sizeOfFPD = 8;
	sizeOfFPQ = 16;
	sizeOfInt = 8;
	sizeOfDecimal = 16;
	sizeOfPosit = 8;
	RIimmSize = 24;
}

void BigfootCodeGenerator::banner()
{
	printf("Bigfoot Code Generator v0.02\n");
};


void BigfootCodeGenerator::SignExtendBitfield(Operand* ap3, uint64_t mask)
{
	Operand* ap2;
	uint64_t umask;

	umask = 0x8000000000000000LL | ~(mask >> 1);
	ap2 = GetTempRegister();
	GenerateDiadic(op_loadi, 0, ap2, cg.MakeImmediate((int64_t)umask));
	GenerateTriadic(op_add, 0, ap3, ap3, ap2);
	GenerateTriadic(op_xor, 0, ap3, ap3, ap2);
	ReleaseTempRegister(ap2);
}

// Convert a value to a Boolean.
Operand* BigfootCodeGenerator::MakeBoolean(Operand* ap)
{
	Operand* ap1,* ap2;
	OCODE* ip;

	ap2 = GetTempRegister();
	ap1 = GetTempCrRegister();
	ip = currentFn->pl.tail;
	if (ip->opcode & 0x8000)
		return (ap1);
	GenerateTriadic(op_cmp, 0, ap1, ap, MakeImmediate(0));
	GenerateDiadic(op_mov, 0, ap2, ap1);
	GenerateTriadic(op_lsr , 0, ap2, ap2, MakeImmediate((ap1->preg & 7)*8));
	GenerateTriadic(op_and, 0, ap2, ap2, MakeImmediate(1));
	ReleaseTempReg(ap1);
	ap2->isBool = true;
	return (ap2);
}

void BigfootCodeGenerator::GenerateLea(Operand* ap1, Operand* ap2)
{
	switch (ap2->mode) {
	case am_reg:
		GenerateMove(ap1, ap2);
		break;
	default:
		GenerateLoadAddress(ap1, ap2);
		//if (!compiler.os_code) {
		//	switch (ap1->segment) {
		//	case tlsseg:		GenerateTriadic(op_base, 0, ap1, ap1, MakeImmediate(8));	break;
		//	case rodataseg:	GenerateTriadic(op_base, 0, ap1, ap1, MakeImmediate(12));	break;
		//	}
		//}
	}
}

Operand* BigfootCodeGenerator::GenerateSafeLand(ENODE *node, int flags, int op)
{
	Operand* ap1, * ap2, * ap4, * ap5;
	int lab0;

	lab0 = nextlabel++;

	ap1 = GenerateExpression(node->p[0], am_reg|am_creg, node->p[0]->GetNaturalSize(), 0);
	ap2 = GenerateExpression(node->p[1], am_reg|am_creg, node->p[1]->GetNaturalSize(), 1);

	if (!ap1->isBool)
		ap4 = MakeBoolean(ap1);
	else
		ap4 = ap1;

	if (!ap2->isBool)
		ap5 = MakeBoolean(ap2);
	else
		ap5 = ap2;

	GenerateTriadic(op_and, 0, ap4, ap4, ap5);
	ReleaseTempReg(ap2);
	//ap2->MakeLegal(flags, cpu.sizeOfWord);
	ap1->isBool = true;
	return (ap1);
}


void BigfootCodeGenerator::GenerateBitfieldInsert(Operand* ap1, Operand* ap2, int offset, int width)
{
	ap1->MakeLegal(am_reg, cpu.sizeOfWord);
	ap2->MakeLegal(am_reg, cpu.sizeOfWord);
	Generate4adic(op_dep, 0, ap1, ap2, MakeImmediate(offset), MakeImmediate(((int64_t)offset+width-1LL) & 0x3fLL));
}


static Operand* CombineOffsetWidth(Operand* offset, Operand* width)
{
	Operand* ap3;

	// Combine offset,width into one register.
	ap3 = GetTempRegister();
	if (width->mode == am_imm)
		GenerateDiadic(op_loadi, 0, ap3, cg.MakeImmediate(width->offset->i << 8LL));
	else
		GenerateTriadic(op_asl, 0, ap3, width, cg.MakeImmediate(8));
	GenerateTriadic(op_or, 0, ap3, ap3, offset);
	return (ap3);
}


void BigfootCodeGenerator::GenerateBitfieldInsert(Operand* ap1, Operand* ap2, Operand* offset, Operand* width)
{
	Operand* op_begin;
	Operand* op_end;

	ConvertOffsetWidthToBeginEnd(offset, width, &op_begin, &op_end);
	Generate4adic(op_dep, 0, ap1, ap2, op_begin, op_end);
	ReleaseTempReg(op_end);
	ReleaseTempReg(op_begin);
}


void BigfootCodeGenerator::GenerateBitfieldInsert(Operand* ap1, Operand* ap2, ENODE* offset, ENODE* width)
{
	Operand* ap3, * ap4;
	Operand* op_begin;
	Operand* op_end;

	// If the field being inserted occupies the entire target, just use a move
	// instruction.
	if (offset->i128.low == 0 && offset->i128.high == 0) {
		if (width->i128.low >= cpu.sizeOfWord || (ap1->tp && width->i128.low >= ap1->tp->size)) {
			switch (ap2->mode) {
			case am_reg:
				GenerateDiadic(op_move, 0, ap1, ap2);
				return;
			case am_imm:
				GenerateLoadConst(ap1, ap2);
				return;
			default:
				GenerateLoad(ap1, ap2, ap1->tp->size, ap1->tp->size);
				return;
			}
		}
	}
	ap3 = GenerateExpression(offset, am_reg | am_imm | am_imm0, cpu.sizeOfWord, 1);
	ap4 = GenerateExpression(width, am_reg | am_imm | am_imm0, cpu.sizeOfWord, 1);
	ConvertOffsetWidthToBeginEnd(ap3, ap4, &op_begin, &op_end);
	GenerateBitfieldInsert(ap1, ap2, op_begin, op_end);
	ReleaseTempReg(op_end);
	ReleaseTempReg(op_begin);
	ReleaseTempReg(ap4);
	ReleaseTempReg(ap3);
}

void BigfootCodeGenerator::ConvertOffsetWidthToBeginEnd(Operand* offset, Operand* width, Operand** op_begin, Operand** op_end)
{
	Int128 me;

	*op_begin = offset->Clone();
	if (offset->mode == am_imm && width->mode == am_imm) {
		Int128::Add(&me, &offset->offset->i128, &width->offset->i128);
		*op_end = compiler.of.OperandFactory::MakeImmediate(me);
		return;
	}
	*op_end = GetTempRegister();
	GenerateTriadic(op_add, 0, *op_end, offset, width);
}

Operand* BigfootCodeGenerator::GenerateBitfieldExtract(Operand* ap, Operand* offset, Operand* width)
{
	Operand* ap1, *op_begin, * op_end;
	Int128 me;

	ap1 = GetTempRegister();
	ConvertOffsetWidthToBeginEnd(offset, width, &op_begin, &op_end);
	if (cpu.SupportsBitfield) {
		Generate4adic(isSigned ? op_ext : op_extu, 0, ap1, ap, op_begin, op_end);
	}
	else {
		/* under construction */
		GenerateTriadic(op_lsr, 0, ap1, ap, op_begin);
		if (isSigned)
			GenerateSignBitExtend(ap1, width);
		else {
			//GenerateZeroBitExtend(ap1, width);
		}
	}
	ReleaseTempReg(op_end);
	ReleaseTempReg(op_begin);
	return (ap1);
}

Operand* BigfootCodeGenerator::GenerateBitfieldExtract(Operand* ap, ENODE* offset, ENODE* width)
{
	Operand* ap1;
	Operand* ap2;
	Operand* ap3;
	Operand* op_begin;
	Operand* op_end;

	ap1 = GetTempRegister();
	ap2 = GenerateExpression(offset, am_reg | am_imm | am_imm0, cpu.sizeOfWord, 1);
	ap3 = GenerateExpression(width, am_reg | am_imm | am_imm0, cpu.sizeOfWord, 1);
	ConvertOffsetWidthToBeginEnd(ap2, ap3, &op_begin, &op_end);
	if (cpu.SupportsBitfield)
		Generate4adic(isSigned ? op_ext : op_extu, 0, ap1, ap, op_begin, op_end);
	else
		;	/* under construction */
	ReleaseTempReg(ap3);
	ReleaseTempReg(ap2);
	return (ap1);
}

Operand* BigfootCodeGenerator::GenerateEq(ENODE *node)
{
	Operand* ap1, * ap2, * ap3, * ap4;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempCrRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_seq)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_seq)->amclass3, node->p[1]->GetNaturalSize(), 1);
	if (ap2->mode == am_i16 || ap2->mode == am_imm) {
		ap4 = GetTempRegister();
		GenerateDiadic(op_loadi, 0, ap4, ap2);
		GenerateTriadic(op_zseq, 0, ap3, ap1, ap4);
		ReleaseTempRegister(ap4);
	}
	else
		GenerateTriadic(op_zseq, 0, ap3, ap1, ap2);
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	return (ap3);
}

Operand* BigfootCodeGenerator::GenerateNe(ENODE* node)
{
	Operand* ap1, * ap2, * ap3, * ap4;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempCrRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_sne)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_sne)->amclass3, node->p[1]->GetNaturalSize(), 1);
	if (ap2->mode == am_i16 || ap2->mode == am_imm) {
		ap4 = GetTempRegister();
		GenerateDiadic(op_loadi, 0, ap4, ap2);
		GenerateTriadic(op_zsne, 0, ap3, ap1, ap4);
		ReleaseTempRegister(ap4);
	}
	else {
		GenerateTriadic(op_zsne, 0, ap3, ap1, ap2);
	}
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	return (ap3);
}

Operand* BigfootCodeGenerator::GenerateLt(ENODE* node)
{
	Operand* ap1, * ap2, * ap3, * ap4;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_slt)->amclass3, node->p[1]->GetNaturalSize(), 1);
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_slt)->amclass2, node->p[0]->GetNaturalSize(), 1);
	if (ap2->mode == am_i16 || ap2->mode == am_imm) {
		ap4 = GetTempRegister();
		GenerateDiadic(op_loadi, 0, ap4, ap2);
		GenerateTriadic(op_zslt, 0, ap3, ap1, ap4);
		ReleaseTempRegister(ap4);
	}
	else
		GenerateTriadic(op_zslt, 0, ap3, ap1, ap2);
	ReleaseTempRegister(ap1);
	ReleaseTempRegister(ap2);
	return (ap3);
}

Operand* BigfootCodeGenerator::GenerateLe(ENODE* node)
{
	Operand* ap1, * ap2, * ap3, * ap4;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_sle)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_sle)->amclass3, node->p[1]->GetNaturalSize(), 1);
	if (ap2->mode == am_i16)
		GenerateTriadic(op_sle, 0, ap3, ap1, ap2);
	else if (ap2->mode == am_imm) {
		ap4 = GetTempRegister();
		GenerateDiadic(op_loadi, 0, ap4, ap2);
		GenerateTriadic(op_zsle, 0, ap3, ap1, ap4);
		ReleaseTempRegister(ap4);
	}
	else
		GenerateTriadic(op_zsle, 0, ap3, ap1, ap2);
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	return (ap3);
}

Operand* BigfootCodeGenerator::GenerateGt(ENODE* node)
{
	Operand* ap1, * ap2, * ap3, * ap4;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_sgt)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_sgt)->amclass3, node->p[1]->GetNaturalSize(), 1);
	if (ap2->mode == am_i16 || ap2->mode == am_imm) {
		ap4 = GetTempRegister();
		GenerateDiadic(op_loadi, 0, ap4, ap2);
		GenerateTriadic(op_zsgt, 0, ap3, ap1, ap4);
		ReleaseTempRegister(ap4);
	}
	else
		GenerateTriadic(op_zsgt, 0, ap3, ap1, ap2);
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	//		GenerateDiadic(op_sgt,0,ap3,ap3);
	return (ap3);
}

Operand* BigfootCodeGenerator::GenerateGe(ENODE* node)
{
	Operand* ap1, * ap2, * ap3, * ap4;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_sge)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_sge)->amclass3, node->p[1]->GetNaturalSize(), 1);
	if (ap2->mode == am_i16 || ap2->mode == am_imm) {
		ap4 = GetTempRegister();
		GenerateDiadic(op_loadi, 0, ap4, ap2);
		GenerateTriadic(op_zsge, 0, ap3, ap1, ap4);
		ReleaseTempRegister(ap4);
	}
	else
		GenerateTriadic(op_zsge, 0, ap3, ap1, ap2);
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	return (ap3);
}

Operand* BigfootCodeGenerator::GenerateLtu(ENODE* node)
{
	Operand* ap1, * ap2, * ap3;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_sltu)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_sltu)->amclass3, node->p[1]->GetNaturalSize(), 1);
	if (ap2->mode == am_i16 || ap2->mode == am_imm)
		GenerateTriadic(op_zsltu, 0, ap3, ap1, ap2);
	else
		Generate4adic(op_zsltu, 0, ap3, ap1, ap2, MakeImmediate(1));
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	return (ap3);
}

Operand* BigfootCodeGenerator::GenerateLeu(ENODE* node)
{
	Operand* ap1, * ap2, * ap3;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_sleu)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_sleu)->amclass3, node->p[1]->GetNaturalSize(), 1);
	if (ap2->mode == am_i16 || ap2->mode == am_imm)
		GenerateTriadic(op_zsleu, 0, ap3, ap1, ap2);
	else
		Generate4adic(op_zsleu, 0, ap3, ap1, ap2, MakeImmediate(1));
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	return (ap3);
}

Operand* BigfootCodeGenerator::GenerateGtu(ENODE* node)
{
	Operand* ap1, * ap2, * ap3;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_sgtu)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_sgtu)->amclass3, node->p[1]->GetNaturalSize(), 1);
	if (ap2->mode == am_i16 || ap2->mode==am_imm)
		GenerateTriadic(op_zsgtu, 0, ap3, ap1, ap2);
	else
		Generate4adic(op_zsgtu, 0, ap3, ap1, ap2, MakeImmediate(1));
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	//		GenerateDiadic(op_sgt,0,ap3,ap3);
	return (ap3);
}

Operand* BigfootCodeGenerator::GenerateGeu(ENODE* node)
{
	Operand* ap1, * ap2, * ap3;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_sgeu)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_sgeu)->amclass3, node->p[1]->GetNaturalSize(), 1);
	if (ap2->mode == am_i16 || ap2->mode==am_imm)
		GenerateTriadic(op_zsgeu, 0, ap3, ap1, ap2);
	else
		Generate4adic(op_zsgeu, 0, ap3, ap1, ap2, MakeImmediate(1));
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	return (ap3);
}

Operand* BigfootCodeGenerator::GenerateFeq(ENODE* node)
{
	Operand* ap1, * ap2, * ap3;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_fcmp)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_fcmp)->amclass3, node->p[1]->GetNaturalSize(), 1);
	Generate4adic(op_feq, 0, ap3, ap1, ap2, MakeImmediate(1));
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	return (ap3);
}

Operand* BigfootCodeGenerator::GenerateFne(ENODE* node)
{
	Operand* ap1, * ap2, * ap3;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_fcmp)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_fcmp)->amclass3, node->p[1]->GetNaturalSize(), 1);
	Generate4adic(op_fne, 0, ap3, ap1, ap2, MakeImmediate(1));
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	return (ap3);
}

Operand* BigfootCodeGenerator::GenerateFlt(ENODE* node)
{
	Operand* ap1, * ap2, * ap3;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_fcmp)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_fcmp)->amclass3, node->p[1]->GetNaturalSize(), 1);
	Generate4adic(op_flt, 0, ap3, ap1, ap2, MakeImmediate(1));
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	return (ap3);
}

Operand* BigfootCodeGenerator::GenerateFle(ENODE* node)
{
	Operand* ap1, * ap2, * ap3;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_fcmp)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_fcmp)->amclass3, node->p[1]->GetNaturalSize(), 1);
	Generate4adic(op_fle, 0, ap3, ap1, ap2, MakeImmediate(1));
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	return (ap3);
}

Operand* BigfootCodeGenerator::GenerateFgt(ENODE* node)
{
	Operand* ap1, * ap2, * ap3;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_fcmp)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_fcmp)->amclass3, node->p[1]->GetNaturalSize(), 1);
	Generate4adic(op_fgt, 0, ap3, ap1, ap2, MakeImmediate(1));
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	return (ap3);
}

Operand* BigfootCodeGenerator::GenerateFge(ENODE* node)
{
	Operand* ap1, * ap2, * ap3;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_fcmp)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_fcmp)->amclass3, node->p[1]->GetNaturalSize(), 1);
	Generate4adic(op_fge, 0, ap3, ap1, ap2, MakeImmediate(1));
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	return (ap3);
}

Operand *BigfootCodeGenerator::GenExpr(ENODE *node)
{
	Operand *ap1,*ap2,*ap3,*ap4;
	int64_t lab0, lab1;
	int64_t size = cpu.sizeOfWord;
	int op;

  lab0 = nextlabel++;
  lab1 = nextlabel++;

	switch(node->nodetype) {
	case en_eq:		op = op_seq;	break;
	case en_ne:		op = op_sne;	break;
	case en_lt:		op = op_slt;	break;
	case en_ult:	op = op_sltu;	break;
	case en_le:		op = op_sle;	break;
	case en_ule:	op = op_sleu;	break;
	case en_gt:		op = op_sgt;	break;
	case en_ugt:	op = op_sgtu;	break;
	case en_ge:		op = op_sge;	break;
	case en_uge:	op = op_sgeu;	break;
	case en_flt:	op = op_fslt;	break;
	case en_fle:	op = op_fsle;	break;
	case en_fgt:	op = op_fsgt;	break;
	case en_fge:	op = op_fsge;	break;
	case en_feq:	op = op_fseq;	break;
	case en_fne:	op = op_fsne;	break;
	case en_veq:
		size = node->GetNaturalSize();
		ap3 = GetTempVectorRegister();         
		ap1 = cg.GenerateExpression(node->p[0],am_reg,size,1);
		ap2 = cg.GenerateExpression(node->p[1],am_reg,size,1);
		GenerateTriadic(op_vseq,0,ap3,ap1,ap2);
		ReleaseTempReg(ap2);
		ReleaseTempReg(ap1);
		return (ap3);
	case en_vne:
		size = node->GetNaturalSize();
		ap3 = GetTempVectorRegister();         
		ap1 = cg.GenerateExpression(node->p[0],am_reg,size,1);
		ap2 = cg.GenerateExpression(node->p[1],am_reg,size,1);
		GenerateTriadic(op_vsne,0,ap3,ap1,ap2);
		ReleaseTempReg(ap2);
		ReleaseTempReg(ap1);
		return (ap3);
	case en_vlt:
		size = node->GetNaturalSize();
		ap3 = GetTempVectorRegister();         
		ap1 = cg.GenerateExpression(node->p[0],am_reg,size,1);
		ap2 = cg.GenerateExpression(node->p[1],am_reg,size,1);
		GenerateTriadic(op_vslt,0,ap3,ap1,ap2);
		ReleaseTempReg(ap2);
		ReleaseTempReg(ap1);
		return (ap3);
	case en_vle:
		size = node->GetNaturalSize();
		ap3 = GetTempVectorRegister();         
		ap1 = cg.GenerateExpression(node->p[0], am_reg, size, 1);
		ap2 = cg.GenerateExpression(node->p[1], am_reg, size, 1);
		GenerateTriadic(op_vsle,0,ap3,ap1,ap2);
		ReleaseTempReg(ap2);
		ReleaseTempReg(ap1);
		return (ap3);
	case en_vgt:
		size = node->GetNaturalSize();
		ap3 = GetTempVectorRegister();         
		ap1 = cg.GenerateExpression(node->p[0],am_reg,size,1);
		ap2 = cg.GenerateExpression(node->p[1],am_reg,size,1);
		GenerateTriadic(op_vsgt,0,ap3,ap1,ap2);
		ReleaseTempReg(ap2);
		ReleaseTempReg(ap1);
		return (ap3);
	case en_vge:
		size = node->GetNaturalSize();
		ap3 = GetTempVectorRegister();         
		ap1 = cg.GenerateExpression(node->p[0],am_reg,size,1);
		ap2 = cg.GenerateExpression(node->p[1],am_reg,size,1);
		GenerateTriadic(op_vsge,0,ap3,ap1,ap2);
		ReleaseTempReg(ap2);
		ReleaseTempReg(ap1);
		return (ap3);
	case en_land_safe:
		size = node->GetNaturalSize();
		ap3 = GetTempVectorRegister();
		ap1 = cg.GenerateExpression(node->p[0], am_reg, size,1);
		ap2 = cg.GenerateExpression(node->p[1], am_reg | am_imm, size,1);
		GenerateTriadic(op_and, 0, ap3, ap1, ap2);
		ReleaseTempReg(ap2);
		ReleaseTempReg(ap1);
		return(ap3);
	case en_lor_safe:
		size = node->GetNaturalSize();
		ap3 = GetTempVectorRegister();
		ap1 = cg.GenerateExpression(node->p[0], am_reg, size,1);
		ap2 = cg.GenerateExpression(node->p[1], am_reg, size,1);
		GenerateTriadic(op_or, 0, ap3, ap1, ap2);
		ReleaseTempReg(ap2);
		ReleaseTempReg(ap1);
		return(ap3);
	default:	// en_land, en_lor
		//ap1 = GetTempRegister();
		//ap2 = cg.GenerateExpression(node,am_reg,8);
		//GenerateDiadic(op_redor,0,ap1,ap2);
		//ReleaseTempReg(ap2);
		GenerateFalseJump(node,lab0,0);
		ap1 = GetTempRegister();
		GenerateDiadic(cpu.ldi_op|op_dot,0,ap1,MakeImmediate(1));
		GenerateMonadic(op_branch,0,MakeDataLabel(lab1,regZero));
		GenerateLabel(lab0);
		GenerateDiadic(cpu.ldi_op|op_dot,0,ap1,MakeImmediate(0));
		GenerateLabel(lab1);
		ap1->isBool = true;
		return (ap1);
	}

	switch (node->nodetype) {
	case en_eq:	return (GenerateEq(node));
	case en_ne:	return (GenerateNe(node));
	case en_lt:	return (GenerateLt(node));
	case en_le:	return (GenerateLe(node));
	case en_gt: return (GenerateGt(node));
	case en_ge:	return (GenerateGe(node));
	case en_ult:	return (GenerateLtu(node));
	case en_ule:	return (GenerateLeu(node));
	case en_ugt:	return (GenerateGtu(node));
	case en_uge:	return (GenerateGeu(node));
	case en_flt:	return (GenerateFlt(node));
	case en_fle:	return (GenerateFle(node));
	case en_fgt:	return (GenerateFgt(node));
	case en_fge:	return (GenerateFge(node));
	case en_feq:	return (GenerateFeq(node));
	case en_fne:	return (GenerateFne(node));
	case en_chk:
		size = node->GetNaturalSize();
        ap4 = GetTempRegister();         
		ap1 = cg.GenerateExpression(node->p[0],am_reg,size,1);
		ap2 = cg.GenerateExpression(node->p[1],am_reg,size,1);
		ap3 = cg.GenerateExpression(node->p[2],am_reg|am_imm0,size,1);
		if (ap3->mode == am_imm) {  // must be a zero
		   ap3->mode = am_reg;
		   ap3->preg = 0;
        }
   		Generate4adic(op_chk,0,ap4,ap1,ap2,ap3);
        ReleaseTempRegister(ap3);
        ReleaseTempRegister(ap2);
        ReleaseTempRegister(ap1);
        return ap4;
	}
	size = node->GetNaturalSize();
  ap3 = GetTempRegister();         
	ap1 = cg.GenerateExpression(node->p[0],am_reg,size,1);
	ap2 = cg.GenerateExpression(node->p[1],am_reg|am_imm|am_imm0,size,1);
	GenerateTriadic(op,0,ap3,ap1,ap2);
  ReleaseTempRegister(ap2);
  ReleaseTempRegister(ap1);
	ap3->isBool = true;
	return (ap3);
	/*
    GenerateFalseJump(node,lab0,0);
    ap1 = GetTempRegister();
    GenerateDiadic(op_ld,0,ap1,MakeImmediate(1));
    GenerateMonadic(op_branch,0,MakeDataLabel(lab1));
    GenerateLabel(lab0);
    GenerateDiadic(op_ld,0,ap1,MakeImmediate(0));
    GenerateLabel(lab1);
    return ap1;
	*/
}

void BigfootCodeGenerator::GenerateBranchTrue(Operand* ap, int64_t label)
{
	gHeadif = currentFn->pl.tail;
	ap->MakeLegal(amCrReg, 1);
	GenerateDiadic(op_bnez, 0, ap, MakeCodeLabel(label));
}

void BigfootCodeGenerator::GenerateBranchFalse(Operand* ap, int64_t label)
{
	gHeadif = currentFn->pl.tail;
	ap->MakeLegal(amCrReg, 1);
	GenerateDiadic(op_beqz, 0, ap, MakeCodeLabel(label));
}

void BigfootCodeGenerator::GenerateBeq(Operand* ap1, Operand* ap2, int64_t label)
{
	Int128 i;

	i = regs[ap1->preg].val128;
	if (ap2->mode == am_imm) {
		// Check if branch is always false. If the branch is always false, do nothing.
		// If the branch is always true, generate unconditional branch. Unconditional
		// branches are faster than conditional ones.
		if (regs[ap1->preg].isConst) {
			if (!Int128::IsEQ(&i, &ap2->offset->i128))
				;
			else
				GenerateMonadic(op_branch, 0, MakeCodeLabel(label));
		}
		else {
			if (Int128::IsEQ(&ap2->offset->i128, Int128::Zero())) {
				ap1->MakeLegal(amCrReg, 1);
				GenerateDiadic(op_beqz, 0, ap1, MakeCodeLabel(label));
			}
			else {
				Operand* ap4 = GetTempRegister();
				Operand* ap3 = GetTempCrRegister();
				GenerateLoadConst(ap2, ap4);
				GenerateTriadic(op_cmp, 0, ap3, ap1, ap4);
				GenerateDiadic(op_beq, 0, ap3, MakeCodeLabel(label));
				ReleaseTempRegister(ap3);
				ReleaseTempRegister(ap4);
			}
		}
	}
	else {
		Operand* ap3 = GetTempCrRegister();
		GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
		GenerateDiadic(op_beq, 0, ap3, MakeCodeLabel(label));
		ReleaseTempRegister(ap3);
	}
}

void BigfootCodeGenerator::GenerateBne(Operand* ap1, Operand* ap2, int64_t label)
{
	Int128 i;

	i = regs[ap1->preg].val128;
	if (ap2->mode == am_imm) {
		// Check if branch is always false. If the branch is always false, do nothing.
		// If the branch is always true, generate unconditional branch. Unconditional
		// branches are faster than conditional ones.
		if (regs[ap1->preg].isConst) {
			if (Int128::IsEQ(&i, &ap2->offset->i128))
				;
			else
				GenerateMonadic(op_branch, 0, MakeCodeLabel(label));
		}
		else {
			if (Int128::IsEQ(&ap2->offset->i128, Int128::Zero())) {
				ap1->MakeLegal(amCrReg, 1);
				GenerateDiadic(op_bnez, 0, ap1, MakeCodeLabel(label));
			}
			else {
				Operand* ap4 = GetTempRegister();
				Operand* ap3 = GetTempCrRegister();
				GenerateLoadConst(ap2, ap4);
				GenerateTriadic(op_cmp, 0, ap3, ap1, ap4);
				GenerateDiadic(op_bne, 0, ap3, MakeCodeLabel(label));
				ReleaseTempRegister(ap3);
				ReleaseTempRegister(ap4);
			}
		}
	}
	else {
		Operand* ap3 = GetTempCrRegister();
		GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
		GenerateDiadic(op_bne, 0, ap3, MakeCodeLabel(label));
		ReleaseTempRegister(ap3);
	}
}

void BigfootCodeGenerator::GenerateBlt(Operand* ap1, Operand* ap2, int64_t label)
{
	Int128 i;

	i = regs[ap1->preg].val128;
	if (ap2->mode == am_imm) {
		// Check if branch is always false. If the branch is always false, do nothing.
		// If the branch is always true, generate unconditional branch. Unconditional
		// branches are faster than conditional ones.
		if (regs[ap1->preg].isConst) {
			if (!Int128::IsLT(&i, &ap2->offset->i128))
				;
			else
				GenerateMonadic(op_branch, 0, MakeCodeLabel(label));
		}
		else {
			Operand* ap4 = GetTempRegister();
			Operand* ap3 = GetTempCrRegister();
			GenerateLoadConst(ap2, ap4);
			GenerateTriadic(op_cmp, 0, ap3, ap1, ap4);
			GenerateDiadic(op_blt, 0, ap3, MakeCodeLabel(label));
			ReleaseTempRegister(ap3);
			ReleaseTempRegister(ap4);
		}
	}
	else {
		Operand* ap3 = GetTempCrRegister();
		GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
		GenerateDiadic(op_blt, 0, ap3, MakeCodeLabel(label));
		ReleaseTempRegister(ap3);
	}
}

void BigfootCodeGenerator::GenerateBge(Operand* ap1, Operand* ap2, int64_t label)
{
	if (ap2->mode == am_imm) {
		Int128 i;

		i = regs[ap1->preg].val128;
		// Check if branch is always false. If the branch is always false, do nothing.
		if (regs[ap1->preg].isConst && !Int128::IsGE(&i, &ap2->offset->i128))
			;
		else {
			Operand* ap4 = GetTempRegister();
			Operand* ap3 = GetTempCrRegister();
			GenerateLoadConst(ap2, ap4);
			GenerateTriadic(op_cmp, 0, ap3, ap1, ap4);
			GenerateDiadic(op_bge, 0, ap3, MakeCodeLabel(label));
			ReleaseTempRegister(ap3);
			ReleaseTempRegister(ap4);
		}
	}
	else {
		Operand* ap3 = GetTempCrRegister();
		GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
		GenerateDiadic(op_bge, 0, ap3, MakeCodeLabel(label));
		ReleaseTempRegister(ap3);
	}
}

void BigfootCodeGenerator::GenerateBle(Operand* ap1, Operand* ap2, int64_t label)
{
	Int128 i;

	i = regs[ap1->preg].val128;
	if (ap2->mode == am_imm) {
		// Check if branch is always false. If the branch is always false, do nothing.
		// If the branch is always true, generate unconditional branch. Unconditional
		// branches are faster than conditional ones.
		if (regs[ap1->preg].isConst) {
			if (!Int128::IsLE(&i, &ap2->offset->i128))
				;
			else
				GenerateMonadic(op_branch, 0, MakeCodeLabel(label));
		}
		else {
			Operand* ap4 = GetTempRegister();
			Operand* ap3 = GetTempCrRegister();
			GenerateLoadConst(ap2, ap4);
			GenerateTriadic(op_cmp, 0, ap3, ap1, ap4);
			GenerateDiadic(op_ble, 0, ap3, MakeCodeLabel(label));
			ReleaseTempRegister(ap3);
			ReleaseTempRegister(ap4);
		}
	}
	else {
		Operand* ap3 = GetTempCrRegister();
		GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
		GenerateDiadic(op_ble, 0, ap3, MakeCodeLabel(label));
		ReleaseTempRegister(ap3);
	}
}

void BigfootCodeGenerator::GenerateBgt(Operand* ap1, Operand* ap2, int64_t label)
{
	Int128 i;

	i = regs[ap1->preg].val128;
	if (ap2->mode == am_imm) {
		// Check if branch is always false. If the branch is always false, do nothing.
		// If the branch is always true, generate unconditional branch. Unconditional
		// branches are faster than conditional ones.
		if (regs[ap1->preg].isConst) {
			if (Int128::IsLE(&i, &ap2->offset->i128))
				;
			else
				GenerateMonadic(op_branch, 0, MakeCodeLabel(label));
		}
		else {
			Operand* ap4 = GetTempRegister();
			Operand* ap3 = GetTempCrRegister();
			GenerateLoadConst(ap2, ap4);
			GenerateTriadic(op_cmp, 0, ap3, ap1, ap4);
			GenerateDiadic(op_bgt, 0, ap3, MakeCodeLabel(label));
			ReleaseTempRegister(ap3);
			ReleaseTempRegister(ap4);
		}
	}
	else {
		Operand* ap3 = GetTempCrRegister();
		GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
		GenerateDiadic(op_bgt, 0, ap3, MakeCodeLabel(label));
		ReleaseTempRegister(ap3);
	}
}

void BigfootCodeGenerator::GenerateBltu(Operand* ap1, Operand* ap2, int64_t label)
{
	Int128 i;

	i = regs[ap1->preg].val128;
	if (ap2->mode == am_imm) {
		// Check if branch is always false. If the branch is always false, do nothing.
		// If the branch is always true, generate unconditional branch. Unconditional
		// branches are faster than conditional ones.
		if (regs[ap1->preg].isConst) {
			if (!Int128::IsULT(&i, &ap2->offset->i128))
				;
			else
				GenerateMonadic(op_branch, 0, MakeCodeLabel(label));
		}
		else {
			Operand* ap4 = GetTempRegister();
			Operand* ap3 = GetTempCrRegister();
			GenerateLoadConst(ap2, ap4);
			GenerateTriadic(op_cmp, 0, ap3, ap1, ap4);
			GenerateDiadic(op_bltu, 0, ap3, MakeCodeLabel(label));
			ReleaseTempRegister(ap3);
			ReleaseTempRegister(ap4);
		}
	}
	else {
		Operand* ap3 = GetTempCrRegister();
		GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
		GenerateDiadic(op_bltu, 0, ap3, MakeCodeLabel(label));
		ReleaseTempRegister(ap3);
	}
}

void BigfootCodeGenerator::GenerateBgeu(Operand* ap1, Operand* ap2, int64_t label)
{
	Int128 i;

	i = regs[ap1->preg].val128;
	if (ap2->mode == am_imm) {
		// Check if branch is always false. If the branch is always false, do nothing.
		// If the branch is always true, generate unconditional branch. Unconditional
		// branches are faster than conditional ones.
		if (regs[ap1->preg].isConst) {
			if (Int128::IsULT(&i, &ap2->offset->i128))
				;
			else
				GenerateMonadic(op_branch, 0, MakeCodeLabel(label));
		}
		else {
			Operand* ap4 = GetTempRegister();
			Operand* ap3 = GetTempCrRegister();
			GenerateLoadConst(ap2, ap4);
			GenerateTriadic(op_cmp, 0, ap3, ap1, ap4);
			GenerateDiadic(op_bgeu, 0, ap3, MakeCodeLabel(label));
			ReleaseTempRegister(ap3);
			ReleaseTempRegister(ap4);
		}
	}
	else {
		Operand* ap3 = GetTempCrRegister();
		GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
		GenerateDiadic(op_bgeu, 0, ap3, MakeCodeLabel(label));
		ReleaseTempRegister(ap3);
	}
}

void BigfootCodeGenerator::GenerateBleu(Operand* ap1, Operand* ap2, int64_t label)
{
	Int128 i;

	i = regs[ap1->preg].val128;
	if (ap2->mode == am_imm) {
		// Check if branch is always false. If the branch is always false, do nothing.
		// If the branch is always true, generate unconditional branch. Unconditional
		// branches are faster than conditional ones.
		if (regs[ap1->preg].isConst) {
			if (!Int128::IsULE(&i, &ap2->offset->i128))
				;
			else
				GenerateMonadic(op_branch, 0, MakeCodeLabel(label));
		}
		else {
			Operand* ap4 = GetTempRegister();
			Operand* ap3 = GetTempCrRegister();
			GenerateLoadConst(ap2, ap4);
			GenerateTriadic(op_cmp, 0, ap3, ap1, ap4);
			GenerateDiadic(op_bleu, 0, ap3, MakeCodeLabel(label));
			ReleaseTempRegister(ap3);
			ReleaseTempRegister(ap4);
		}
	}
	else {
		Operand* ap3 = GetTempCrRegister();
		GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
		GenerateDiadic(op_bleu, 0, ap3, MakeCodeLabel(label));
		ReleaseTempRegister(ap3);
	}
}

void BigfootCodeGenerator::GenerateBgtu(Operand* ap1, Operand* ap2, int64_t label)
{
	Int128 i;

	i = regs[ap1->preg].val128;
	if (ap2->mode == am_imm) {
		// Check if branch is always false. If the branch is always false, do nothing.
		// If the branch is always true, generate unconditional branch. Unconditional
		// branches are faster than conditional ones.
		if (regs[ap1->preg].isConst) {
			if (Int128::IsULE(&i, &ap2->offset->i128))
				;
			else
				GenerateMonadic(op_branch, 0, MakeCodeLabel(label));
		}
		else {
			Operand* ap4 = GetTempRegister();
			Operand* ap3 = GetTempCrRegister();
			GenerateLoadConst(ap2, ap4);
			GenerateTriadic(op_cmp, 0, ap3, ap1, ap4);
			GenerateDiadic(op_bgtu, 0, ap3, MakeCodeLabel(label));
			ReleaseTempRegister(ap3);
			ReleaseTempRegister(ap4);
		}
	}
	else {
		Operand* ap3 = GetTempCrRegister();
		GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
		GenerateDiadic(op_bgtu, 0, ap3, MakeCodeLabel(label));
		ReleaseTempRegister(ap3);
	}
}

void BigfootCodeGenerator::GenerateBand(Operand* ap1, Operand* ap2, int64_t label)
{
	Operand* ap3, * ap4;

	if (cpu.SupportsBand)
		GenerateTriadic(op_band, 0, ap1, ap2, MakeCodeLabel(label));
	else {
		ap3 = GetTempRegister();
		ap4 = GetTempCrRegister();
		GenerateTriadic(op_and, 0, ap3, ap1, ap2);
		GenerateTriadic(op_sne, 0, ap4, ap3, makereg(regZero));
		GenerateDiadic(op_beq, 0, ap4, MakeCodeLabel(label));
		ReleaseTempRegister(ap4);
		ReleaseTempReg(ap3);
	}
}

void BigfootCodeGenerator::GenerateBor(Operand* ap1, Operand* ap2, int64_t label)
{
	Operand* ap3, * ap4;

	if (cpu.SupportsBor)
		GenerateTriadic(op_bor, 0, ap1, ap2, MakeCodeLabel(label));
	else {
		ap3 = GetTempRegister();
		ap4 = GetTempCrRegister();
		GenerateTriadic(op_or, 0, ap3, ap1, ap2);
		GenerateTriadic(op_sne, 0, ap4, ap3, makereg(regZero));
		GenerateDiadic(op_beq, 0, ap4, MakeCodeLabel(label));
		ReleaseTempRegister(ap4);
		ReleaseTempReg(ap3);
	}
}

void BigfootCodeGenerator::GenerateBnand(Operand* ap1, Operand* ap2, int64_t label)
{
	Operand* ap3, * ap4;

	ap3 = GetTempRegister();
	ap4 = GetTempCrRegister();
	GenerateTriadic(op_and, 0, ap3, ap1, ap2);
	GenerateTriadic(op_seq, 0, ap4, ap3, makereg(regZero));
	GenerateDiadic(op_beq, 0, ap4, MakeCodeLabel(label));
	ReleaseTempRegister(ap4);
	ReleaseTempReg(ap3);
}

void BigfootCodeGenerator::GenerateBnor(Operand* ap1, Operand* ap2, int64_t label)
{
	Operand* ap3, * ap4;

	ap3 = GetTempRegister();
	ap4 = GetTempCrRegister();
	GenerateTriadic(op_or, 0, ap3, ap1, ap2);
	GenerateTriadic(op_seq, 0, ap4, ap3, makereg(regZero));
	GenerateDiadic(op_beq, 0, ap4, MakeCodeLabel(label));
	ReleaseTempRegister(ap4);
	ReleaseTempReg(ap3);
}

bool BigfootCodeGenerator::GenerateBranch(ENODE *node, int op, int64_t label, int predreg, unsigned int prediction, bool limit)
{
	int64_t size, sz;
	Operand *ap1, *ap2, *ap3;
	OCODE *ip;

	if ((op == op_nand || op == op_nor || op == op_and || op == op_or) && (node->p[0]->HasCall() || node->p[1]->HasCall()))
		return (false);
	ap3 = GetTempRegister();
	size = node->GetNaturalSize();
	ip = currentFn->pl.tail;
	if (op==op_flt || op==op_fle || op==op_fgt || op==op_fge || op==op_feq || op==op_fne) {
		ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_fcmp)->amclass2, size, 1);
		ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_fcmp)->amclass3, size, 1);
	}
  else {
		ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op)->amclass2, size, 1);
		ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op)->amclass3, size, 1);
	}
	if (ap1->mode == am_imm && ap2->mode == am_imm) {
		GenerateLoadConst(ap3, ap1);
		ap1 = ap3;
	}
	if (limit && currentFn->pl.Count(ip) > 10) {
		currentFn->pl.tail = ip;
		currentFn->pl.tail->fwd = nullptr;
		return (false);
	}

	/*
	// Optimize CMP to zero and branch into plain branch, this works only for
	// signed relational compares.
	if (ap2->mode == am_imm && ap2->offset->i==0 && (op==op_eq || op==op_ne || op==op_lt || op==op_le || op==op_gt || op==op_ge)) {
    	switch(op)
    	{
    	case op_eq:	op = op_beq; break;
    	case op_ne:	op = op_bne; break;
    	case op_lt: op = op_blt; break;
    	case op_le: op = op_ble; break;
    	case op_gt: op = op_bgt; break;
    	case op_ge: op = op_bge; break;
    	}
    	ReleaseTempReg(ap3);
		ReleaseTempReg(ap2);
		ReleaseTempReg(ap1);
		GenerateDiadic(op,0,ap1,MakeCodeLabel(label));
		return;
	}
	*/
	/*
	if (op==op_ltu || op==op_leu || op==op_gtu || op==op_geu)
 	    GenerateTriadic(op_cmpu,0,ap3,ap1,ap2);
    else if (op==op_flt || op==op_fle || op==op_fgt || op==op_fge || op==op_feq || op==op_fne)
        GenerateTriadic(op_fdcmp,0,ap3,ap1,ap2);
	else 
 	    GenerateTriadic(op_cmp,0,ap3,ap1,ap2);
	*/
	sz = 0;
	switch(op)
	{
	case op_bchk:	break;
	case op_nand:	op = op_bnand; break;
	case op_nor:	op = op_bnor; break;
	case op_and:	op = op_band; break;
	case op_or:	op = op_bor; break;
	case op_eq:	op = op_beq; break;
	case op_ne:	op = op_bne; break;
	case op_lt: op = op_blt; break;
	case op_le: op = op_ble; break;
	case op_gt: op = op_bgt; break;
	case op_ge: op = op_bge; break;
	case op_ltu: op = op_bltu; break;
	case op_leu: op = op_bleu; break;
	case op_gtu: op = op_bgtu; break;
	case op_geu: op = op_bgeu; break;
	case op_feq:	op = op_fbeq; sz = 'd'; break;
	case op_fne:	op = op_fbne; sz = 'd'; break;
	case op_flt:	op = op_fblt; sz = 'd'; break;
	case op_fle:	op = op_fble; sz = 'd'; break;
	case op_fgt:	op = op_fbgt; sz = 'd'; break;
	case op_fge:	op = op_fbge; sz = 'd'; break;
	}

	switch(op) {

	case op_fbne:
		ap3 = GetTempRegister();
		GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
		GenerateTriadic(op_bbs, 0, ap3, MakeImmediate(8+16), MakeCodeLabel(label));
		//GenerateTriadic(op_fsne, 0, ap3, ap1, ap2);
		//GenerateDiadic(op_bnez, 0, ap3, MakeCodeLabel(label));
		ReleaseTempReg(ap3);
		break;

	case op_fbeq:
		ap3 = GetTempRegister();
		GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
		GenerateTriadic(op_bbs, 0, ap3, MakeImmediate(0+16), MakeCodeLabel(label));
		//GenerateTriadic(op_fseq, 0, ap3, ap1, ap2);
		//GenerateDiadic(op_bnez, 0, ap3, MakeCodeLabel(label));
		ReleaseTempReg(ap3);
		break;

	case op_fblt:
		ap3 = GetTempRegister();
		GenerateTriadic(op_fcmp, 0, ap3, ap1, ap2);
		GenerateTriadic(op_bbs, 0, ap3, MakeImmediate(1), MakeCodeLabel(label));
		//GenerateTriadic(op_fslt, 0, ap3, ap1, ap2);
		//GenerateDiadic(op_bnez, 0, ap3, MakeCodeLabel(label));
		ReleaseTempReg(ap3);
		break;

	case op_fble:
		ap3 = GetTempRegister();
		GenerateTriadic(op_fcmp, 0, ap3, ap1, ap2);
		GenerateTriadic(op_bbs, 0, ap3, MakeImmediate(2), MakeCodeLabel(label));
		//GenerateTriadic(op_fsle, 0, ap3, ap1, ap2);
		//GenerateDiadic(op_bnez, 0, ap3, MakeCodeLabel(label));
		ReleaseTempReg(ap3);
		break;

	case op_fbgt:
		ap3 = GetTempRegister();
		GenerateTriadic(op_fcmp, 0, ap3, ap1, ap2);
		GenerateTriadic(op_bbs, 0, ap3, MakeImmediate(10), MakeCodeLabel(label));
		//GenerateTriadic(op_fslt, 0, ap3, ap2, ap1);
		//GenerateDiadic(op_bnez, 0, ap3, MakeCodeLabel(label));
		ReleaseTempReg(ap3);
		break;

	case op_fbge:
		ap3 = GetTempRegister();
		GenerateTriadic(op_fcmp, 0, ap3, ap1, ap2);
		GenerateTriadic(op_bbs, 0, ap3, MakeImmediate(11), MakeCodeLabel(label));
		//GenerateTriadic(op_fsle, 0, ap3, ap2, ap1);
		//GenerateDiadic(op_bnez, 0, ap3, MakeCodeLabel(label));
		ReleaseTempReg(ap3);
		break;

	case op_band:	GenerateBand(ap1, ap2, label); break;
	case op_bor:	GenerateBor(ap1, ap2, label);	break;
	case op_bnand:	GenerateBnand(ap1, ap2, label);	break;
	case op_bnor:	GenerateBnor(ap1, ap2, label);	break;
	case op_beq:	GenerateBeq(ap1, ap2, label); break;
	case op_bne:	GenerateBne(ap1, ap2, label); break;
	case op_blt:	GenerateBlt(ap1, ap2, label); break;
	case op_ble:	GenerateBle(ap1, ap2, label); break;
	case op_bgt:	GenerateBgt(ap1, ap2, label); break;
	case op_bge:	GenerateBge(ap1, ap2, label);	break;
	case op_bltu:	GenerateBltu(ap1, ap2, label);	break;
	case op_bleu:	GenerateBleu(ap1, ap2, label);  break;
	case op_bgtu:	GenerateBgtu(ap1, ap2, label);	break;
	case op_bgeu:	GenerateBgeu(ap1, ap2, label);	break;
	}
  ReleaseTempReg(ap2);
  ReleaseTempReg(ap1);
	return (true);
}


static void SaveRegisterSet(Symbol *sym)
{
	int nn, mm;

	if (!cpu.SupportsPush || true) {
		mm = sym->tp->btpp->type!=bt_void ? 29 : 30;
		GenerateTriadic(op_sub,0,makereg(regSP),makereg(regSP),cg.MakeImmediate(mm*cpu.sizeOfWord));
		mm = 0;
		for (nn = 1 + (sym->tp->btpp->type!=bt_void ? 1 : 0); nn < 31; nn++) {
			cg.GenerateStore(makereg(nn),cg.MakeIndexed(mm,regSP),cpu.sizeOfWord);
			mm += cpu.sizeOfWord;
		}
	}
	else
		for (nn = 1 + (sym->tp->btpp->type!=bt_void ? 1 : 0); nn < 31; nn++)
			GenerateMonadic(op_push,0,makereg(nn));
}

static void RestoreRegisterSet(Symbol * sym)
{
	int nn, mm;

	if (!cpu.SupportsPop || true) {
		mm = 0;
		for (nn = 1 + (sym->tp->btpp->type!=bt_void ? 1 : 0); nn < 31; nn++) {
			cg.GenerateLoad(makereg(nn),cg.MakeIndexed(mm,regSP),cpu.sizeOfWord,cpu.sizeOfWord);
			mm += cpu.sizeOfWord;
		}
		mm = sym->tp->btpp->type!=bt_void ? 29 : 30;
		GenerateTriadic(op_add,0,makereg(regSP),makereg(regSP),cg.MakeImmediate(mm*cpu.sizeOfWord));
	}
	else // ToDo: check pop is in reverse order to push
		for (nn = 1 + (sym->tp->btpp->type!=bt_void ? 1 : 0); nn < 31; nn++)
			GenerateMonadic(op_pop,0,makereg(nn));
}


// Push temporaries on the stack.

void BigfootCodeGenerator::SaveRegisterVars(CSet *rmask)
{
	int cnt;
	int nn;
	int64_t mask;

	if (cpu.SupportsEnter)
		return;

	if (pass == 1) {
		max_stack_use += rmask->NumMember() * cpu.sizeOfWord;
		currentFn->regvarbot = max_stack_use;
	}
	if( rmask->NumMember() ) {
		if (cpu.SupportsSTM && rmask->NumMember() > 2) {
			mask = 0;
			for (nn = nregs; nn >= 0; nn--)
				if (rmask->isMember(nn))
					mask = mask | (1LL << (nregs-1-nn-1));
			//GenerateMonadic(op_reglist, 0, cg.MakeImmediate(mask, 16));
			GenerateDiadic(op_stm, 0, cg.MakeIndirect(regSP), cg.MakeImmediate(mask, 16));
		}
		else {
			cnt = 0;
			//GenerateTriadic(op_sub, 0, makereg(regSP), makereg(regSP), cg.MakeImmediate(rmask->NumMember() * cpu.sizeOfWord));
			rmask->resetPtr();
			for (nn = 0; nn < rmask->NumMember(); nn++) {
				if (nn == 0)
					cg.GenerateStore(makereg(cpu.saved_regs[0]), cg.MakeIndirect(regSP), cpu.sizeOfWord);
				else
					cg.GenerateStore(makereg(cpu.saved_regs[nn]), cg.MakeIndexed(cpu.sizeOfWord*nn, regSP), cpu.sizeOfWord);
			}
			/*
			if (rmask->NumMember() == 1) {
				cg.GenerateStore(makereg(cpu.saved_regs[0]), cg.MakeIndirect(regSP), cpu.sizeOfWord);
			}
			else if (rmask->NumMember() == 2) {
				cg.GenerateStore(makereg(cpu.saved_regs[0]), cg.MakeIndirect(regSP), cpu.sizeOfWord);
				cg.GenerateStore(makereg(cpu.saved_regs[1]), cg.MakeIndexed(cpu.sizeOfWord,regSP), cpu.sizeOfWord);
			}
			else {
				sprintf_s(buf, sizeof(buf), "__store_s0s%d", rmask->NumMember() - 1);
				cg.GenerateMillicodeCall(cg.MakeStringAsNameConst(buf, codeseg));
			}
			*/
			/*
			for (nn = rmask->lastMember(); nn >= 0; nn = rmask->prevMember()) {
				// nn = nregs - 1 - regno
				// regno = -(nn - nregs + 1);
				// regno = nregs - 1 - nn
				cg.GenerateStore(makereg(nregs - 1 - nn), cg.MakeIndexed(cnt, regSP), cpu.sizeOfWord);
				cnt += cpu.sizeOfWord;
			}
			*/
		}
	}
}

/*
void SaveFPRegisterVars(CSet *rmask)
{
	int cnt;
	int nn;

	if( rmask->NumMember() ) {
		cnt = 0;
		GenerateTriadic(op_sub,0,makereg(regSP),makereg(regSP),cg.MakeImmediate(rmask->NumMember()*8));
		for (nn = rmask->lastMember(); nn >= 0; nn = rmask->prevMember()) {
			cg.GenerateStore(makereg(nregs - 1 - nn), cg.MakeIndexed(cnt, regSP), cpu.sizeOfWord);
//			GenerateDiadic(op_sto, 0, makereg(nregs - 1 - nn), cg.MakeIndexed(cnt, regSP));
			cnt += cpu.sizeOfWord;
		}
	}
}

void SavePositRegisterVars(CSet* rmask)
{
	int cnt;
	int nn;

	if (rmask->NumMember()) {
		cnt = 0;
		GenerateTriadic(op_sub, 0, makereg(regSP), makereg(regSP), cg.MakeImmediate(rmask->NumMember() * 8));
		for (nn = rmask->lastMember(); nn >= 0; nn = rmask->prevMember()) {
			cg.GenerateStore(makereg(nregs - 1 - nn), cg.MakeIndexed(cnt, regSP), cpu.sizeOfWord);
//			GenerateDiadic(op_psto, 0, makefpreg(nregs - 1 - nn), cg.MakeIndexed(cnt, regSP));
			cnt += cpu.sizeOfWord;
		}
	}
}
*/

// Restore registers used as register variables.

static void RestoreRegisterVars()
{
	int cnt2, cnt;
	int nn;
	int64_t mask;

	if( save_mask->NumMember()) {
		if (cpu.SupportsLDM && save_mask->NumMember() > 2) {
			mask = 0;
			for (nn = 0; nn < 32; nn++)
				if (save_mask->isMember(nn))
					mask = mask | (1LL << nn);
			GenerateMonadic(op_reglist, 0, cg.MakeImmediate(mask, 1));
			GenerateMonadic(op_ldm, 0, cg.MakeIndirect(regSP));
		}
		else {
			cnt2 = cnt = save_mask->NumMember() * cpu.sizeOfWord;
			cnt = 0;
			save_mask->resetPtr();
			for (nn = save_mask->nextMember(); nn >= 0; nn = save_mask->nextMember()) {
				cg.GenerateLoad(makereg(nn), cg.MakeIndexed(cnt, regSP), cpu.sizeOfWord, cpu.sizeOfWord);
				cnt += cpu.sizeOfWord;
			}
			GenerateTriadic(op_add, 0, makereg(regSP), makereg(regSP), cg.MakeImmediate(cnt2));
		}
	}
}

static void RestoreFPRegisterVars()
{
	int cnt2, cnt;
	int nn;

	if( fpsave_mask->NumMember()) {
		cnt2 = cnt = fpsave_mask->NumMember()*cpu.sizeOfWord;
		cnt = 0;
		fpsave_mask->resetPtr();
		for (nn = fpsave_mask->nextMember(); nn >= 0; nn = fpsave_mask->nextMember()) {
			GenerateDiadic(op_fldo, 0, makefpreg(nn), cg.MakeIndexed(cnt, regSP));
			cnt += cpu.sizeOfWord;
		}
		GenerateTriadic(op_add,0,makereg(regSP),makereg(regSP),cg.MakeImmediate(cnt2));
	}
}

// push the operand expression onto the stack.
// Structure variables are represented as an address in a register and arrive
// here as autocon nodes if on the stack. If the variable size is greater than
// 8 we assume a structure variable and we assume we have the address in a reg.
// Returns: number of stack words pushed.
//
int64_t BigfootCodeGenerator::PushArgument(ENODE *ep, int regno, int stkoffs, bool *isFloat, int* push_count, bool large_argcount)
{    
	Operand *ap, *ap1, *ap2, *ap3;
	int nn = 0;
	int sz;

	*isFloat = false;
	*push_count = 0;
	if (ep == nullptr) {
		return (0);
	}
	switch(ep->etype) {
	case bt_quad:	sz = cpu.sizeOfFPQ; break;
	case bt_double:	sz = cpu.sizeOfFPD; break;
	case bt_float:	sz = cpu.sizeOfFPD; break;
	case bt_posit:	sz = cpu.sizeOfPosit; break;
	default:	sz = cpu.sizeOfWord; break;
	}
	if (ep->tp) {
		if (ep->tp->IsFloatType())
			ap = cg.GenerateExpression(ep,am_reg,cpu.sizeOfFPQ,1);
		else if (ep->tp->IsPositType())
			ap = cg.GenerateExpression(ep, am_preg, cpu.sizeOfPosit,1);
		else
			ap = cg.GenerateExpression(ep,am_reg|am_imm,ep->GetNaturalSize(),0);
	}
	else if (ep->etype==bt_quad)
		ap = cg.GenerateExpression(ep,am_reg,sz,1);
	else if (ep->etype==bt_double)
		ap = cg.GenerateExpression(ep,am_reg,sz,1);
	else if (ep->etype==bt_float)
		ap = cg.GenerateExpression(ep,am_reg,sz,1);
	else if (ep->etype == bt_posit)
		ap = cg.GenerateExpression(ep, am_reg, sz,1);
	else
		ap = cg.GenerateExpression(ep,am_reg|am_imm,ep->GetNaturalSize(),0);
	switch(ap->mode) {
	case am_fpreg:
		*isFloat = true;
	case am_preg:
	case am_reg:
  case am_imm:
/*
        nn = roundWord(ep->esize); 
        if (nn > 8) {// && (ep->tp->type==bt_struct || ep->tp->type==bt_union)) {           // structure or array ?
            ap2 = GetTempRegister();
            GenerateTriadic(op_subui,0,makereg(regSP),makereg(regSP),MakeImmediate(nn));
            GenerateDiadic(op_mov, 0, ap2, makereg(regSP));
            GenerateMonadic(op_push,0,MakeImmediate(ep->esize));
            GenerateMonadic(op_push,0,ap);
            GenerateMonadic(op_push,0,ap2);
            GenerateMonadic(op_bsr,0,MakeStringAsNameConst("memcpy_"));
            GenerateTriadic(op_addui,0,makereg(regSP),makereg(regSP),MakeImmediate(24));
          	GenerateMonadic(op_push,0,ap2);
            ReleaseTempReg(ap2);
            nn = nn >> 3;
        }
        else {
*/
			if (regno) {
				GenerateMonadic(op_hint,0,MakeImmediate(1));
				if (ap->mode==am_imm) {
					GenerateDiadic(op_loadi,0,makereg(regno & 0xfffL), ap);
					if (regno & rt_float) {
						GenerateTriadic(op_sub,0,makereg(regSP),makereg(regSP),MakeImmediate(cpu.sizeOfWord));
						nn = 1;
					}
				}
				else if (ap->mode==am_fpreg) {
					*isFloat = true;
					GenerateDiadic(op_mov,0,makefpreg(regno & 0xfffL), ap);
					if (regno & 0x8000) {
						GenerateTriadic(op_sub,0,makereg(regSP),makereg(regSP),MakeImmediate(sz));
						nn = sz/cpu.sizeOfWord;
					}
				}
				else {
					//ap->preg = regno & 0x7fff;
					GenerateDiadic(cpu.mov_op,0,makereg(regno & 0x7fff), ap);
					if (regno & 0x8000) {
						GenerateTriadic(op_sub,0,makereg(regSP),makereg(regSP),MakeImmediate(cpu.sizeOfWord));
						nn = 1;
					}
				}
			}
			else {
				if (cpu.SupportsPush && !large_argcount) {
					if (ap->mode==am_imm) {	// must have been a zero
						if (ap->offset->i==0)
         					GenerateMonadic(op_push,0,makereg(regZero));
						else {
							ap3 = GetTempRegister();
							GenerateLoadConst(ap, ap3);
							GenerateMonadic(op_push,0,ap3);
							ReleaseTempReg(ap3);
						}
						nn = 1;
						*push_count = 1;
					}
					else {
						if (ap->typep==&stddouble) 
						{
							*isFloat = true;
							GenerateMonadic(op_push,0,ap);
							nn = sz/cpu.sizeOfWord;
							nn = 1;
							*push_count = 1;
						}
						else {
							regs[ap->preg].IsArg = true;
							GenerateMonadic(op_push,0,ap);
							nn = 1;
							*push_count = 1;
						}
					}
				}
				else {
					if (ap->mode==am_imm) {	// must have been a zero
						ap3 = nullptr;
						if (ap->offset->i!=0) {
							GenerateStoreImmediate(ap, MakeIndexed(stkoffs, regSP), cpu.sizeOfWord);
						}
						else {
							cg.GenerateStore(makereg(0), MakeIndexed(stkoffs, regSP), cpu.sizeOfWord);
						}
						nn = 1;
					}
					else {
						// For aggregate types larger than the word size, a pointer to a buffer
						// is pushed instead of the actual value. The buffer will have been 
						// allocated by the caller.
						// What needs to be done is copy the aggregate to the buffer then push
						// the buffer address.
						if (ap->tp->IsAggregateType() && ap->tp->size > cpu.sizeOfWord) {
							ap2 = GetTempRegister();
							GenerateDiadic(op_lea, 0, ap2, MakeIndexed(ep->stack_offs, regSP));	// push target
							cg.GenerateStore(ap2, MakeIndexed(cpu.sizeOfWord, regSP), cpu.sizeOfWord);	
							ReleaseTempRegister(ap2);
							cg.GenerateStore(ap, MakeIndexed((int64_t)0, regSP), cpu.sizeOfWord);		// and source
							ap3 = GetTempRegister();
							GenerateLoadConst(MakeImmediate(ap->tp->size), ap3);								// and size
							cg.GenerateStore(ap3, MakeImmediate(ap->tp->size), cpu.sizeOfWord);
							ReleaseTempRegister(ap3);
							GenerateMonadic(op_bsr, 0, MakeStringAsNameConst((char *)"__aacpy", codeseg));	// call copy helper
							ap1 = GetTempRegister();
							GenerateLoadConst(MakeImmediate(ep->stack_offs), ap1);							// and size
							cg.GenerateStore(ap1, MakeIndexed(stkoffs, regSP), cpu.sizeOfWord);
							ReleaseTempRegister(ap1);
						}
						else if (ap->tp->IsFloatType()) {
							*isFloat = true;
							cg.GenerateStore(ap,MakeIndexed(stkoffs,regSP),cpu.sizeOfWord);
							nn = 1;// sz / cpu.sizeOfWord;
						}
						else if (ap->type==bt_posit) {
							cg.GenerateStore(ap, MakeIndexed(stkoffs, regSP), cpu.sizeOfWord);
							nn = 1;
						}
						else if (ap->type == bt_vector) {
							cg.GenerateStore(ap, MakeIndexed(stkoffs, regSP), cpu.sizeOfWord);
							nn = 4;
						}
						else {
							regs[ap->preg].IsArg = true;
							cg.GenerateStore(ap,MakeIndexed(stkoffs,regSP),cpu.sizeOfWord);
							nn = 1;
						}
					}
				}
			}
//        }
    	break;
    }
	ReleaseTempReg(ap);
	return (nn);
}

// Store entire argument list onto stack
// large_arg_count is used only if push is supported. It is less expensive to
// use a push instruction rather than subtracting from the sp and using stores
// if there are only a small number of arguments (<3).
//
int64_t BigfootCodeGenerator::PushArguments(Function *sym, ENODE *plist)
{
	TypeArray *ta = nullptr;
	int64_t i,sum;
	OCODE *ip;
	ENODE *p;
	ENODE *pl[100];
	int64_t nn, maxnn;
	int pc;
	int push_count;
	bool isFloat = false;
	bool sumFloat;
	bool large_argcount = false;
	Symbol** sy = nullptr;
	int64_t stkoffs;
	int regno;

	sum = 0;
	push_count = 0;
	if (sym)
		ta = sym->GetProtoTypes();

	sumFloat = false;
	ip = currentFn->pl.tail;

	// Allocate stack space for arguments.
	if (sym) {
//		if (sym->arg_space > 0)
			GenerateSubtractFrom(makereg(regSP), MakeImmediate(sym->arg_space));
		sy = sym->params.GetParameters();
	}
	// Subtract zero from the SP, this value will be patched up to the correct
	// value after all the arguments are pushed. (The position in the peep list
	// was recorded above).
	else
		GenerateSubtractFrom(makereg(regSP), MakeImmediate(0));


	// Capture the parameter list. It is needed in the reverse order.
	for (nn = 0, p = plist; p != NULL; p = p->p[1], nn++) {
		pl[nn] = p->p[0];
	}
	// ToDo: fix order of push instructions, they are coming out in the reverse order
	// to what is needed. So always use store instructions for now.
	if (nn > 4)
		large_argcount = true;
	large_argcount = true;	// disable for now
	maxnn = nn;
	for(nn = large_argcount ? nn-1 : 0, i = 0; large_argcount ? nn >= 0 : nn < maxnn; large_argcount ? nn-- : nn++,i++ )
  {
		if (pl[nn]->etype == bt_pointer) {
			if (pl[nn]->tp->btpp == nullptr) {
				sum++;
				continue;
			}
			if (pl[nn]->tp->btpp->type == bt_ichar || pl[nn]->tp->btpp->type == bt_iuchar)
				continue;
		}
				//		sum += GeneratePushParameter(pl[nn],ta ? ta->preg[ta->length - i - 1] : 0,sum*8);
		// Variable argument list functions may cause the type array values to be
		// exhausted before all the parameters are pushed. So, we check the parm number.
		if (pl[nn]->etype == bt_none) {	// was there an empty parameter?
			if (sy != nullptr) {
				if (sy[nn]) {
					sum += PushArgument(sy[nn]->defval, ta ? (i < ta->length ? ta->preg[i] : 0) : 0, sy[nn] ? sy[nn]->value.i : sum * cpu.sizeOfWord, &isFloat, &pc, large_argcount);
					push_count += pc;
				}
				else {
					error(ERR_MISSING_PARM);
				}
			}
		}
		else {
			if (sy != nullptr) {
				if (sy[nn]) {
					regno = ta ? (i < ta->length ? ta->preg[i] : 0) : 0;
//					stkoffs = sy[nn] ? sy[nn]->value.i : sum * cpu.sizeOfWord;
					stkoffs = sum * cpu.sizeOfWord;
					sum += PushArgument(pl[nn], regno, stkoffs, &isFloat, &pc, large_argcount);
					push_count += pc;
				}
				else {
					//error(ERR_MISSING_PARM);
					regno = ta ? (i < ta->length ? ta->preg[i] : 0) : 0;
					stkoffs = sum * cpu.sizeOfWord;
					sum += PushArgument(pl[nn], regno, stkoffs, &isFloat, &pc, large_argcount);
					push_count += pc;
				}
			}
			else {
				sum += PushArgument(pl[nn], ta ? (i < ta->length ? ta->preg[i] : 0) : 0, sum * cpu.sizeOfWord, &isFloat, &pc, large_argcount);
				push_count += pc;
			}
		}
		sumFloat |= isFloat;
//		plist = plist->p[1];
  }
	if (sum == 0)
		ip->fwd->MarkRemove();
	else
		ip->fwd->oper3 = MakeImmediate(sum * cpu.sizeOfWord);

		/*
	if (sum == 0 || !large_argcount)
		ip->fwd->MarkRemove();
	else
		ip->fwd->oper3 = MakeImmediate(sum*cpu.sizeOfWord);
	*/
	/*
	if (!sumFloat) {
		o_supportsPush = cpu.SupportsPush;
		cpu.SupportsPush = false;
		currentFn->pl.tail = ip;
		currentFn->pl.tail->fwd = nullptr;
		i = maxnn-1;
		for (nn = 0; nn < maxnn; nn++, i--) {
			if (pl[nn]->etype == bt_pointer)
				if (pl[nn]->tp->btpp->type == bt_ichar || pl[nn]->tp->btpp->type == bt_iuchar)
					continue;
			if (pl[nn]->etype == bt_none) {	// was there an empty parameter?
				if (sy == nullptr && sym)
					sy = sym->params.GetParameters();
				if (sy)
					PushArgument(sy[nn]->defval, ta ? (i < ta->length ? ta->preg[i] : 0) : 0, sum * cpu.sizeOfWord, &isFloat);
			}
			else
				PushArgument(pl[nn], ta ? (i < ta->length ? ta->preg[i] : 0) : 0, sum * 8, &isFloat);
		}
		cpu.SupportsPush = o_supportsPush;
	}
	*/
	if (ta)
		delete ta;
	if (sy)
		delete[] sy;
  return (sum);
}

// Pop parameters off the stack

void BigfootCodeGenerator::PopArguments(Function *fnc, int howMany, bool isPascal)
{
	howMany *= cpu.sizeOfWord;
	if (howMany != 0) {
		if (fnc) {
			if (!fnc->IsPascal)
//				GenerateAddOnto(makereg(regSP), MakeImmediate(fnc->arg_space));
				GenerateAddOnto(makereg(regSP), MakeImmediate(howMany));
			else if (howMany - (fnc->NumFixedAutoParms * cpu.sizeOfWord) > 0)
//				GenerateAddOnto(makereg(regSP), MakeImmediate(fnc->arg_space - (fnc->NumFixedAutoParms * cpu.sizeOfWord)));
				GenerateAddOnto(makereg(regSP), MakeImmediate(howMany - (fnc->NumFixedAutoParms * cpu.sizeOfWord)));
		}
		else {
//			error(ERR_UNKNOWN_FN);
			if (!isPascal)
				GenerateAddOnto(makereg(regSP), MakeImmediate(howMany));
		}
	}
}


void BigfootCodeGenerator::LinkAutonew(ENODE *node)
{
	if (node->isAutonew) {
		currentFn->hasAutonew = true;
	}
}

void BigfootCodeGenerator::GenerateDirectJump(ENODE* node, Operand* ap, Function* sym, int flags, int lab)
{
	char buf[500];

	if (sym && sym->IsLeaf) {
		sprintf_s(buf, sizeof(buf), "%s_ip", sym->sym->name->c_str());
		if (flags & am_jmp)
			GenerateMonadic(sym->sym->storage_class == sc_static ? op_branch : op_branch, 0, MakeDirect(node->p[0]));
		else
			GenerateMonadic(sym->sym->storage_class == sc_static ? op_bsr : op_bsr, 0, MakeDirect(node->p[0]));
		currentFn->doesJAL = true;
	}
	else if (sym) {
		if (flags & am_jmp)
			GenerateMonadic(sym->sym->storage_class == sc_static ? op_branch : op_branch, 0, MakeDirect(node->p[0]));
		else
			GenerateMonadic(sym->sym->storage_class == sc_static ? op_bsr : op_bsr, 0, MakeDirect(node->p[0]));
		currentFn->doesJAL = true;
	}
	else {
		if (flags & am_jmp)
			GenerateMonadic(op_branch, 0, MakeDirect(node->p[0]));
		else
			GenerateMonadic(op_bsr, 0, MakeDirect(node->p[0]));
		currentFn->doesJAL = true;
	}
	GenerateMonadic(op_bex, 0, MakeDataLabel(throwlab, regZero));
	if (lab)
		GenerateLabel(lab);
	LinkAutonew(node);
}

void BigfootCodeGenerator::GenerateIndirectJump(ENODE* node, Operand* ap, Function* sym, int flags, int lab)
{
	ap->MakeLegal(am_ind, cpu.sizeOfWord);
	if (sym && sym->IsLeaf) {
		if (flags & am_jmp)
			GenerateMonadic(op_jmp, 0, ap);
		else
			GenerateMonadic(op_jsr, 0, ap);
		currentFn->doesJAL = true;
	}
	else {
		if (flags & am_jmp)
			GenerateMonadic(op_jmp, 0, ap);
		else
			GenerateMonadic(op_jsr, 0, ap);
		currentFn->doesJAL = true;
	}
	GenerateMonadic(op_bex, 0, MakeDataLabel(throwlab, regZero));
	if (lab)
		GenerateLabel(lab);
	LinkAutonew(node);
}

void BigfootCodeGenerator::GenerateUnlink(int64_t amt)
{
	if (cpu.SupportsLeave) {
		GenerateDiadic(op_leave, 0, MakeImmediate(currentFn->mask->NumMember()), MakeImmediate(amt,0));
	}
	else if (cpu.SupportsUnlink)
		GenerateZeradic(op_unlk);
	else
	{
		//GenerateDiadic(cpu.mov_op, 0, makereg(regSP), makereg(regFP));
		//GenerateDiadic(cpu.ldo_op, 0, makereg(regFP), MakeIndirect(regSP));
	}
}

// The compiler makes use of local labels for the switch table. The data table label must also be local.

void BigfootStatementGenerator::GenerateTabularSwitch(Statement* stmt, int64_t minv, int64_t maxv, Operand* ap, bool HasDefcase, int deflbl, int tablabel)
{
	Operand* ap2, * ap3;

	stmt->tabular = true;
	ap2 = GetTempRegister();
	ap3 = GetTempCrRegister();
	if (minv != 0)
		GenerateTriadic(op_sub, 0, ap, ap, MakeImmediate(minv));
	GenerateTriadic(op_cmp, 0, ap3, ap, MakeImmediate(maxv - minv + 1));
	GenerateDiadic(op_bgeu, 0, ap3, MakeCodeLabel(HasDefcase ? deflbl : breaklab));
	ReleaseTempRegister(ap3);
	ReleaseTempRegister(ap2);
	GenerateTriadic(op_asl, 0, ap, ap, MakeImmediate(3));
	//GenerateDiadic(op_ldt, 0, ap, compiler.of.MakeIndexedCodeLabel(tablabel, ap->preg));
	cg.GenerateLoad(ap, compiler.of.MakeIndexedDataLabel(tablabel, ap->preg), 8, 8);
	GenerateMonadic(op_jmp, 0, MakeIndirect(ap->preg));
	ReleaseTempRegister(ap);
	GenerateSwitchStatements(stmt);
}

void BigfootStatementGenerator::GenerateNakedTabularSwitch(Statement* stmt, int64_t minv, Operand* ap, int tablabel)
{
	if (minv != 0)
		GenerateTriadic(op_sub, 0, ap, ap, MakeImmediate(minv));
	GenerateTriadic(op_asl, 0, ap, ap, MakeImmediate(3));
	//	GenerateDiadic(cpu.ldo_op, 0, ap, compiler.of.MakeIndexedCodeLabel(tablabel, ap->preg));
//	GenerateDiadic(op_ldt, 0, ap, compiler.of.MakeIndexedName((char*)stmt->GenerateSwitchTargetName(tablabel).c_str(), ap->preg)); // MakeIndexedCodeLabel(tablabel, ap->preg));
	cg.GenerateLoad(ap, compiler.of.MakeIndexedDataLabel(tablabel, ap->preg), 8, 8);
	//cg.GenerateLoad(ap, compiler.of.MakeIndexedName((char*)stmt->GenerateSwitchTargetName(tablabel).c_str(), ap->preg), 4, 4);
	GenerateMonadic(op_jmp, 0, MakeIndirect(ap->preg));
	ReleaseTempRegister(ap);
	GenerateSwitchStatements(stmt);
}

//
// Generate a jump to label if the node passed evaluates to
// a true condition.
//
void BigfootCodeGenerator::GenerateTrueJump(ENODE* node, int64_t label, unsigned int prediction)
{
	Operand* ap1, * ap2;
	int64_t lab0;
	int64_t siz1;

	if (node == 0)
		return;
	switch (node->nodetype)
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
		ap1 = GenerateExpression(node, am_reg, siz1, 1);
		//                        GenerateDiadic(op_tst,siz1,ap1,0);
		ReleaseTempRegister(ap1);
		if (ap1->tp->IsFloatType()) {
			ap2 = GetTempRegister();
			GenerateTriadic(op_fcmp, 0, ap2, ap1, makereg(regZero));
			GenerateTriadic(op_bbs, 0, ap2, MakeImmediate(0), MakeCodeLabel(label));	// bit 0 is eq
			ReleaseTempReg(ap2);
		}
		else {
			if (IsArgReg(ap1->preg))
				GenerateDiadic(op_bnez, 0, ap1, MakeCodeLabel(label));
			else {
				//ap2 = MakeBoolean(ap1);
				//ReleaseTempReg(ap1);
				GenerateBranchTrue(ap1, label);
			}
		}
		break;
	}
}

// Generate code to execute a jump to label if the expression
// passed is false.
//
void BigfootCodeGenerator::GenerateFalseJump(ENODE* node, int64_t label, unsigned int prediction)
{
	int64_t siz1;
	int64_t lab0;
	Operand* ap;

	if (node == (ENODE*)NULL)
		return;
	switch (node->nodetype)
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
		GenerateFalseJump(node->p[0], label, prediction ^ 1);
		GenerateFalseJump(node->p[1], label, prediction ^ 1);
		break;
	case en_lor_safe:
		if (GenerateBranch(node, op_nor, label, 0, prediction, true))
			break;
	case en_lor:
		lab0 = nextlabel++;
		GenerateTrueJump(node->p[0], lab0, prediction);
		GenerateFalseJump(node->p[1], label, prediction ^ 1);
		GenerateLabel(lab0);
		break;
	case en_not:
		GenerateTrueJump(node->p[0], label, prediction);
		break;
	default:
		siz1 = node->GetNaturalSize();
		ap = GenerateExpression(node, am_reg, siz1, 1);
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
					//ap1 = MakeBoolean(ap);
					//ReleaseTempReg(ap);
					GenerateBranchFalse(ap, label);
				}
			}
		}
		break;
	}
}

void BigfootCodeGenerator::GenerateLoadFloat(Operand* ap3, Operand* ap1, int64_t ssize, int64_t size, Operand* mask)
{
	if (ap3->typep == &stdflt) {
		GenerateTriadic(op_fload, 'd', ap3, ap1, mask);
	}
	else if (ap3->typep == &stddouble) {
		GenerateTriadic(op_fload, 'd', ap3, ap1, mask);
	}
	else if (ap3->typep == &stdquad) {
		GenerateTriadic(op_fload, 'q', ap3, ap1, mask);
	}
	else if (ap3->typep == &stdsingle) {
		GenerateTriadic(op_fload, 's', ap3, ap1, mask);
	}
	else if (ap3->typep == &stdhalf) {
		GenerateTriadic(op_fload, 'h', ap3, ap1, mask);
	}
	//	else
	//		throw C64PException(ERR_UNKNOWN_FLOAT_TYPE, 0);
}

void BigfootCodeGenerator::GenerateInterruptSave(Function* func)
{
	int nn, kk, last_reg;
	int64_t tsm = func->int_save_mask;

	nn = popcnt(tsm);
	// Allocate storage for registers on stack
	GenerateSubtractFrom(makereg(regSP), MakeImmediate(nn * cpu.sizeOfWord));
	for (kk = nn = 0; nn < 63; nn++) {
		/*
		if ((tsm & 15) == 15 && ((nn % 4) == 0)) {
			GenerateDiadic(op_storeg, 0, makereg((nn / 4) | rt_group), MakeIndexed(kk * cpu.sizeOfWord, regSP));
			kk += 4;
			tsm = tsm >> 4;
			nn += 3;
		}
		else 
		*/
		if (tsm & 1) {
			GenerateStore(makereg(nn), MakeIndexed(kk * cpu.sizeOfWord, regSP), cpu.sizeOfWord);
			last_reg = nn;
			kk++;
		}
		tsm = tsm >> 1;
	}
	// Always save the condition register group
	GenerateMove(makereg(last_reg), makecreg(0xfffL | rt_cr));
	GenerateStore(makereg(last_reg), MakeIndexed(kk * cpu.sizeOfWord, regSP), cpu.sizeOfWord);
	/*
	if (DoesContextSave) {
		for (kk = 0; kk < 16; kk++)
			GenerateDiadic(op_storeg, 0, makereg(kk | rt_group), MakeIndexed(kk * cpu.sizeOfWord * 4, regTS));
	}
	*/
	/*
	if (sp_init) {
		GenerateLoadConst(MakeImmediate(sp_init), makereg(regSP));
	}
	*/
	/*
	if (stkname) {
		GenerateDiadic(op_lea, 0, makereg(SP), MakeStringAsNameConst(stkname,dataseg));
		GenerateTriadic(op_ori, 0, makereg(SP), makereg(SP), MakeImmediate(0xFFFFF00000000000LL));
	}
	*/
}

void BigfootCodeGenerator::GenerateInterruptLoad(Function* func)
{
	int nn, kk, first_reg, first = 1;
	int64_t tsm = func->int_save_mask;

	if (DoesContextSave) {
		for (kk = 0; kk < 16; kk++)
			GenerateDiadic(op_loadg, 0, makereg(kk | rt_group), MakeIndexed(kk * cpu.sizeOfWord * 4, regTS));
	}

	nn = popcnt(tsm);
	for (kk = nn = 0; nn < 63; nn++) {
		/*
		if ((tsm & 15) == 15 && ((nn % 4) == 0)) {
			GenerateDiadic(op_loadg, 0, makereg((nn >> 2) | rt_group), MakeIndexed(kk * cpu.sizeOfWord, regSP));
			kk += 4;
			nn += 3;
			tsm = tsm >> 4;
		}
		else
		*/
		if (tsm & 1) {
			// Always save the condition register group
			if (first) {
				GenerateLoad(makereg(nn), MakeIndexed(kk * cpu.sizeOfWord, regSP), cpu.sizeOfWord, cpu.sizeOfWord);
				GenerateMove(makecreg(0xfffL | rt_cr), makereg(nn));
				first = 0;
			}
			GenerateLoad(makereg(nn), MakeIndexed(kk * cpu.sizeOfWord, regSP), cpu.sizeOfWord, cpu.sizeOfWord);
			kk++;
		}
		tsm = tsm >> 1;
	}
	// Deallocate stack storage
	GenerateAddOnto(makereg(regSP), MakeImmediate(kk * cpu.sizeOfWord));
}

void BigfootCodeGenerator::GenerateLoadConst(Operand* ap1, Operand* ap2)
{
	Operand* ap3;

	ap1->isConst = true;
	if (ap1->isPtr) {
		ap3 = ap1->Clone();
		ap3->mode = am_direct;
		GenerateLoadAddress(ap2, ap3);
		//if (!compiler.os_code) {
		//	switch (ap1->segment) {
		//	case tlsseg:		GenerateTriadic(op_base, 0, ap2, ap2, MakeImmediate(8));	break;
		//	case rodataseg:	GenerateTriadic(op_base, 0, ap2, ap2, MakeImmediate(12));	break;
		//	}
		//}
	}
	else {
		OCODE* ip;
		if (ap1->offset == nullptr)
			;
		//		if (ap1->offset->esize <= 8)
		//			ip = GenerateDiadic(cpu.ldi_op, 0, ap2, MakeImmediate(ap1->offset->i));
		//		else 
		{
			// Try to compress a float into the smallest representation.
			if (ap1->tp->IsFloatType())
				ip = GenerateLoadFloatConst(ap1, ap2);
			else {
				if (ap1->offset) {
					ip = GenerateDiadic(op_loadi, 0, ap2, MakeImmediate(ap1->offset->i128.low));
				}
				else {
					error(1000);	// NULL pointer
					ip = GenerateDiadic(op_loadi, 0, ap2, MakeImmediate(0xdeadbeefLL));
				}
			}
		}
		if (ip->oper2) {
			ip->oper2->isConst = true;
			if (ip->oper2->offset)
				ip->oper2->offset->constflag = true;
		}
		regs[ap2->preg].isConst = true;
		if (ap2->tp) {
			//				ap2->tp->type = bt_long;
			//				ap2->tp->size = 16;
		}
	}
	// ap2 inherits type from ap1
//	ap2->tp = ap1->tp;
	regs[ap2->preg].offset = ap1->offset;
}

// For now, just assume the data pointers have been set at program start-up.

void BigfootCodeGenerator::GenerateLoadDataPointer()
{
	Operand* ap = GetTempRegister();

	return;
	if (address_bits > 24) {
		cg.GenerateLoadAddress(makereg(regGP), MakeStringAsNameConst((char*)"<_start_data", dataseg));
		GenerateDiadic(op_orm, 0, makereg(regGP), MakeStringAsNameConst((char*)"_start_data", dataseg));
		if (address_bits > 48)
			GenerateDiadic(op_orh, 0, makereg(regGP), MakeStringAsNameConst((char*)">_start_data", dataseg));
	}
	else
		cg.GenerateLoadAddress(makereg(regGP), MakeStringAsNameConst((char*)"_start_data", dataseg));
	ReleaseTempRegister(ap);
}

// Compiler now uses global pointer two addressing for the rodataseg
void BigfootCodeGenerator::GenerateLoadRodataPointer()
{
	Operand* ap = GetTempRegister();

	return;
	//cg.GenerateLoadConst(MakeStringAsNameConst("__rodata_base", dataseg), ap);
//	cg.GenerateLoadAddress(makereg(regGP1), MakeStringAsNameConst((char*)currentFn->sym->name->c_str(), codeseg));
	if (address_bits > 24) {
		cg.GenerateLoadAddress(makereg(regGP), MakeStringAsNameConst((char*)"<_start_rodata", rodataseg));
		GenerateDiadic(op_orm, 0, makereg(regGP), MakeStringAsNameConst((char*)"_start_rodata", rodataseg));
		if (address_bits > 48)
			GenerateDiadic(op_orh, 0, makereg(regGP), MakeStringAsNameConst((char*)">_start_rodata", rodataseg));
	}
	else
		cg.GenerateLoadAddress(makereg(regGP), MakeStringAsNameConst((char*)"_start_rodata", rodataseg));
	//cg.GenerateLoadAddress(makereg(regGP1), MakeStringAsNameConst((char *)"_start_rodata", dataseg));
	//if (!compiler.os_code)
	//GenerateTriadic(op_base, 0, makereg(regGP1), makereg(regGP1), ap);
	ReleaseTempRegister(ap);
}

void BigfootCodeGenerator::GenerateLoadBssPointer()
{
	Operand* ap = GetTempRegister();

	return;
	if (address_bits > 24) {
		cg.GenerateLoadAddress(makereg(regGP1), MakeStringAsNameConst((char*)"<_start_bss", bssseg));
		GenerateDiadic(op_orm, 0, makereg(regGP1), MakeStringAsNameConst((char*)"_start_bss", bssseg));
		if (address_bits > 48)
			GenerateDiadic(op_orh, 0, makereg(regGP1), MakeStringAsNameConst((char*)">_start_bss", bssseg));
	}
	else
		cg.GenerateLoadAddress(makereg(regGP1), MakeStringAsNameConst((char*)"_start_bss", bssseg));
	ReleaseTempRegister(ap);
}

void BigfootCodeGenerator::GenerateSmallDataRegDecl()
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

void BigfootCodeGenerator::GenerateSignExtendByte(Operand* tgt, Operand* src)
{
	GenerateDiadic(op_movsxb, 0, tgt, src);
}

void BigfootCodeGenerator::GenerateSignExtendWyde(Operand* tgt, Operand* src)
{
	GenerateDiadic(op_movsxw, 0, tgt, src);
}

void BigfootCodeGenerator::GenerateSignExtendTetra(Operand* tgt, Operand* src)
{
	GenerateDiadic(op_movsxt, 0, tgt, src);
}

void BigfootCodeGenerator::GenerateReturnAndDeallocate(Operand* ap1)
{
	GenerateMonadic(op_retd, 0, ap1);
}

void BigfootCodeGenerator::GenerateReturnAndDeallocate(int64_t amt)
{
	GenerateMonadic(op_retd, 0, MakeImmediate(amt));
}

void BigfootCodeGenerator::GenerateLoadAddress(Operand* ap3, Operand* ap1)
{
	Operand* ap2, * ap4;

	if (address_bits > 24)
		ap1->lowhigh = 1;
	GenerateDiadic(op_lda, 0, ap3, ap1);
	/*
	if (address_bits > 24) {
		ap2 = ap1->Clone();
		ap2->lowhigh = 2;
		ap2->mode = am_imm;
		if (!ap2->offset->i128.IsNBit(address_bits))
			GenerateDiadic(op_orm, 0, ap3, ap2);
	}
	if (address_bits > 48) {
		ap4 = ap1->Clone();
		ap4->lowhigh = 2;
		ap4->mode = am_imm;
		if (!ap4->offset->i128.IsNBit(address_bits))
			GenerateDiadic(op_orh, 0, ap3, ap4);
	}
	*/
}

// Generate load or store operation taking into consideration the number of
// address bits required. If too many bits are needed turn the load or store
// into a load of the address into a register, then use the register.

void BigfootCodeGenerator::GenerateLoadStore(e_op opcode, Operand* ap1, Operand* ap2)
{
	// If fewer than 22 bits addressing, nothing to worry about.
	if (address_bits < 22) {
		GenerateDiadic(opcode, 0, ap1, ap2);
		return;
	}
	// Only two modes that have issues: direct and register indirect with disp.
	if (ap2->mode == am_indx) {
		if (ap2->preg == 0)
			ap2->mode = am_direct;
	}
	if (ap2->mode == am_direct || ap2->mode==am_direct2) {
		if (ap2->offset == nullptr || ap2->offset2 != nullptr || ap2->mode == am_direct2 || !ap2->offset->i128.IsNBit(21)) {
			Operand* ap4;
			ap4 = GetTempRegister();
			GenerateLoadAddress(ap4, ap2);
			GenerateDiadic(opcode, 0, ap1, MakeIndirect(ap4->preg));
			return;
		}
		GenerateDiadic(opcode, 0, ap1, ap2);
		return;
	}
	else if (ap2->mode == am_indx) {
		if (ap2->offset == nullptr || ap2->offset2!=nullptr || !ap2->offset->i128.IsNBit(21)) {
			Operand* ap4;
			ap4 = GetTempRegister();
			if (ap2->offset) {
				GenerateLoadConst(MakeImmediate(ap2->offset->i128), ap4);
				ap2->mode = am_indx2;
				ap2->offset = nullptr;
				ap2->sreg = ap4->preg;
				GenerateDiadic(opcode, 0, ap1, ap2);
				return;
			}
			GenerateLoadConst(ap2, ap4);
			ap2->mode = am_indx2;
			ap2->offset = nullptr;
			ap2->sreg = ap4->preg;
			GenerateDiadic(opcode, 0, ap1, ap4);
			return;
		}
		GenerateDiadic(opcode, 0, ap1, ap2);
		return;
	}
	else
		GenerateDiadic(opcode, 0, ap1, ap2);
}

void BigfootCodeGenerator::GenerateLoad(Operand* ap3, Operand* ap1, int64_t ssize, int64_t size, Operand* mask)
{
	if (ap3->typep == &stdposit) {
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
	else if (ap3->typep == &stdvector) {
		GenerateTriadic(op_load, 0, ap3, ap1, mask);
	}
	else if (ap3->typep->IsFloatType())
		GenerateLoadFloat(ap3, ap1, ssize, size, mask);
	//else if (ap3->mode == am_fpreg) {
	//	GenerateTriadic(op_fldo, 0, ap3, ap1);
	//}
	else if (ap3->isUnsigned) {
		// If size is zero, probably a pointer to void being processed.
		switch (size) {
		case 0: GenerateLoadStore(op_ldbu, ap3, ap1); break;
		case 1:	GenerateLoadStore(op_ldbu, ap3, ap1); break;
		case 2:	GenerateLoadStore(op_ldwu, ap3, ap1); break;
		case 4:	GenerateLoadStore(op_ldtu, ap3, ap1); break;
		case 8: GenerateLoadStore(op_ldou, ap3, ap1); break;
		case 16:	GenerateLoadStore(op_ldh, ap3, ap1); break;
		case 64:	GenerateLoadStore(op_load, ap3, ap1); break;
		}
	}
	else {
		switch (size) {
		case 0: GenerateLoadStore(op_ldb, ap3, ap1); break;
		case 1:	GenerateLoadStore(op_ldb, ap3, ap1); break;
		case 2:	GenerateLoadStore(op_ldw, ap3, ap1); break;
		case 4:	GenerateLoadStore(op_ldt, ap3, ap1); break;
		case 8:	GenerateLoadStore(op_load, ap3, ap1); break;
		case 16: GenerateLoadStore(op_ldh, ap3, ap1); break;
		case 64:	GenerateLoadStore(op_load, ap3, ap1); break;
		}
	}
	ap3->memref = true;
	ap3->memop = ap1->Clone();
}

void BigfootCodeGenerator::GenerateStore(Operand* ap1, Operand* ap3, int64_t size, Operand* mask)
{
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
	if (ap3->typep == &stdposit) {
		GenerateTriadic(op_sto, 0, ap1, ap3, mask);
	}
	else if (ap1->tp->IsVectorType())//typep == &stdvector)
		GenerateTriadic(op_store, 0, ap1, ap3, mask);
	else if (ap1->typep == &stdflt) {
		GenerateTriadic(op_store, 0, ap1, ap3, mask);
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
		switch (size) {
		case 1: GenerateLoadStore(op_stb, ap1, ap3); break;
		case 2: GenerateLoadStore(op_stw, ap1, ap3); break;
		case 4: GenerateLoadStore(op_stt, ap1, ap3); break;
		case 8:	GenerateLoadStore(op_store, ap1, ap3); break;
		case 16:	GenerateLoadStore(op_sth, ap1, ap3); break;
		case 64:  GenerateLoadStore(op_store, ap1, ap3); break;
		default:
			;
		}
	}
}

void BigfootCodeGenerator::GenerateStoreImmediate(Operand* val, Operand* dst, int64_t size)
{
	e_op opcode;

	if (dst->mode == am_indx && dst->preg == regSP) {
		if (val->offset->i128.Int128::IsNBit(8))
			opcode = op_stib;
		if (val->offset->i128.Int128::IsNBit(16))
			opcode = op_stiw;
		else if (val->offset->i128.Int128::IsNBit(32))
			opcode = op_stit;
		else
			opcode = op_stio;
	}
	switch (size) {
	case 1:	GenerateDiadic(opcode, 'b', val, dst); break;
	case 2:	GenerateDiadic(opcode, 'w', val, dst); break;
	case 4:	GenerateDiadic(opcode, 't', val, dst); break;
	case 8:	GenerateDiadic(opcode, 'o', val, dst); break;
	}
}

Operand* BigfootCodeGenerator::GenerateFloatcon(ENODE* node, int flags, int64_t size)
{
	Operand* ap1;

	// Code for generating a reference to the constant which is 
	// stored in rodata.
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
			ap1->preg = GetSegmentIndexReg((e_sg)node->segment);
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
}

Operand* BigfootCodeGenerator::GenPositcon(ENODE* node, int flags, int64_t size)
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

Operand* BigfootCodeGenerator::GenLabelcon(ENODE* node, int flags, int64_t size)
{
	Operand* ap1, * ap2;

	if (use_gp) {
		ap1 = GetTempRegister();
		ap2 = allocOperand();
		ap2->mode = am_indx;
		ap2->preg = GetSegmentIndexReg((e_sg)node->segment);
		ap2->offset = node;     // use as constant node
		GenerateLoadAddress(ap1, ap2);
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

Operand* BigfootCodeGenerator::GenNacon(ENODE* node, int flags, int64_t size)
{
	Operand* ap1, * ap2;

	if (use_gp) {
		ap1 = GetTempRegister();
		ap2 = allocOperand();
		ap2->mode = am_indx;
		ap2->preg = GetSegmentIndexReg((e_sg)node->segment);
		ap2->offset = node;     // use as constant node
		if (node)
			DataLabels[node->i]++;
		GenerateLoadAddress(ap1, ap2);
		ap1->MakeLegal(flags, size);
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
	return (ap1);
}

int BigfootCodeGenerator::GetSegmentIndexReg(e_sg segment)
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
OCODE* BigfootCodeGenerator::GenerateReturnBlock(Function* fn)
{
	Operand* ap, * ap2;
	int n;
	char buf[300];
	OCODE* ip;

	ip = nullptr;
	fn->alstk = false;
	if (!cpu.SupportsEnter)
		GenerateMonadic(op_hint, 0, MakeImmediate(begin_return_block));
	if (cpu.SupportsEnter)
	{
		if (fn->stkspace < 8388607LL) {
			GenerateDiadic(op_enter, 0, MakeImmediate(15), MakeImmediate(-fn->tempbot));
			ip = currentFn->pl.tail;
			//			GenerateMonadic(op_link, 0, MakeImmediate(stkspace));
						//spAdjust = pl.tail;
			fn->alstk = true;
		}
		else {
			GenerateDiadic(op_enter, 0, MakeImmediate(15), MakeImmediate(8388600LL));
			ip = currentFn->pl.tail;
			GenerateTriadic(op_subtract, 0, makereg(regSP), makereg(regSP), MakeImmediate(-fn->tempbot - 8388600LL));
			//GenerateMonadic(op_link, 0, MakeImmediate(SizeofReturnBlock() * cpu.sizeOfWord));
			fn->alstk = true;
		}
	}
	else if (cpu.SupportsLink) {
		if (fn->stkspace < 8388607LL - Compiler::GetReturnBlockSize()) {
			GenerateMonadic(op_link, 0, MakeImmediate(Compiler::GetReturnBlockSize() + fn->stkspace));
			//			GenerateMonadic(op_link, 0, MakeImmediate(stkspace));
						//spAdjust = pl.tail;
			fn->alstk = true;
		}
		else {
			GenerateMonadic(op_link, 0, MakeImmediate(8388600LL));
			GenerateTriadic(op_sub, 0, makereg(regSP), makereg(regSP), MakeImmediate(Compiler::GetReturnBlockSize() + fn->stkspace - 8388600LL));
			//GenerateMonadic(op_link, 0, MakeImmediate(SizeofReturnBlock() * cpu.sizeOfWord));
			fn->alstk = true;
		}
	}
	else {
		GenerateTriadic(op_sub, 0, makereg(regSP), makereg(regSP), MakeImmediate(cpu.ReturnBlockSize()-cpu.sizeOfWord));
		cg.GenerateStore(makereg(regFP), MakeIndirect(regSP), cpu.sizeOfWord);
		cg.GenerateMove(makereg(regFP), makereg(regSP));
		GenerateTriadic(op_sub, 0, makereg(regSP), makereg(regSP), MakeImmediate(fn->stkspace));
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
		GenerateLoadAddress(ap, cg.MakeStringAsNameConst(buf, codeseg));
		cg.GenerateStore(ap, cg.MakeIndexed((int64_t)32, regFP), cpu.sizeOfWord);
		ReleaseTempRegister(ap);
		//		GenerateDiadic(cpu.mov_op, 0, makereg(regAFP), makereg(regFP));
		GenerateMonadic(op_bex, 0, cg.MakeCodeLabel(currentFn->defCatchLabel));
	}
	fn->tryCount = 0;
	return (ip);
}

Operand* BigfootCodeGenerator::GenerateLand(ENODE* node, int flags, int op, bool safe)
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

/* Generate code for an immediate 'add' operation. Usually one of the immediate
* mode instructions will be used, unless the value is too large (>30 bits). In
* which case the value is loaded into a temp register then used.
*
* Parameters:
*		dst (input) pointer to the destination operand
*		src1 (input) pointer to the first source operand
*		src2 (input) pointer to the immediate operand
* Return:
*		a pointer to the destination operand.
*/
Operand* BigfootCodeGenerator::GenerateAddImmediate(Operand* dst, Operand* src1, Operand* src2)
{
	Operand* ap5;

	// ToDo: Should spit out a compiler warning here.
	if (src2->offset == nullptr)
		return (dst);

	// Will it fit into one of the immediate mode instructions?
	if (src2->offset->i128.IsNBit(5) && dst->preg == src1->preg) {
		GenerateDiadic(op_addq, 0, dst, MakeImmediate(src2->offset->i128.low));
		return (dst);
	}
	if (src2->offset->i128.IsNBit(30)) {
		GenerateTriadic(op_add, 0, dst, src1, MakeImmediate(src2->offset->i128.low));
		return (dst);
	}
	// Use a register.
	ap5 = GetTempRegister();
	GenerateLoadConst(src2, ap5);
	GenerateTriadic(op_add, 0, dst, src1, ap5);
	ReleaseTempRegister(ap5);
	return (dst);
}

/* Generate code for an immediate 'and' operation. 
* 
* Parameters:
*		dst (input) pointer to the destination operand
*		src1 (input) pointer to the first source operand
*		src2 (input) pointer to the immediate operand
* Return:
*		a pointer to the destination operand.
*/
Operand* BigfootCodeGenerator::GenerateAndImmediate(Operand* dst, Operand* src1, Operand* src2)
{
	Operand* ap5;

	// ToDo: Should spit out a compiler warning here.
	if (src2->offset == nullptr)
		return (dst);

	// Will it fit into one of the immediate mode instructions?
	if (src2->offset->i128.IsNBit(30)) {
		GenerateTriadic(op_and, 0, dst, src1, MakeImmediate(src2->offset->i128.low));
		return (dst);
	}
	// Use a register.
	ap5 = GetTempRegister();
	GenerateLoadConst(src2, ap5);
	GenerateTriadic(op_and, 0, dst, src1, ap5);
	ReleaseTempRegister(ap5);
	return (dst);
}

/* Generate code for an immediate 'or' operation.
*
* Parameters:
*		dst (input) pointer to the destination operand
*		src1 (input) pointer to the first source operand
*		src2 (input) pointer to the immediate operand
* Return:
*		a pointer to the destination operand.
*/
Operand* BigfootCodeGenerator::GenerateOrImmediate(Operand* dst, Operand* src1, Operand* src2)
{
	Operand* ap5;

	// ToDo: Should spit out a compiler warning here.
	if (src2->offset == nullptr)
		return (dst);

	// Will it fit into one of the immediate mode instructions?
	if (src2->offset->i128.IsNBit(30)) {
		GenerateTriadic(op_or, 0, dst, src1, MakeImmediate(src2->offset->i128.low));
		return (dst);
	}
	// Use a register.
	ap5 = GetTempRegister();
	GenerateLoadConst(src2, ap5);
	GenerateTriadic(op_or, 0, dst, src1, ap5);
	ReleaseTempRegister(ap5);
	return (dst);
}

/* Generate code for an immediate 'or' operation. Uses the CPU's shifted and
* operations if the immediate constant is large. Note the CPU does not require
* the use of temporary registers to extend the operation.
*
* Parameters:
*		dst (input) pointer to the destination operand
*		src1 (input) pointer to the first source operand
*		src2 (input) pointer to the immediate operand
* Return:
*		a pointer to the destination operand.
*/
Operand* BigfootCodeGenerator::GenerateEorImmediate(Operand* dst, Operand* src1, Operand* src2)
{
	Operand* ap5, * ap4;

	// ToDo: Should spit out a compiler warning here.
	if (src2->offset == nullptr)
		return (dst);

	// Will it fit into one of the immediate mode instructions?
	if (src2->offset->i128.IsNBit(30)) {
		GenerateTriadic(op_eor, 0, dst, src1, MakeImmediate(src2->offset->i128.low));
		return (dst);
	}
	// Use a register.
	ap5 = GetTempRegister();
	GenerateLoadConst(src2, ap5);
	GenerateTriadic(op_eor, 0, dst, src1, ap5);
	ReleaseTempRegister(ap5);
	return (dst);
}

void BigfootOCODE::OptAdd()
{
	OCODE::OptAdd();
	if (oper1->preg == oper2->preg && oper3->offset->i128.IsNBit(5)) {
		opcode = op_addq;
		insn = GetInsn(op_addq);
		optimized++;
	}
}
