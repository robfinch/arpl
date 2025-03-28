// ============================================================================
//        __
//   \\__/ o\    (C) 2017-2024  Robert Finch, Waterloo
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

Operand *Operand::Clone()
{
	Operand *newap;

	if (this == NULL)
		return NULL;
	newap = allocOperand();
	memcpy(newap, this, sizeof(Operand));
	offset = offset->Clone();
	return (newap);
}


//      compare two address nodes and return true if they are
//      equivalent.

bool Operand::IsEqual(Operand *ap1, Operand *ap2)
{
	if (ap1 == nullptr || ap2 == nullptr)
		return (false);
	if (ap1->mode != ap2->mode && !((ap1->mode == am_ind && ap2->mode == am_indx) || (ap1->mode == am_indx && ap2->mode == am_ind)))
		return (false);
	switch (ap1->mode)
	{
	case am_imm:
		return (ENODE::IsEqual(ap1->offset, ap2->offset));
	case am_fpreg:
	case am_reg:
	case am_creg:
	case am_preg:
		return (ap1->preg == ap2->preg);
	case am_ind:
	case am_indx:
		if (ap1->preg != ap2->preg)
			return (false);
		if (!ENODE::IsEqual(ap1->offset, ap2->offset) || !ENODE::IsEqual(ap1->offset2, ap2->offset2))
			return (false);
		return (true);
	}
	return (false);
}

bool Operand::IsSameType(Operand *ap1, Operand *ap2)
{
	if (ap1 == nullptr || ap2 == nullptr)
		return (false);
	return (ENODE::IsSameType(ap1->offset, ap2->offset));
}

char Operand::fpsize()
{
	if (typep == &stddouble)
		return ' ';
	if (typep == &stdquad)
		return 'q';
	if (typep == &stdflt)
		return 's';
	if (typep == &stdtriple)
		return 't';

	if (FloatSize)
		return (FloatSize);
	if (offset == nullptr)
		return (' ');
	if (offset->tp == nullptr)
		return (' ');
	switch (offset->tp->precision) {
	case 32:	return ('s');
	case 64:	return (' ');
	case 96:	return ('t');
	case 128:	return ('q');
	default:	return (' ');
	}
}

Operand* Operand::GenerateBitfieldClear(int startpos, int width)
{
	if (startpos + width < cpu.sizeOfWord * 8) {
		Operand* ap = GetTempRegister();
		Generate4adic(op_clr, 0, ap, this, cg.MakeImmediate((int64_t)startpos), cg.MakeImmediate((int64_t)startpos + width - 1));
		return (ap);
	}
	return (this);
}

Operand* Operand::GenerateZeroExtend(int startpos, int totwidth)
{
	Operand* ap;

	if (startpos >= totwidth)
		return (this);
	ap = GetTempRegister();
	switch (startpos)
	{
	case 8:	GenerateDiadic(op_zxb, 0, ap, this); break;
	case 16:	GenerateDiadic(op_zxw, 0, ap, this); break;
	case 32:	GenerateDiadic(op_zxt, 0, ap, this); break;
	case 64:	GenerateDiadic(op_zxo, 0, ap, this); break;
	default:
		ap = GenerateBitfieldClear(startpos, totwidth-startpos);
	}
	return (ap);
}

Operand *Operand::GenerateSignExtend(int64_t isize, int64_t osize, int flags)
{
	Operand *ap1;
	Operand *ap = this;

	if (isize >= osize)
		return (ap);
	if (ap->isPtr)
		return (ap);
	if (ap->isUnsigned)
		return (ap);
	if (ap->mode != am_reg) {
		ap1 = GetTempRegister();
		ap1->tp = ap->tp;
		cg.GenerateLoad(ap1, ap, isize, isize);
		ReleaseTempRegister(ap);
		switch (isize)
		{
		case 1:	GenerateDiadic(op_sxb, 0, ap1, ap1); break;
		case 2:	GenerateDiadic(op_sxw, 0, ap1, ap1); break;
		case 4:	GenerateDiadic(op_sxt, 0, ap1, ap1); break;
		case 8:	GenerateDiadic(op_sxo, 0, ap1, ap1); break;
		}
		//GenStore(ap1, ap, osize);
		//ReleaseTempRegister(ap1);
		return (ap1);
		//MakeLegalOperand(ap,flags & (am_reg|am_fpreg),isize);
	}
	if (ap->typep == &stddouble) {
		switch (isize) {
		case 4:	GenerateDiadic(op_fs2d, 0, ap, ap); break;
		}
	}
	else {
		switch (isize)
		{
		case 1:	GenerateDiadic(op_sxb, 0, ap, ap); break;
		case 2:	GenerateDiadic(op_sxw, 0, ap, ap); break;
		case 4:	GenerateDiadic(op_sxt, 0, ap, ap); break;
		case 8:	GenerateDiadic(op_sxo, 0, ap, ap); break;
		}
	}
	return (ap);
}

void Operand::MakeLegalReg(int flags, int64_t size)
{
	Operand* ap2;

	if (mode == am_reg)	// Might get this if am_volatile specified
		return;
	if (mode == am_vreg && size == 64)
		return;
	ReleaseTempRegister(this);      // maybe we can use it...
	if (size == 64) {
		ap2 = GetTempVectorRegister();
	}
	else if (this)
		ap2 = GetTempRegister();// GetTempReg(ap->type);
	else
		ap2 = GetTempRegister();// (stdint.GetIndex());
	if (this->tp)
		ap2->tp = this->tp;
	else
		ap2->tp = &stdint;
	switch (mode) {
	case am_ind:
	case am_indx:
		ap2->isUnsigned = this->isUnsigned;
		if (this->tp) {
			if (this->tp->btpp)
				ap2->isUnsigned = this->tp->btpp->isUnsigned;
		}
		cg.GenerateLoad(ap2, this, size, size);
		break;
	case am_indx2:
		cg.GenerateLoad(ap2, this, size, size);
		break;
	case am_imm:
		cg.GenerateLoadConst(this, ap2);
		break;
	case am_reg:
		cg.GenerateMove(ap2, this);
		break;
	case am_vreg:
		cg.GenerateMove(ap2, this);
		break;
	case am_preg:
		GenerateDiadic(op_ptoi, 0, ap2, this);
		break;
	case am_fpreg:
		GenerateDiadic(op_ftoi, fpsize(), ap2, this);
		break;
	case am_creg:
		GenerateTriadic(op_aslx, 0, ap2, makereg(regZero), cg.MakeImmediate((int64_t)1));
		break;
	case amCrReg:
		GenerateTriadic(op_crext, 0, ap2, this, cg.MakeImmediate(2));
		break;
	default:
		cg.GenerateLoad(ap2, this, size, size);
		break;
	}
	if (ap2->preg & rt_vector)
		mode = am_vreg;
	else if (IsTempCrReg(ap2->preg))
		mode = amCrReg;
	else
		mode = am_reg;
	switch (size) {
	case 0: typep = &stdvoid; break;
	case 1: typep = &stdbyte; break;
	case 2: typep = &stdchar; break;
	case 4:	typep = &stdshort; break;
	case 8: typep = &stdint; break;
	case 16: typep = &stdlong; break;
	case 64: typep = &stdvector; break;
	default:
		typep = &stdint;
	}
	preg = ap2->preg;
	deep = ap2->deep;
	pdeep = ap2->pdeep;
	tempflag = 1;
	memref = ap2->memref;
	memop = ap2->memop;
}

void Operand::MakeLegalCrReg(int flags, int64_t size)
{
	Operand* ap2, * ap3;

	if (mode == amCrReg)	// Might get this if am_volatile specified
		return;
	if (mode == am_vreg && size == 64)
		return;
	ReleaseTempRegister(this);      // maybe we can use it...
	if (size == 64) {
		ap2 = GetTempVectorRegister();
	}
	else if (this)
		ap2 = GetTempCrRegister();// GetTempReg(ap->type);
	else
		ap2 = GetTempCrRegister();// (stdint.GetIndex());
	if (this->tp)
		ap2->tp = this->tp;
	else
		ap2->tp = &stdint;
	switch (mode) {
	case am_direct:
	case am_ind:
	case am_indx:
		ap2->isUnsigned = this->isUnsigned;
		if (this->tp) {
			if (this->tp->btpp)
				ap2->isUnsigned = this->tp->btpp->isUnsigned;
		}
		ap3 = GetTempRegister();
		cg.GenerateLoad(ap3, this, size, size);
		cg.GenerateCrMove(ap2, ap3);
		ReleaseTempRegister(ap3);
		break;
	case am_indx2:
		ap3 = GetTempRegister();
		cg.GenerateLoad(ap3, this, size, size);
		cg.GenerateCrMove(ap2, ap3);
		ReleaseTempRegister(ap3);
		break;
	case am_imm:
		ap3 = GetTempRegister();
		cg.GenerateLoadConst(this, ap3);
		cg.GenerateCrMove(ap2, ap3);
		ReleaseTempRegister(ap3);
		break;
	case am_reg:
		GenerateTriadic(op_zsne, 0, ap2, this, makereg(regZero));
		break;
	case am_vreg:
		cg.GenerateMove(ap2, this);
		break;
	case am_preg:
		GenerateDiadic(op_ptoi, 0, ap2, this);
		break;
	case am_fpreg:
		GenerateTriadic(op_fsne, 0, ap2, this, makereg(regZero));
		break;
	case amCrReg:
		cg.GenerateCrMove(ap2, this);
		break;
	case am_creg:
		GenerateTriadic(op_aslx, 0, ap2, makereg(regZero), cg.MakeImmediate((int64_t)1));
		break;
	default:
		cg.GenerateLoad(ap2, this, size, size);
		break;
	}
	if (ap2->preg & rt_vector)
		mode = am_vreg;
	else
		mode = amCrReg;
	typep = &stdbyte;
	/*
	switch (size) {
	case 0: typep = &stdvoid; break;
	case 1: typep = &stdbyte; break;
	case 2: typep = &stdchar; break;
	case 4:	typep = &stdshort; break;
	case 8: typep = &stdint; break;
	case 16: typep = &stdlong; break;
	case 64: typep = &stdvector; break;
	default:
		typep = &stdint;
	}
	*/
	preg = ap2->preg;
	deep = ap2->deep;
	pdeep = ap2->pdeep;
	tempflag = 1;
	memref = ap2->memref;
	memop = ap2->memop;
}

// ----------------------------------------------------------------------------
// MakeLegal will coerce the addressing mode in ap1 into a mode that is
// satisfactory for the flag word.
// ----------------------------------------------------------------------------
void Operand::MakeLegal(int flags, int64_t size)
{
	Operand *ap2, *ap1;
	int64_t i;

	if (this == nullptr)
		return;

	//	if (flags & am_novalue) return;
	if (((flags & am_volatile) == 0) || tempflag)
	{
		switch (mode) {
		case am_imm:
			i = ((ENODE *)(offset))->i;
			if (flags & am_i8) {
				if (i < 256 && i >= 0)
					return;
			}
			else if (flags & am_ui6) {
				if (i < 64 && i >= 0)
					return;
			}
			else if (flags & am_i16) {
				if (i < 32767 && i >= -32768)
					return;
			}
			else if (flags & am_imm0) {
				if (i == 0)
					return;
				if (flags & am_imm) {
					if (flags & am_reg) {
						if (offset->i == 0) {
							mode = am_reg;
							preg = 0;
						}
					}
					return;
				}
			}
			// If there is a choice between r0 and #0 choose r0.
			else if (flags & am_imm) {
				
				if (tp && tp->IsFloatType()) {
					return;
					if (flags & am_reg) {
						if (offset->f128.IsZero()) {
							mode = am_reg;
							preg = 0;
						}
					}
				}
				else if (tp && tp->IsPositType()) {
					if (flags & am_reg) {
						if (offset->posit.val == 0) {
							mode = am_reg;
							preg = 0;
						}
					}
				}
				else {
					if (flags & am_reg) {
						if (offset->i == 0) {
							mode = am_reg;
							preg = 0;
						}
					}
				}
				return;
			}
			break;
		case am_reg:
			if (flags & am_reg)
				return;
			// Allow r0 to substitute for #0
			if (flags & am_imm) {
				if (preg == 0) {
					offset = allocEnode();
					offset->i = 0;
					offset->i128 = *Int128::Zero();
					isConst = true;
					return;
				}
			}
			break;
		case am_fpreg:
			if (flags & am_fpreg)
				return;
			break;
		case am_vreg:
			if (flags & am_vreg)
				return;
			break;
		case am_preg:
			if (flags & am_reg)
				return;
			break;
		case amCrReg:
			if (flags & amCrReg)
				return;
		case am_ind:
		case am_indx:
		case am_indx2:
		case am_direct:
			if (flags & am_mem)
				return;
			break;
		}
	}

	if (flags & am_reg)
	{
		MakeLegalReg(flags, size);
		return;
	}
	if (flags & amCrReg) {
		MakeLegalCrReg(flags, size);
		return;
	}
	if (flags & am_fpreg)
	{
		if (mode == am_fpreg)
			return;
		ReleaseTempReg(this);      /* maybe we can use it... */
		ap2 = GetTempFPRegister();
		ap2->tp = this->tp;	// Load needs this
		switch (mode) {
		case am_ind:
		case am_indx:
			cg.GenerateLoad(ap2, this, size, size);
			break;
		case am_imm:
			ap1 = GetTempRegister();
			GenerateDiadic(cpu.ldi_op, 0, ap1, this);
			GenerateDiadic(cpu.mov_op, 0, ap2, ap1);
			ReleaseTempReg(ap1);
			break;
		case am_reg:
			GenerateDiadic(op_itof, ap2->fpsize(), ap2, this);
			break;
		default:
			cg.GenerateLoad(ap2, this, size, size);
			break;
		}
		mode = am_reg;
		switch (ap2->fpsize()) {
		case 'd':	typep = &stddouble; break;
		case 's': typep = &stddouble; break;
		case 't': typep = &stdtriple; break;
		case 'q':	typep = &stdquad; break;
		default:	typep = &stddouble; break;
		}
		preg = ap2->preg;
		deep = ap2->deep;
		pdeep = ap2->pdeep;
		tempflag = 1;
		return;
	}
	if (flags & am_preg)
	{
		if (mode == am_preg)
			return;
		ReleaseTempReg(this);      /* maybe we can use it... */
		ap2 = GetTempPositRegister();
		ap2->typep = &stdposit;
		ap2->tp = &stdposit;	// load needs this
		switch (mode) {
		case am_ind:
		case am_indx:
			cg.GenerateLoad(ap2, this, size, size);
			break;
		case am_imm:
			ap1 = GetTempRegister();
			GenerateDiadic(cpu.ldi_op, 0, ap1, this);
			GenerateDiadic(cpu.mov_op, 0, ap2, ap1);
			ReleaseTempReg(ap1);
			break;
		case am_reg:
			if (regs[preg].ContainsPositConst())
				GenerateDiadic(cpu.mov_op, 0, ap2, this);
			else
				GenerateDiadic(op_itop, 0, ap2, this);
			break;
		default:
			cg.GenerateLoad(ap2, this, size, size);
			break;
		}
		mode = am_preg;
		switch (ap2->fpsize()) {
		case 'd':	typep = &stdposit; break;
		case 's': typep = &stdposit32; break;
		case 'h': typep = &stdposit16; break;
		default:	typep = &stdposit; break;
		}
		preg = ap2->preg;
		deep = ap2->deep;
		pdeep = ap2->pdeep;
		tempflag = 1;
		return;
	}
	if (flags & am_creg) {
		switch (mode) {
		case am_ind:
		case am_indx:
		case am_indx2:
			ap2 = GetTempRegister();
			cg.GenerateLoad(ap2, this, size, size);
			ap1 = cg.MakeBoolean(ap2);
			GenerateDiadic(cpu.mov_op, 0, this, ap1); //ToDo: fix
			ReleaseTempReg(ap1);
			ReleaseTempReg(ap2);
			return;
		case am_imm:
			Generate4adic(op_sne, 0, this, makereg(regZero), this, cg.MakeImmediate(1));
			return;
		case am_reg:
			ap1 = cg.MakeBoolean(this);
			GenerateDiadic(cpu.mov_op, 0, this, ap1);
			ReleaseTempReg(ap1);
			return;
		}
	}

	// Here we wanted the mode to be non-register (memory/immed)
	// Should fix the following to place the result in memory and
	// not a register.
	if (size == 1)
	{
		ReleaseTempRegister(this);
		ap2 = GetTempRegister();
		if (this->mode == am_reg)
			GenerateDiadic(cpu.mov_op, 0, ap2, this);
		else if (this->mode == amCrReg)
			cg.GenerateCrMove(ap2, this);
		else if (this->mode == am_imm)
			cg.GenerateLoadConst(this, ap2);
		else
			cg.GenerateLoad(ap2, this, size, size);
		if (this->mode != amCrReg) {
			if (isUnsigned)
				cg.GenerateAndImmediate(ap2, ap2, cg.MakeImmediate(255));
			else {
				cg.GenerateSignExtendByte(ap2, ap2);
			}
		}
		mode = ap2->mode;
		preg = ap2->preg;
		deep = ap2->deep;
		pdeep = ap2->pdeep;
		size = 2;
	}
	if (tp && tp->type == bt_vector) {
		ap2 = GetTempVectorRegister();
		ap2->typep = &stdvector;
	}
	else {
		ap2 = GetTempRegister();
		ap2->typep = &stdlong;
	}
	switch (mode) {
	case am_ind:
	case am_indx:
		cg.GenerateLoad(ap2, this, size, size);
		break;
	case am_imm:
		cg.GenerateLoadConst(this, ap2);
		//GenerateDiadic(op_loadi, 0, ap2, this);
		break;
	case am_reg:
		GenerateDiadic(cpu.mov_op, 0, ap2, this);
		break;
	default:
		cg.GenerateLoad(ap2, this, size, size);
	}
	if (tp && tp->type == bt_vector)
		mode = am_vreg;
	else
		mode = am_reg;
	preg = ap2->preg;
	deep = ap2->deep;
	pdeep = ap2->pdeep;
	tempflag = 1;
	//     Leave("MkLegalOperand",0);
}

int Operand::OptRegConst(int regclass, bool tally)
{
	MachineReg *mr;
	int count = 0;

	if (this == nullptr)
		return (0);

	//if (regs[sreg].number != regZero)
	//	return(0);
	mr = &regs[preg];
	if (regclass & am_imm) {
		if (mode == am_reg) {
			if (mr->sub) {
				if (tally)
					count++;
				else {
					mode = am_imm;
					offset = mr->offset;
				}
			}
		}
	}
	else {
		if (mode == am_reg)
			mr->sub = false;
		else if (mode == am_ind) {
			if (mr->sub) {
				if (tally)
					count++;
				else {
					mode = am_direct;
					offset = mr->offset;
				}
			}
		}
		else if (mode == am_indx) {
			if (mr->sub) {
				if (tally)
					count++;
				else {
					mode = am_direct;
					offset2 = mr->offset;
				}
			}
		}
		/*
		else if (mode == am_indx2) {
			if (mr->sub && scale == 1) {
				mode = am_indx;
				preg = sreg;
				offset = mr->offset;
			}
			mr2 = &regs[sreg];
			if (mr2->sub) {
				if (mode == am_indx) {
					mode = am_direct;
					offset2 = mr2->offset;
				}
				else {	// indx
					mode = am_indx;
					offset = mr2->offset;
				}
			}
		}
		*/
	}
	return (count);
}

void Operand::storeHex(txtoStream& ofs)
{
	ofs.printf("O");
	switch (mode) {
	case am_reg:
		ofs.printf("R%02X", (int)preg);
		break;
	//case am_fpreg:
	//	ofs.printf("FP%02X", (int)preg);
	//	break;
	case am_imm:
		ofs.printf("#");
		offset->storeHex(ofs);
		offset2->storeHex(ofs);
		break;
	}
}

Operand *Operand::loadHex(txtiStream& ifs)
{
	Operand *oper;
	char ch;
	char buf[100];

	oper = allocOperand();
	ifs.read(&ch,1);
	switch (ch) {
	case '#':
		oper->mode = am_imm;
		oper->offset->loadHex(ifs);
		oper->offset2->loadHex(ifs);
		break;
	case 'R':
		oper->mode = am_reg;
		ifs.read(buf, 2);
		buf[2] = '\0';
		oper->preg = strtoul(buf, nullptr, 16);
		break;
	case 'F':
		ifs.read(buf, 1);
		switch (buf[0]) {
		case 'P':
			oper->mode = am_reg;
			ifs.read(buf, 2);
			buf[2] = '\0';
			oper->preg = strtoul(buf, nullptr, 16);
			break;
		}
	}
	return (oper);
}


void Operand::store(txtoStream& ofs)
{
	ENODE* tnode;

	if (mode == 0)
		mode = am_reg;
	switch (mode)
	{
	case am_imm:
		ofs.puts(imm_char);
		if (!cpu.Addsi)
			ofs.write("");	// was "#"
		// Fall through
	case am_direct:
		offset->PutConstant(ofs, lowhigh, rshift, false, display_opt);
		if (offset2) {
			ofs.printf("+");
			offset2->PutConstant(ofs, lowhigh, rshift);
		}
		break;
	case am_direct2:
		offset->PutConstant(ofs, lowhigh, rshift);
		ofs.printf("+%d", (int)preg);
		break;
	case am_reg:
		if (typep == &stdvector)
			ofs.write(cpu.RegMoniker(preg | rt_vector));
		else if (typep == &stdvectormask)
			ofs.printf("vm%d", (int)preg);
		else {
			ofs.write(cpu.RegMoniker(preg));
			//if (renamed)
			//	ofs.printf(".%d", (int)pregs);
		}
		break;
	case am_vreg:
		ofs.write(cpu.RegMoniker(preg|rt_vector));
		break;
	case am_vmreg:
		ofs.printf("vm%d", (int)preg);
		break;
	case am_creg:
	case am_preg:
	case am_fpreg:
	case amCrReg:
		ofs.printf("%s", cpu.RegMoniker(preg));
		break;
	case amCrgReg:
		ofs.printf("%s", cpu.RegMoniker(preg));
		break;
	case am_ind:
		//if (preg == 0)
		//	printf("hello");
		ofs.printf("[%s]", cpu.RegMoniker(preg));
		break;
	case am_indx:
		// It's not known the function is a leaf routine until code
		// generation time. So the parameter offsets can't be determined
		// until code is being output. This bit of code first adds onto
		// parameter offset the size of the return block, then later
		// subtracts it off again.
		if (offset) {
			tnode = offset->Clone();
			if (preg == regFP) {
				if (tnode->sym) {
					if (tnode->sym->IsParameter) {	// must be an parameter
						tnode->i += compiler.GetReturnBlockSize();	// The frame pointer is the second word of the return block.
						tnode->i128.Add(&offset->i128, &offset->i128, Int128::MakeInt128(compiler.GetReturnBlockSize()));
					}
				}
			}
			if (tnode->scale) {
				Int128 s;

				tnode->i *= tnode->scale;
				s = Int128::Convert((int64_t)tnode->scale);
				Int128::Mul(&tnode->i128, &tnode->i128, &s);
			}
			tnode->PutConstant(ofs, 0, 0);
			/*
			if (preg == regFP) {
				if (offset->sym) {
					if (offset->sym->IsParameter) {
						offset->i -= compiler.GetReturnBlockSize();
						offset->i128.Sub(&offset->i128, &offset->i128, Int128::MakeInt128(compiler.GetReturnBlockSize()));
					}
				}
			}
			*/
		}
		if (offset2) {
			if (offset2->i < 0)
				ofs.printf("%d", (int)offset2->i);
			else
				ofs.printf("+%d", (int)offset2->i);
		}
		else
			ofs.printf("[%s]", cpu.RegMoniker(preg));
		break;

	case am_indx2:
		if (offset)
			offset->PutConstant(ofs,0,0);
		if (scale == 1) {
			if (sreg==regZero)
				ofs.printf("[%s]", cpu.RegMoniker(preg));
			else if (preg==regZero)
				ofs.printf("[%s]", cpu.RegMoniker(sreg));
			else
				ofs.printf((char *)"[%s+%s]", cpu.RegMoniker(preg), cpu.RegMoniker(sreg));
		}
		else if (scale == 0) {
			if (sreg == regZero)
				ofs.printf("[%s]", cpu.RegMoniker(preg));
			else if (preg == regZero)
				ofs.printf("[%s]", cpu.RegMoniker(sreg));
			else
				ofs.printf((char*)"[%s+%s*]", cpu.RegMoniker(preg), cpu.RegMoniker(sreg));
		}
		else {
			printf("DIAG - illegal address mode.\n");
			ofs.printf((char*)"[%s+%s*%d]", cpu.RegMoniker(preg), cpu.RegMoniker(sreg), scale);
		}
		break;
		
	case am_iindx2:
		ofs.puts("[");
		if (offset) {
			offset->PutConstant(ofs, 0, 0);
			ofs.puts("@got");
		}
		if (scale == 1) {
			if (sreg == regZero)
				ofs.printf("[%s]", cpu.RegMoniker(preg));
			else if (preg == regZero)
				ofs.printf("[%s]", cpu.RegMoniker(sreg));
			else
				ofs.printf((char*)"[%s+%s]", cpu.RegMoniker(preg), cpu.RegMoniker(sreg));
		}
		else if (scale == 0) {
			if (sreg == regZero)
				ofs.printf("[%s]", cpu.RegMoniker(preg));
			else if (preg == regZero)
				ofs.printf("[%s]", cpu.RegMoniker(sreg));
			else
				ofs.printf((char*)"[%s+%s*]", cpu.RegMoniker(preg), cpu.RegMoniker(sreg));
		}
		else {
			printf("DIAG - illegal address mode.\n");
			ofs.printf((char*)"[%s+%s*%d]", cpu.RegMoniker(preg), cpu.RegMoniker(sreg), scale);
		}
		ofs.puts("]");
		break;
		//	case am_mask:
//		put_mask((int)offset);
//		break;
	default:
		printf("DIAG - illegal address mode.\n");
		break;
	}
}

void Operand::load(txtiStream& ifp)
{
//	char ch;

	//ifp.get(&ch);
	/*
	switch (ch)
	{
	case '$':
		break;
	case '#':
		break;
	}
	*/
}

/*
* Gets the constant value associated with an operand, if the operand is known
* to be a constant. The value could be either an immediate constant or a value
* in a register.
* 
* Parameter:
* val: (output) pointer to place to store value.
* 
* Returns:
*		a Boolean indicating if the constant value could be returned.
* 
*/
bool Operand::GetConstValue(Int128* val)
{
	if (isConst) {
		if (mode == am_imm) {
			val = &offset->i128;
			return (true);
		}
		else if (mode == am_reg) {
			val = &regs[preg].val128;
			return (true);
		}
	}
	return (false);
}
