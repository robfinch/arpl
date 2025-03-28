// ============================================================================
//        __
//   \\__/ o\    (C) 2018-2024  Robert Finch, Waterloo
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

void OCODE::Remove()
{
	currentFn->pl.Remove(this);
};

bool OCODE::IsLoad() const
{
	if (insn)
		return (insn->IsLoad());
	return (false);
}

bool OCODE::IsStore() const
{
	if (insn)
		return (insn->IsStore());
	return (false);
}

// Return true if the instruction has a target register.

bool OCODE::HasTargetReg() const
{
	if (insn) {
		if (insn->opcode == op_call || insn->opcode == op_jal || insn->opcode==op_jsr)
			return (oper1->type != bt_void);
		return (insn->HasTarget());
	}
	else
		return (false);
}

bool OCODE::HasTargetReg(int regno) const
{
	int rg1, rg2;
	if (HasTargetReg()) {
		if (insn->opcode == op_loadm)
			return ((oper1->offset->i >> regno) & 1LL);
		GetTargetReg(&rg1, &rg2);
		if (rg1 == regno || rg2 == regno)
			return (true);
	}
	return (false);
}

bool OCODE::HasSourceReg(int regno) const
{
	if (insn == nullptr)
		return (false);
	if (insn->opcode==op_storem)
		return ((oper1->offset->i >> regno) & 1LL);
	if ((IsStore() || IsLoad()) && oper1->preg == regno)
		return (true);
	// Push has an implied target, so oper1 is actually a source.
	// For deposit, the target is also a source
	// For orm, orh, the target is also a source
	if ((oper1 && !insn->HasTarget()) || opcode==op_push || opcode==op_dep ||
		opcode==op_ors || opcode==op_orm || opcode==op_orh ||
		opcode == op_eors || opcode == op_eorm || opcode == op_eorh ||
		opcode == op_ands || opcode == op_andm || opcode == op_andh ||
		opcode==op_adds || opcode==op_addm || opcode==op_addh) {
		if (oper1) {
			if (oper1->preg == regno)
				return (true);
			if (oper1->sreg == regno)
				return (true);
		}
		else
			return (false);
	}
	if (oper2 && oper2->preg==regno)
		return (true);
	if (oper2 && oper2->sreg==regno)
		return (true);
	if (oper3 && oper3->preg==regno)
		return (true);
	if (oper3 && oper3->sreg==regno)
		return (true);
	if (oper4 && oper4->preg==regno)
		return (true);
	if (oper4 && oper4->sreg==regno)
		return (true);
	// For pop the source operand is implied.
	if (opcode == op_pop)
		return (regno == regSP);
	if (opcode == op_unlk)
		return (regno == regFP);
	// The call instruction implicitly has register arguments as source registers.
	if (opcode==op_call || opcode==op_jsr) {
		if (IsArgumentReg(regno))
			return(true);
	}
	return (false);
}

// Get target reg needs to distinguish floating-point registers from regular
// general purpose registers. It returns a one for floating-point registers or
// a zero for general-purpose registers.

int OCODE::GetTargetReg(int *rg1, int *rg2) const
{
	if (insn==nullptr)
		return(0);
	if (insn->HasTarget()) {
		// Handle implicit targets
		switch(insn->opcode) {
		case op_link:
		case op_unlk:
			*rg1 = regSP;
			*rg2 = regFP;
			return(0);
		case op_pop:
			*rg1 = regSP;
			*rg2 = oper1->preg;
			return(0);
		case op_divmod:
		case op_mul:
		case op_mulu:
		case op_sort:
		case op_demux:
		case op_mov2:
			*rg1 = oper1->preg;
			*rg2 = oper1->sreg;
			return (0);
		case op_pea:
		case op_push:
		case op_ret:
		case op_rts:
		case op_jsr:
		case op_call:
			*rg1 = regSP;
			*rg2 = 0;
			return (0);
		default:
			if (oper1) {
				if (oper1->mode == am_reg) {
					*rg1 = oper1->preg;
					*rg2 = 0;
					return (1);
				}
				else {
					*rg1 = oper1->preg;
					*rg2 = 0;
					return (0);
				}
			}
		}
	}
	else {
		*rg1 = 0;
		*rg2 = 0;
		return (0);
	}
	return (0);
}

OCODE *OCODE::Clone(OCODE *c)
{
	OCODE *cd;
	cd = (OCODE *)xalloc(sizeof(OCODE));
	memcpy(cd, c, sizeof(OCODE));
	if (cd->opcode != op_label) {
		cd->oper1 = cd->oper1->Clone();
		cd->oper2 = cd->oper2->Clone();
		cd->oper3 = cd->oper3->Clone();
		cd->oper4 = cd->oper4->Clone();
	}
	return (cd);
}

void OCODE::Swap(OCODE *ip1, OCODE *ip2)
{
	OCODE *ip1b = ip1->back, *ip1f = ip1->fwd;
	OCODE *ip2b = ip2->back, *ip2f = ip2->fwd;
	ip1b->fwd = ip2;
	ip2f->back = ip1;
	ip1->fwd = ip2f;
	ip1->back = ip2;
	ip2->fwd = ip1;
	ip2->back = ip1b;
}

void OCODE::OptVmask()
{
	int count;
	OCODE* ip;

	ip = this->fwd;
	for (count = 1; count < 5; count++, ip = ip->fwd) {

		// If there is no room in the vmask modifier, exit.
		if (oper2 != nullptr && oper3 != nullptr && oper4 != nullptr)
			return;

		// Abort if a label encountered.
		if (ip->opcode == op_label)
			return;
		// If pc changing instruction encountered, exit.
		if (ip->insn->IsFlowControl())
			return;
		if (ip->opcode == op_vmask) {
			if (count==2)
				if (this->oper2 == nullptr) {
					this->oper2 = ip->oper1;
					ip->oper1 = nullptr;
				}
			if (count==3)
				if (this->oper3 == nullptr) {
					if (ip->oper1 != nullptr) {
						this->oper3 = ip->oper1;
						ip->oper1 = nullptr;
					}
					else {
						this->oper3 = ip->oper2;
						ip->oper2 = nullptr;
					}
				}
			if (count==4)
				if (this->oper4 == nullptr) {
					if (ip->oper1 != nullptr) {
						this->oper4 = ip->oper1;
						ip->oper1 = nullptr;
					}
					else if (ip->oper2 != nullptr) {
						this->oper4 = ip->oper2;
						ip->oper2 = nullptr;
					}
					else {
						this->oper4 = ip->oper3;
						ip->oper3 = nullptr;
					}
				}
				if (ip->oper1 == nullptr && ip->oper2 == nullptr && ip->oper3 == nullptr && ip->oper4 == nullptr) {
					ip->MarkRemove();
					optimized++;
				}
			if (ip->remove)
				count--;
			continue;
		}
		if (count == 2)
			this->oper2 = makereg(regZero);
		if (count == 3)
			this->oper3 = makereg(regZero);
		if (count == 4)
			this->oper4 = makereg(regZero);
	}
}


//
//      mov r3,r3 removed
//
// Code Like:
//		add		r3,r2,#10
//		mov		r3,r5
// Changed to:
//		mov		r3,r5

void OCODE::OptMove()
{
	OCODE* pbk;

	if (OCODE::IsEqualOperand(oper1, oper2)) {
		MarkRemove();
		optimized++;
		return;
	}
	
	/* under construction */
	// look for
	//		add Rt,Ra,Rb
	//		mov Rn,Rt
	// substitute
	//		add Rt,Ra,Rb
	//		add Rn,Ra,Rb
	// should allow the compiler to remove the first add
	// But, do not do for volatile operations, a I/O load should not be replicated.
	return;
	for (pbk = back; pbk && (pbk->opcode == op_hint || pbk->opcode == op_remark); pbk = pbk->back)
		;
	if (pbk && pbk->oper1 && pbk->HasTargetReg() && !pbk->isVolatile) {
		if (pbk->oper1->mode == am_reg && oper2->mode == am_reg) {
			if (pbk->oper1->preg == oper2->preg) {
				OCODE* p;
				p = pbk->Clone(pbk);
				p->oper1 = oper1;
				p->fwd = fwd;
				p->back = pbk;
				if (fwd)
					fwd->back = p;
				pbk->fwd = p;
				optimized++;
				return;
			}
		}
	}
}

//	   sge		$v1,$r12,$v2
//     redor	$v2,$v1
// Translates to:
//	   sge		$v1,$r12,$v2
//     mov		$v2,$v1
// Because redundant moves will be eliminated by further compiler
// optimizations.

void OCODE::OptRedor()
{
	if (back == nullptr)
		return;
	if (back->insn->IsSetInsn()) {
		if (back->oper1->preg == oper2->preg) {
			opcode = cpu.mov_op;
			insn = GetInsn(cpu.mov_op);
			optimized++;
		}
	}
}


bool OCODE::IsSubiSP()
{
	if (opcode == op_sub || opcode == op_gcsub) {
		if (oper3->mode == am_imm) {
			if (oper1->preg == regSP && oper2->preg == regSP) {
				return (true);
			}
		}
	}
	return (false);
}


void OCODE::OptAdd()
{
	// Add zero to self.
	if (IsEqualOperand(oper1, oper2)) {
		if (oper3) {
			if (oper3->mode == am_imm) {
				if (oper3->offset->nodetype == en_icon) {
					if (oper3->offset->i == 0) {
						MarkRemove();
						optimized++;
					}
				}
			}
			if (oper3->mode == am_reg && oper3->preg == regZero) {
				MarkRemove();
				optimized++;
			}
		}
	}
#ifdef BIGFOOT
	if (oper3->mode == am_imm && oper3->offset) {
		if (oper1->preg == oper2->preg && oper3->offset->i128.IsNBit(5)) {
			oper2 = oper3;
			oper3 = nullptr;
			opcode = op_addq;
			insn = GetInsn(op_addq);
			optimized++;
		}
	}
#endif
}
// 'subui' followed by a 'bne' gets turned into 'loop'
//
void OCODE::OptSubtract()
{
	OCODE *ip2;
	OCODE* fsbi;
	Int128 i128;

	ip2 = fwd;

	// Subtract zero from self.
	if (IsEqualOperand(oper1, oper2)) {
		if (oper3->mode == am_imm) {
			if (oper3->offset->nodetype == en_icon) {
				if (oper3->offset->i == 0) {
					MarkRemove();
					optimized++;
					return;
				}
			}
		}
		if (oper3->mode == am_reg && oper3->preg == regZero) {
			MarkRemove();
			optimized++;
		}
	}
#ifdef BIGFOOT
	if (oper3->mode == am_imm && oper3->offset) {
		i128 = oper3->offset->i128;
		// Ensure a negative value will fit in 5 bits when negated.
		Int128::Add(&i128, &i128, Int128::One());
		if (oper1->preg == oper2->preg && i128.IsNBit(5)) {
			// Make the offset negative.
			Int128::Sub(&oper3->offset->i128, Int128::Zero(), &oper3->offset->i128);
			opcode = op_addq;
			oper2 = oper3;
			oper3 = nullptr;
			insn = GetInsn(op_addq);
			optimized++;
		}
	}
#endif
	return;
	// The following seems not to work all the time.

	//if (oper2->isPtr && oper3->isPtr && oper2->mode == am_reg && oper3->mode == am_reg) {
	//	opcode = op_ptrdif;
	//	insn = GetInsn(op_ptrdif);
	//	oper4 = MakeImmediate(1);
	//}
	//if (ip2->opcode == op_asr || ip2->opcode == op_stpru) {
	//	if (Operand::IsEqual(ip2->oper2,oper1)) {
	//		if (oper2->isPtr && oper3->isPtr && oper2->mode == am_reg && oper3->mode == am_reg) {
	//			if (ip2->oper3->mode == am_imm) {
	//				if (ip2->oper3->offset->i < 8) {
	//					opcode = op_ptrdif;
	//					insn = GetInsn(op_ptrdif);
	//					oper4 = MakeImmediate(ip2->oper3->offset->i);
	//					ip2->MarkRemove();
	//					optimized++;
	//				}
	//			}
	//		}
	//	}
	//}
	// Find the first subtract
	fsbi = nullptr;
	for (ip2 = fwd; ip2; ip2 = ip2->fwd) {
		if (ip2->IsSubiSP()) {
			fsbi = ip2;
			break;
		}
	}
	if (fsbi == nullptr)
		return;
	ip2 = ip2->fwd;
	// Find subsequent subtracts
	while (ip2->opcode == op_hint)
		ip2 = ip2->fwd;
	for (; ip2; ip2 = ip2->fwd) {
		if (ip2->opcode == op_label)
			return;
		if (ip2->remove)
			continue;
		if (ip2->insn->IsFlowControl())
			return;
		if (!ip2->IsSubiSP()) {
			if (ip2->oper1 && ip2->oper1->preg == regSP)
				return;
			if (ip2->oper2 && ip2->oper2->preg == regSP)
				return;
			if (ip2->oper3 && ip2->oper3->preg == regSP)
				return;
			if (ip2->oper4 && ip2->oper4->preg == regSP)
				return;
		}
		else {
			// The following add does not update properly.
			fsbi->oper3->offset->i += ip2->oper3->offset->i;
			ip2->MarkRemove();
			optimized++;
		}
	}
	/*
	if (IsSubiSP() && ip2->fwd)
		if (ip2->IsSubiSP()) {
			oper3->offset->i += ip2->oper3->offset->i;
			ip2->MarkRemove();
		}
	*/
	for (ip2 = fwd; ip2; ip2 = ip2->fwd) {
		if (ip2->remove)
			continue;
		if (ip2->IsSubiSP() && ip2->oper3->offset->i == 0)
			ip2->MarkRemove();
	}
	return;
	if (opcode == op_subui) {
		if (oper3) {
			if (oper3->mode == am_imm) {
				if (oper3->offset->nodetype == en_icon && oper3->offset->i == 1) {
					if (fwd) {
						if (fwd->opcode == op_ne && fwd->oper2->mode == am_reg && fwd->oper2->preg == 0) {
							if (fwd->oper1->preg == oper1->preg) {
								opcode = op_loop;
								oper2 = fwd->oper3;
								oper3 = NULL;
								fwd->MarkRemove();
								optimized++;
								return;
							}
						}
					}
				}
			}
		}
	}
	return;
}


// This optimization eliminates an 'AND' instruction when the value
// in the register is a constant less than one of the two special
// constants 255 or 65535. This is typically a result of a zero
// extend operation.
//
// So code like:
//		ld		r3,#4
//		and		r3,r3,#255
// Eliminates the useless 'and' operation.

void OCODE::OptAnd()
{
	// This doesn't work properly yet in all cases.
	if (oper1 && oper2 && oper3) {
		if (oper1->mode == am_reg && oper2->mode == am_reg && oper3->mode == am_imm) {
			if (oper1->preg == oper2->preg && oper3->offset->i == -1) {
				MarkRemove();
				optimized++;
			}
		}
	}
	return;
	if (oper2 == nullptr || oper3 == nullptr)
		throw new ArplException(ERR_NULLPOINTER, 0x50);
	if (oper2->offset == nullptr || oper3->offset == nullptr)
		return;
	if (oper2->offset->constflag == false)
		return;
	if (oper3->offset->constflag == false)
		return;
	if (
		oper3->offset->i != 1 &&
		oper3->offset->i != 3 &&
		oper3->offset->i != 7 &&
		oper3->offset->i != 15 &&
		oper3->offset->i != 31 &&
		oper3->offset->i != 63 &&
		oper3->offset->i != 127 &&
		oper3->offset->i != 255 &&
		oper3->offset->i != 511 &&
		oper3->offset->i != 1023 &&
		oper3->offset->i != 2047 &&
		oper3->offset->i != 4095 &&
		oper3->offset->i != 8191 &&
		oper3->offset->i != 16383 &&
		oper3->offset->i != 32767 &&
		oper3->offset->i != 65535)
		// Could do this up to 32 bits
		return;
	if (oper2->offset->i < oper3->offset->i) {
		MarkRemove();
	}
}

// Strip out useless masking operations generated by type conversions.

void OCODE::OptLoad()
{
	if (oper2->mode != am_imm)
		return;
	if (insn->opcode == op_loadm)
		return;
	// This optimization is also caught by the code generator.
	if (oper2->tp && oper2->tp->IsPositType()) {
		if (oper2->offset->posit.val == 0) {
			opcode = cpu.mov_op;
			oper2->mode = am_preg;
			oper2->preg = 0;
			optimized++;
			return;
		}
	}
	else if (oper2->tp && oper2->tp->IsFloatType()) {
		if (oper2->offset->f128.IsZero()) {
			opcode = cpu.mov_op;
			oper2->mode = am_reg;
			oper2->preg = 0;
			optimized++;
			return;
		}
	}
	else if (oper2->offset->i == 0) {
		opcode = cpu.mov_op;
		oper2->mode = am_reg;
		oper2->preg = 0;
		optimized++;
		return;
	}
	if (!fwd)
		return;
	if (fwd->opcode != op_and)
		return;
	if (fwd->oper1->preg != oper1->preg)
		return;
	if (fwd->oper2->preg != oper1->preg)
		return;
	if (fwd->oper3->mode != am_imm)
		return;
	oper2->offset->i = oper2->offset->i & fwd->oper3->offset->i;
	fwd->MarkRemove();
	/*
	if (fwd->fwd)
		fwd->fwd->back = this;
	fwd = fwd->fwd;
	*/
	optimized++;
}


// Remove the sign extension of a value after a sign-extending load.
// This most commonly is generated by character processing.

void OCODE::OptLoadChar()
{
	if (fwd) {
		if (fwd->opcode == op_sext16 || fwd->opcode == op_sxc ||
			(fwd->opcode == op_bfext && fwd->oper3->offset->i == 0 && fwd->oper4->offset->i == 15)) {
			if (fwd->oper1->preg == oper1->preg) {
				fwd->MarkRemove();
				//if (fwd->fwd) {
				//	fwd->fwd->back = this;
				//}
				//fwd = fwd->fwd;
			}
		}
	}
}

void OCODE::OptLoadByte()
{
	if (fwd) {
		if (fwd->opcode == op_sext8 || fwd->opcode == op_sxb ||
			(fwd->opcode == op_bfext && fwd->oper3->offset->i == 0 && fwd->oper4->offset->i == 7)) {
			if (fwd->oper1->preg == oper1->preg) {
				fwd->MarkRemove();
				//if (fwd->fwd) {
				//	fwd->fwd->back = this;
				//}
				//fwd = fwd->fwd;
			}
		}
	}
}


void OCODE::OptLoadHalf()
{
	if (fwd) {
		if (fwd->opcode == op_sext32 || fwd->opcode == op_sxt ||
			(fwd->opcode == op_bfext && fwd->oper3->offset->i == 0 && fwd->oper4->offset->i == 31)) {
			if (fwd->oper1->preg == oper1->preg) {
				fwd->MarkRemove();
				//if (fwd->fwd) {
				//	fwd->fwd->back = this;
				//}
				//fwd = fwd->fwd;
			}
		}
	}
}

// Search ahead and remove this load if the register is def'd again without a use.
void OCODE::OptLoadWord()
{
	OCODE *ip;
	int rg1, rg2;

	/* This function is now redundant - and will be removed. */
	return;
	for (ip = fwd; ip; ip = ip->fwd) {
		if (ip->insn->IsFlowControl())
			goto j1;
		if (ip->opcode == op_call || ip->opcode == op_jsr || ip->opcode == op_jal)
			goto j1;
		if (ip->opcode == op_hint) {
			// Do not remove the reloads of callee saved registers.
			if (ip->oper1->offset->i == begin_stack_unlink)
				goto j1;
			continue;
		}
		if (ip->opcode == op_remark) {
			continue;
		}
		if (ip->HasSourceReg(oper1->preg))
			goto j1;
		if (ip->HasTargetReg()) {
			ip->GetTargetReg(&rg1, &rg2);
			if (rg1 == oper1->preg || rg2 == oper1->preg)
				break;
		}
	}
	// If we get to the end, and no use, then eliminate
	// But, do not remove frame pointer load at end of function.
	if (oper1->preg != regFP) {
		MarkRemove();
		optimized++;
	}
j1:
	;
}

/*
* Search for defs that are overwritten by later defs before they are used.
*/
void OCODE::OptDefUse()
{
	OCODE* ip;
	int rg1, rg2;

	if (!HasTargetReg())
		return;
	for (ip = fwd; ip; ip = ip->fwd) {
		if (ip->insn->IsFlowControl())
			goto j1;
		if (ip->opcode == op_hint)
			continue;
		if (ip->opcode == op_remark)
			continue;
		if (ip->HasSourceReg(oper1->preg))
			goto j1;
		if (ip->HasTargetReg()) {
			ip->GetTargetReg(&rg1, &rg2);
			if (rg1 == oper1->preg || rg2 == oper1->preg)
				break;
		}
	}
	// If we get to the end, and no use, then eliminate
	// But, do not remove frame pointer load at end of function.
	if (!isVolatile && insn->opcode!=op_loadm) {
		if (oper1->preg != regFP
			&& oper1->preg != regSP
			&& oper1->preg != regLR
			&& !IsArgReg(oper1->preg)
			&& !IsSavedReg(oper1->preg)
			)
		{
				MarkRemove();
			optimized++;
		}
	}
j1:
	;
}

/*
* Search for defs wothout a usage.
*/
void OCODE::OptNoUse()
{
	OCODE* ip;

	if (!HasTargetReg())
		return;
	for (ip = currentFn->pl.head; ip; ip = ip->fwd) {
		if (ip == this)
			continue;
		if (ip->HasSourceReg(oper1->preg))
			goto j1;
	}
	// If we get to the end, and no use, then eliminate
	// But, do not remove frame pointer load at end of function.
	if (!isVolatile && insn->opcode!=op_loadm) {
		if (oper1->preg != regFP
			&& !oper1->preg==regSP
			&& !IsArgReg(oper1->preg)
			&& !IsSavedReg(oper1->preg)
			&& oper1->preg != regLR
			) {
			MarkRemove();
			optimized++;
		}
	}
j1:
	;
}

void OCODE::OptSxb()
{
	OCODE* ip;

	ip = back;
	if (oper1->preg == regZero) {
		MarkRemove();
		optimized++;
		return;
	}
	if (oper1->preg == oper2->preg) {
		if (ip->opcode == op_ldb) {
			if (ip->oper1->preg == oper1->preg) {
				MarkRemove();
				optimized++;
			}
		}
	}
	if (fwd == nullptr)
		return;
	if (fwd->opcode != op_and)
		return;
	if (fwd->oper3->mode != am_imm)
		return;
	if (fwd->oper3->offset->i != 255)
		return;
	MarkRemove();
	optimized++;
}



// Place the store half so it's more likely to optimize away a load.

void OCODE::OptStoreHalf()
{
	if (back && (back->opcode == op_bfextu || back->opcode == op_bfext)) {
		if (back->oper1->preg == oper1->preg) {
			if (back->oper3->offset->i == 0 && back->oper4->offset->i == 31) {
				Swap(back, this);
			}
		}
	}
}


// Optimize away a store followed immediately by a load of the same value. But
// do not if it's a volatile variable.
// Eg.
// SH   r3,Address
// LH	r3,Address
// Turns into
// SH   r3,Address

void OCODE::OptStore()
{
	OCODE *ip;

	if (opcode == op_stt)
		OptStoreHalf();
	for (ip = fwd; ip; ip = ip->fwd)
		if (ip->opcode != op_remark && ip->opcode != op_hint)
			break;
	if (opcode == op_label || ip->opcode == op_label)
		return;
	if (!OCODE::IsEqualOperand(oper1, ip->oper1))
		return;
	if (!OCODE::IsEqualOperand(oper2, ip->oper2))
		return;
	if (opcode == op_stt && ip->opcode != op_ldt)
		return;
	if (opcode == op_sto && ip->opcode != op_ldo)
		return;
	if (ip->isVolatile)
		return;
	ip->MarkRemove();
	optimized++;
}

// Remove branch on constants

void OCODE::OptBeq()
{
	if (back && back->opcode == op_cmp && back->oper3->preg == regZero) {
		if (back->back && (back->back->opcode & 0x7fff) == cpu.ldi_op) {
			if (back->back->oper1->preg == back->oper2->preg) {
				if (back->back->oper2->offset->i != 0) {
					back->MarkRemove();
					back->back->MarkRemove();
					MarkRemove();
				}
			}
		}
	}
}

void OCODE::OptIncrBranch()
{
	OCODE* bck;

	if (back->opcode == op_loadi && back->oper1->preg != oper1->preg) {
		bck = back->back;
		if (bck && bck->opcode == op_add && bck->oper3->offset) {
			if (bck->oper3->mode == am_imm) {
				if (bck->oper3->offset->i128.low == 1 && bck->oper3->offset->i128.high == 0) {
					if (bck->oper1->mode == am_reg && bck->oper1->preg == oper1->preg) {
						if (bck->oper2->mode == am_reg && bck->oper2->preg == oper1->preg) {
							switch (this->opcode) {
							case op_beq:	opcode = op_ibeq; insn = Instruction::Get(op_ibeq); bck->MarkRemove(); break;
							case op_bne:	opcode = op_ibne; insn = Instruction::Get(op_ibne); bck->MarkRemove(); break;
							case op_ble:	opcode = op_ible; insn = Instruction::Get(op_ible); bck->MarkRemove(); break;
							case op_blt:	opcode = op_iblt; insn = Instruction::Get(op_iblt); bck->MarkRemove(); break;
							}
						}
					}
				}
			}
		}
	}
	if (back && back->opcode == op_add && back->oper3->offset) {
		if (back->oper3->mode == am_imm) {
			if (back->oper3->offset->i128.low == 1 && back->oper3->offset->i128.high == 0) {
				if (back->oper1->mode == am_reg && back->oper1->preg == oper1->preg) {
					if (back->oper2->mode == am_reg && back->oper2->preg == oper1->preg) {
						switch (this->opcode) {
						case op_beq:	opcode = op_ibeq; insn = Instruction::Get(op_ibeq); back->MarkRemove(); break;
						case op_bne:	opcode = op_ibne; insn = Instruction::Get(op_ibne); back->MarkRemove(); break;
						case op_ble:	opcode = op_ible; insn = Instruction::Get(op_ible); back->MarkRemove(); break;
						case op_blt:	opcode = op_iblt; insn = Instruction::Get(op_iblt); back->MarkRemove(); break;
						}
					}
				}
			}
		}
	}
}


int OCODE::TargetDistance(int64_t i)
{
	OCODE* ip;
	int count;

	count = 1;
	for (ip = this->back; ip; ip = ip->back) {
		if (ip->opcode == op_label) {
			if ((int64_t)ip->oper1 == i)
				return (count);
		}
		count++;
	}
	for (ip = this->fwd; ip; ip = ip->fwd) {
		if (ip->opcode == op_label) {
			if ((int64_t)ip->oper1 == i)
				return (count);
		}
		count++;
	}
	return (0x7fffffffL);
}


void OCODE::OptBne()
{
	if (oper2->mode == am_reg && oper2->preg == regZero) {
		if (TargetDistance(oper3->offset->i) < 512) {
			opcode = op_bnez;
			insn = Instruction::Get(op_bnez);
			oper2 = oper3;
			oper3 = nullptr;
			return;
		}
	}
	if (back && back->opcode == op_cmp && back->oper3->preg == regZero) {
		if (back->back && (back->back->opcode & 0x7fff) == cpu.ldi_op) {
			if (back->back->oper1->preg == back->oper2->preg) {
				if (back->back->oper2->offset->i != 0) {
					back->MarkRemove();
					back->back->MarkRemove();
					opcode = op_branch;
					insn = Instruction::Get(op_branch);
					oper1 = oper2;
					oper2 = nullptr;
				}
			}
		}
	}
}

// Remove instructions that branch to the next label.
//
void OCODE::OptBra()
{
	OCODE *p;
	bool opt = false;

	for (p = fwd; p && (p->opcode == op_label); p = p->fwd)
		if (oper1->offset->i == (int64_t)p->oper1) {
			MarkRemove();
			optimized++;
			opt = true;
			break;
		}
	OptUctran();
	if (!opt) {
		/* bra can branch farther and takes same amount of code.
		if (TargetDistance(oper1->offset->i) < 512) {
			opcode = op_beqz;
			insn = Instruction::Get(op_beqz);
			oper2 = oper1;
			oper1 = makereg(regZero);
			return;
		}
		*/
	}
	return;
}

// Optimize unconditional control flow transfers
// Instructions that follow an unconditional transfer won't be executed
// unless there is a label to branch to them.
//
void OCODE::OptUctran()
{
	if (uctran_off) return;

	while (fwd != nullptr && fwd->opcode != op_label)
	{
		fwd = fwd->fwd;
		if (fwd != nullptr) {
			fwd->back = this;
		}
		optimized++;
	}
}

// JAL is used only to call routines so the Uctran opt is not valid.
// The JAL is encoded as a JMP for unconditional transfers.
void OCODE::OptJAL()
{
	return;
	if (oper1->preg != 0)
		return;
	OptUctran();
}

//
//      changes multiplies and divides by convienient values
//      to shift operations. op should be either op_asl or
//      op_asr (for divide).
//
void OCODE::OptMul()
{
	int shcnt;
	int64_t num;

	if (oper3->mode != am_imm)
		return;
	if (oper3->offset->nodetype != en_icon)
		return;

	num = oper3->offset->i;

	// remove multiply by 1
	// This shouldn't get through Optimize, but does sometimes.
	if (num == 1) {
		MarkRemove();
		optimized++;
		return;
	}
	for (shcnt = 1; shcnt < 64; shcnt++) {
		if (num == (int64_t)1 << shcnt) {
			num = shcnt;
			optimized++;
			break;
		}
	}
	if (shcnt == 64)
		return;
	oper3->offset->i = shcnt;
	opcode = op_asl;
	optimized++;
}

void OCODE::OptMulu()
{
	int shcnt;
	int64_t num;

	if (oper3->mode != am_imm)
		return;
	if (oper3->offset->nodetype != en_icon)
		return;

	num = oper3->offset->i;

	// remove multiply by 1
	// This shouldn't get through Optimize, but does sometimes.
	if (num == 1) {
		MarkRemove();
		optimized++;
		return;
	}
	for (shcnt = 1; shcnt < 64; shcnt++) {
		if (num == (int64_t)1 << shcnt) {
			num = shcnt;
			optimized++;
			break;
		}
	}
	if (shcnt == 64)
		return;
	oper3->offset->i = shcnt;
	opcode = op_stpl;
	optimized++;
}

void OCODE::OptDiv()
{
	int shcnt;
	int64_t num;

	if (oper3->mode != am_imm)
		return;
	if (oper3->offset->nodetype != en_icon)
		return;

	num = oper3->offset->i;

	// remove divide by 1
	// This shouldn't get through Optimize, but does sometimes.
	if (num == 1) {
		MarkRemove();
		optimized++;
		return;
	}
	for (shcnt = 1; shcnt < 64; shcnt++) {
		if (num == (int64_t)1 << shcnt) {
			num = shcnt;
			optimized++;
			break;
		}
	}
	if (shcnt == 64)
		return;
	oper3->offset->i = shcnt;
	opcode = op_asr;
	optimized++;
}

/*
 *      changes multiplies and divides by convienient values
 *      to shift operations. op should be either op_asl or
 *      op_asr (for divide).
 */
void PeepoptMuldiv(OCODE *ip, int op)
{
	int shcnt;
	int64_t num;

	if (ip->oper1->mode != am_imm)
		return;
	if (ip->oper1->offset->nodetype != en_icon)
		return;

	num = ip->oper1->offset->i;

	// remove multiply / divide by 1
	// This shouldn't get through Optimize, but does sometimes.
	if (num == 1) {
		if (ip->back)
			ip->back->fwd = ip->fwd;
		if (ip->fwd)
			ip->fwd->back = ip->back;
		optimized++;
		return;
	}
	for (shcnt = 1; shcnt < 32; shcnt++) {
		if (num == (int64_t)1 << shcnt) {
			num = shcnt;
			optimized++;
			break;
		}
	}
	if (shcnt == 32)
		return;
	ip->oper1->offset->i = num;
	ip->opcode = op;
	ip->length = 2;
	optimized++;
}

// Search backwards for the same set operation.

void OCODE::OptScc()
{
	OCODE* ip;

	for (ip = back; ip; ip = ip->back) {
		if (ip->opcode == op_label)
			return;
		if (ip->insn->IsFlowControl())
			return;
		if (ip->insn->IsSetInsn()) {
			if (ip->opcode != opcode)
				return;
			if (IsEqualOperand(ip->oper1, oper1) && IsEqualOperand(ip->oper2, oper2) && IsEqualOperand(ip->oper3, oper3)) {
				MarkRemove();
				optimized++;
				return;
			}
		}
	}
}

// Search for a shift subsequently used as an index register in a
// following load or store operation. Turn it into a scaled index.

void OCODE::OptSll()
{
	OCODE* ip;
	bool retargetted = false;
	bool dorem = false;
	bool reused = false;
	int rg;

	return;	// Under construction
	if (oper4->mode != am_imm)
		return;
	if (oper4->offset->i < 0 || oper4->offset->i > 7)
		return;
	for (ip = fwd; ip; ip = ip->fwd) {
		if (ip->insn->IsLoad() || ip->insn->IsStore()) {
			if (ip->oper2->mode == am_indx2) {
				if (ip->oper2->scale == 0) {
					if (ip->oper2->sreg == oper1->preg) {
						dorem = true;
						continue;
					}
					// Swap registers?
					else if (ip->oper2->preg == oper1->preg) {
						rg = ip->oper2->preg;
						ip->oper2->preg = ip->oper2->sreg;
						ip->oper2->sreg = rg;
						dorem = true;
						continue;
					}
				}
			}
		}
		// Check that the register is not used by another instruction
		if (!retargetted) {
			if (ip->oper2)
				if (ip->oper2->preg == oper1->preg || ip->oper2->sreg == oper1->preg)
					reused = true;
			if (ip->oper3)
				if (ip->oper3->preg == oper1->preg || ip->oper3->sreg == oper1->preg)
					reused = true;
			if (ip->oper4)
				if (ip->oper4->preg == oper1->preg || ip->oper4->sreg == oper1->preg)
					reused = true;
			if (ip->oper1 && ip->opcode != op_label)
				if (ip->oper1->preg == oper3->preg || ip->oper1->sreg == oper3->preg)
					reused = true;
			if (ip->oper2)
				if (ip->oper2->preg == oper3->preg || ip->oper2->sreg == oper3->preg)
					reused = true;
			if (ip->oper3)
				if (ip->oper3->preg == oper3->preg || ip->oper3->sreg == oper3->preg)
					reused = true;
			if (ip->oper4)
				if (ip->oper4->preg == oper3->preg || ip->oper4->sreg == oper3->preg)
					reused = true;
			if (ip->oper1 && ip->opcode != op_label)
				if (ip->oper1->preg == oper1->preg)
					retargetted = true;
		}
		if (retargetted && dorem) {
			if (ip->oper2->sreg == oper1->preg) {
				ip->oper2->sreg = oper3->preg;
				ip->oper2->scale = (int8_t)oper4->offset->i;
				ip->oper2->scale = 1 << oper4->offset->i;
				optimized++;
				this->MarkRemove();
			}
			// Swap registers?
			else if (ip->oper2->preg == oper1->preg) {
				ip->oper2->preg = ip->oper2->sreg;
				ip->oper2->sreg = oper3->preg;
				ip->oper2->scale = 1 << oper4->offset->i;
				optimized++;
				this->MarkRemove();
			}
			return;
		}
		if (reused)
			return;
	}
	if (ip->oper2->sreg == oper1->preg) {
		ip->oper2->sreg = oper3->preg;
		ip->oper2->scale = (int8_t)oper4->offset->i;
		ip->oper2->scale = 1 << oper4->offset->i;
		optimized++;
		this->MarkRemove();
	}
	// Swap registers?
	else if (ip->oper2->preg == oper1->preg) {
		ip->oper2->preg = ip->oper2->sreg;
		ip->oper2->sreg = oper3->preg;
		ip->oper2->scale = 1 << oper4->offset->i;
		optimized++;
		this->MarkRemove();
	}
}


void OCODE::OptDoubleTargetRemoval()
{
	OCODE *ip2;
	int rg1, rg2, rg3, rg4;

	if (!HasTargetReg())
		return;
	for (ip2 = fwd; ip2 && (ip2->opcode == op_remark || ip2->opcode == op_hint); ip2 = ip2->fwd);
	if (ip2 == nullptr)
		return;
	if (!ip2->HasTargetReg())
		return;
	ip2->GetTargetReg(&rg1, &rg2);
	GetTargetReg(&rg3, &rg4);
	// Should look at this more carefully sometime. Generally however target 
	// register classes won't match between integer and float instructions.
	if ((insn->amclass1 ^ ip2->insn->amclass1) != 0)
		return;
	if (rg1 != rg3)
		return;
	if (ip2->HasSourceReg(rg3))
		return;
	// push has an implicit target, but we don't want to remove it.
	if (opcode == op_push)
		return;
	if (opcode == op_loadm || opcode == op_storem)
		return;
	//if (rg3 == regSP)
	//	return;
	// Set type instructions can merge results into previous target cr.
	if (ip2->insn->IsSetInsn() && ip2->insn2 != nullptr)
		return;
	MarkRemove();
	optimized++;
}

void OCODE::OptIndexScale()
{
	OCODE *frwd;

	if (fwd == nullptr || back == nullptr)
		return;
/*
	// Make sure we have the right kind of a shift left.
	if (back->opcode != op_stpl || back->oper3 == nullptr || back->oper3->offset == nullptr)
		return;
	if (back->oper3->offset->i < 1 || back->oper3->offset->i > 3)
		return;
*/
	// Now search for double indexed operation. There could be multiple matches.
	for (frwd = fwd; frwd; frwd = frwd->fwd) {
		// If there's an intervening flow control, can't optimize.
		if (frwd->insn) {
			if (frwd->insn->IsFlowControl()) {
				frwd = nullptr;
				break;
			}
		}
		// If there's a intervening flow control target, can't optimize.
		if (frwd->opcode == op_label) {
			frwd = nullptr;
			break;
		}
		if (frwd->oper2) {
			// Found a double index.
			if (frwd->oper2->mode == am_indx2) {
				/* if (cpu.supports_indx2_disp
				// Is it the right one?
				if (frwd->oper2->preg == back->oper1->preg) {
					frwd->oper2->preg = back->oper2->preg;
					frwd->oper2->offset = back->oper2->offset;
					//frwd->oper2->scale = 1 << back->oper3->offset->i;
					back->MarkRemove();
					optimized++;
				}
				*/
			}
		}
		// If the target register is assigned to something else
		// abort optimization.
		// If the scaling register is assigned to something else
		// abort optimization.
		else if (frwd->oper1) {
			if (frwd->HasTargetReg()) {
				int rg1, rg2;
				frwd->GetTargetReg(&rg1, &rg2);
				if (rg1 == back->oper1->preg || rg2 == back->oper1->preg) {
					frwd = nullptr;
					break;
				}
				if (back->oper2) {
					if (rg1 == back->oper2->preg || rg2 == back->oper2->preg) {
						frwd = nullptr;
						break;
					}
				}
			}
		}
	}
}

void OCODE::OptCom()
{
	if (back == nullptr || fwd == nullptr)
		return;
	if (fwd->remove || back->remove)
		return;
	// If not all in registers
	if (back->oper1->mode != am_reg
		|| back->oper2->mode != am_reg
		|| (back->oper3 && back->oper3->mode != am_reg))
		return;
	if (back->opcode != op_and
		&& back->opcode != op_or
		&& back->opcode != op_xor
		)
		return;
	if (fwd->opcode != op_com)
		return;
	if (fwd->oper2->mode != am_reg)
		return;
	if (back->oper1->preg != fwd->oper2->preg)
		return;
	if (fwd->opcode != op_com)
		return;
	switch (back->opcode) {
	case op_and:
		back->opcode = op_nand;
		back->insn = GetInsn(op_nand);
		back->oper1->preg = fwd->oper1->preg;
		fwd->MarkRemove();
		optimized++;
		break;
	case op_or:
		back->opcode = op_nor;
		back->insn = GetInsn(op_nor);
		back->oper1->preg = fwd->oper1->preg;
		fwd->MarkRemove();
		optimized++;
		break;
	case op_xor:
		back->opcode = op_xnor;
		back->insn = GetInsn(op_xnor);
		back->oper1->preg = fwd->oper1->preg;
		fwd->MarkRemove();
		optimized++;
		break;
	}
}

// Process compiler hint opcodes

void OCODE::OptHint()
{
	OCODE *frwd, *bck;
	Operand *am;

	if ((back && back->opcode == op_label) || (fwd && fwd->opcode == op_label))
		return;
	if (remove)
		return;
	if (remove)
		return;

	switch (oper1->offset->i) {

		// hint #1
		// Takes care of redundant moves at the parameter setup point
		// Code Like:
		//    MOV r3,#constant
		//    MOV r18,r3
		// Translated to:
		//    MOV r18,#constant
	case 1:

		if (fwd && fwd->opcode != cpu.mov_op) {
			Remove();	// remove the hint
			optimized++;
			return;
		}

		if (fwd && IsArgReg(fwd->oper1->preg)) {
			if (OCODE::IsEqualOperand(fwd->oper2, back->oper1)) {
				back->oper1 = fwd->oper1;
				MarkRemove();
				fwd->MarkRemove();
				optimized++;
				return;
			}
		}

		if (back && back->opcode != cpu.mov_op) {
			MarkRemove();
			optimized++;
			return;
		}

		if (IsEqualOperand(fwd->oper2, back->oper1)) {
			back->oper1 = fwd->oper1;
			MarkRemove();
			fwd->MarkRemove();
			optimized++;
		}
		else {
			MarkRemove();
			optimized++;
		}
		break;

		// Can't do this optimization:
		// what if x = (~(y=(a & b)))
		// The embedded assignment to y which might be used later would be lost.
		//
		// hint #2
		// Takes care of redundant moves at the function return point
		// Code like:
		//     MOV R3,arg
		//     MOV R1,R3
		// Translated to:
		//     MOV r1,arg
	case 2:
		// Optimization disabled for now. Does not always work correctly.
		break;
		// This optimization didn't work with:
		// ldi $t1,#0
		// mov $t0,$t1
		// It optimized it to:
		// ldi  $t1,#0
		// It didn't set the back->oper1 properly.
		if (fwd == nullptr || back == nullptr)
			break;
//		if (back->opcode != op_mov || fwd->opcode != op_mov) {
		if (fwd->opcode != cpu.mov_op) {
			break;
		}
		if (IsEqualOperand(fwd->oper2, back->oper1)) {
			if (back->HasTargetReg()) {
				int rg1, rg2;
				back->GetTargetReg(&rg1, &rg2);
				if (!((fwd->oper1->mode == am_reg || fwd->oper1->mode == am_vreg) &&
					(back->opcode & 0x7fff) == cpu.ldi_op)) {
					// Search forward to see if the target register is used anywhere.
					for (frwd = fwd->fwd; frwd; frwd = frwd->fwd) {
						// If the register has been targeted again, it is okay to opt.
						if (frwd->HasTargetReg()) {
							frwd->GetTargetReg(&rg1, &rg2);
							if (back->oper1) {
								if (rg1 == back->oper1->preg && back->insn->amclass1 == frwd->insn->amclass1)
									break;
							}
						}
						if (frwd->HasSourceReg(back->oper1->preg)) {
							return;
						}
					}
					back->oper1 = fwd->oper1->Clone();
					fwd->MarkRemove();
					MarkRemove();
					optimized++;
				}
			}
		}
		break;

		// hint #3
		//	   and r5,r2,r3
		//     com r1,r5
		// Translates to:
		//     nand r5,r2,r3
	case 3:
		OptCom();
		break;

		// hint #9
		// Index calc.
		//		shl r1,r3,#3
		//		sw r4,[r11+r1]
		// Becomes:
		//		sw r4,[r11+r3*8]
	case 9:
		OptIndexScale();
		break;
		// Following is dead code
		//if (fwd->oper2->mode != am_indx2)
		//	break;
		if (fwd->oper2->preg == back->oper1->preg) {
			if ((back->opcode == op_stpl) && back->oper3->offset &&
				(back->oper3->offset->i == 1
					|| back->oper3->offset->i == 2
					|| back->oper3->offset->i == 3)) {
				fwd->oper2->preg = back->oper2->preg;
				fwd->oper2->scale = 1 << back->oper3->offset->i;
				back->MarkRemove();
				optimized++;
				am = back->oper1;
				frwd = fwd->fwd;
				bck = back->back;
				while (back->opcode == op_hint)	// It should be
					bck = back->back;
				// We search backwards for another shl related to a forward op to
				// accomodate assignment operations. Assignment operations may
				// generate indexed code like the following:
				//    shl for target
				//    shl for source
				//    load source
				//    store target
				if (frwd->oper2) {
					if ((bck->opcode == op_stpl) && bck->oper3->offset &&
						(am->preg != frwd->oper2->preg && am->preg != frwd->oper2->sreg) &&
						(bck->oper3->offset->i == 1
							|| bck->oper3->offset->i == 2
							|| bck->oper3->offset->i == 3)
						) {
						frwd->oper2->preg = bck->oper2->preg;
						frwd->oper2->scale = 1 << bck->oper3->offset->i;
						bck->MarkRemove();
						optimized++;
					}
				}
			}
		}
		break;
	}
}

// Remove extra labels at end of subroutines

void OCODE::OptLabel()
{
	if (this == nullptr)
		return;
	if (fwd)
		return;
	if (back)
		back->fwd = nullptr;
	optimized++;
}


// Search ahead for additional LDI instructions loading the same constant
// and remove them.
// Remove sign / zero extension when not needed.

void OCODE::OptLdi()
{
	OCODE *ip;

	if (fwd) {
		if (oper2->offset && oper2->offset->constflag) {
			if (fwd->opcode == op_sxo) {
				if (oper2->offset->i128.IsNBit(64)) {
					fwd->MarkRemove();
					optimized++;
				}
			}
			if (fwd->opcode == op_sxt) {
				if (oper2->offset->i128.IsNBit(32)) {
					fwd->MarkRemove();
					optimized++;
				}
			}
			if (fwd->opcode == op_sxw || fwd->opcode == op_sxc) {
				if (oper2->offset->i128.IsNBit(16)) {
					fwd->MarkRemove();
					optimized++;
				}
			}
			if (fwd->opcode == op_sxb) {
				if (oper2->offset->i128.IsNBit(8)) {
					fwd->MarkRemove();
					optimized++;
				}
			}
			if (fwd->opcode == op_sbx) {
				if (fwd->oper3->offset->i <= 8) {
					if (oper2->offset->i128.IsNBit(8)) {
						fwd->MarkRemove();
						optimized++;
					}
				}
			}
			if (fwd->opcode == op_sbx) {
				if (fwd->oper3->offset->i <= 16) {
					if (oper2->offset->i128.IsNBit(16)) {
						fwd->MarkRemove();
						optimized++;
					}
				}
			}
			if (fwd->opcode == op_sbx) {
				if (fwd->oper3->offset->i <= 32) {
					if (oper2->offset->i128.IsNBit(32)) {
						fwd->MarkRemove();
						optimized++;
					}
				}
			}
			if (fwd->opcode == op_sbx) {
				if (fwd->oper3->offset->i <= 64) {
					if (oper2->offset->i128.IsNBit(64)) {
						fwd->MarkRemove();
						optimized++;
					}
				}
			}
			if (fwd->opcode == op_zxt) {
				if (oper2->offset->i >= 0 && oper2->offset->i <= 4294967295L) {
					fwd->MarkRemove();
					optimized++;
				}
			}
			if (fwd->opcode == op_zxo) {
				if (oper2->offset->i >= 0 && oper2->offset->i <= 65535) {
					fwd->MarkRemove();
					optimized++;
				}
			}
			if (fwd->opcode == op_clr) {
				if (fwd->oper3->offset->i <= 32) {
					if (oper2->offset->i >= 0 && oper2->offset->i <= 4294967295L) {
						fwd->MarkRemove();
						optimized++;
					}
				}
			}
			if (fwd->opcode == op_clr) {
				if (fwd->oper3->offset->i <= 16) {
					if (oper2->offset->i >= 0 && oper2->offset->i <= 65535) {
						fwd->MarkRemove();
						optimized++;
					}
				}
			}
			if (fwd->opcode == op_zxb) {
				if (oper2->offset->i >= 0 && oper2->offset->i <= 255) {
					fwd->MarkRemove();
					optimized++;
				}
			}
			if (fwd->opcode == op_clr) {
				if (fwd->oper3->offset->i <= 8) {
					if (oper2->offset->i >= 0 && oper2->offset->i <= 65535) {
						fwd->MarkRemove();
						optimized++;
					}
				}
			}
		}
	}
	for (ip = fwd; ip; ip = ip->fwd) {
		if (ip->HasTargetReg()) {
			int rg1, rg2;
			ip->GetTargetReg(&rg1, &rg2);
			if (ip->opcode != op_pfx0 && ip->opcode != op_pfx1 && ip->opcode != op_pfx2 && ip->opcode != op_pfx3) {
				if ((ip->opcode & 0x7fff) == cpu.ldi_op) {
					if (rg1 == oper1->preg || rg2 == oper1->preg) {
						if (ip->oper2->offset->i == oper2->offset->i) {
							ip->MarkRemove();
							optimized++;
						}
						else
							return;
					}
					else
						return;
				}
				else if (rg1 == oper1->preg || rg2 == oper1->preg)
					return;
			}
		}
	}
}


void OCODE::OptLea()
{
	OCODE *ip, *ip2;
	bool opt = true;

	// Remove a move following a LEA
	ip = fwd;
	if (ip) {
		if (ip->opcode == cpu.mov_op) {
			if (ip->oper2->preg == oper1->preg) {
				for (ip2 = ip->fwd; ip2; ip2 = ip2->fwd)
				{
					if (ip2->opcode == op_label) {
						opt = false;
						break;
					}
					if (ip2->HasTargetReg(oper1->preg))
						break;
					if (ip2->HasSourceReg(oper1->preg)) {
						opt = false;
						break;
					}
				}
				if (opt) {
					oper1->preg = ip->oper1->preg;
					ip->MarkRemove();
					optimized++;
				}
			}
		}
	}
	for (ip = fwd; ip; ip = ip->fwd) {
		if (ip->HasTargetReg()) {
			int rg1, rg2;
			ip->GetTargetReg(&rg1, &rg2);
			if ((ip->opcode & 0x7fff) == cpu.ldi_op) {
				if (rg1 == oper1->preg || rg2 == oper1->preg) {
					if (ip->oper2->offset->i == oper2->offset->i) {
						ip->MarkRemove();
						optimized++;
					}
					else
						return;
				}
				else
					return;
			}
		}
	}
}

// Converts up to four separate register pushes into one operation.
void OCODE::OptPush()
{
	OCODE *ip;

	//return;
	if (oper1->mode != am_reg)
		return;
	ip = fwd;
	if (ip && !ip->remove) {
		if (ip->opcode == op_push && cpu.pushpop_multiple > 1) {
			if (ip->oper1->mode == am_reg) {
				if (oper2==nullptr)
					oper2 = makereg(ip->oper1->preg);
				else if (oper3==nullptr)
					oper3 = makereg(ip->oper1->preg);
				else if (oper4 == nullptr)
					oper4 = makereg(ip->oper1->preg);
				ip->MarkRemove();
				if (ip) {
					ip = ip->fwd;
					if (ip) {
						if (ip->opcode == op_push && cpu.pushpop_multiple > 2) {
							if (ip->oper1->mode == am_reg) {
								if (oper2 == nullptr)
									oper2 = makereg(ip->oper1->preg);
								else if (oper3 == nullptr)
									oper3 = makereg(ip->oper1->preg);
								else if (oper4 == nullptr)
									oper4 = makereg(ip->oper1->preg);
								ip->MarkRemove();
								if (ip) {
									ip = ip->fwd;
									if (ip) {
										if (ip->opcode == op_push && cpu.pushpop_multiple > 3) {
											if (ip->oper1->mode == am_reg) {
												if (oper2 == nullptr)
													oper2 = makereg(ip->oper1->preg);
												else if (oper3 == nullptr)
													oper3 = makereg(ip->oper1->preg);
												else if (oper4 == nullptr)
													oper4 = makereg(ip->oper1->preg);
												ip->MarkRemove();
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

// Converts up to four separate register pops into one operation.
void OCODE::OptPop()
{
	OCODE* ip;

	//return;
	if (oper1->mode != am_reg)
		return;
	ip = fwd;
	if (ip && !ip->remove) {
		if (ip->opcode == op_pop && cpu.pushpop_multiple > 1) {
			if (ip->oper1->mode == am_reg) {
				if (oper2 == nullptr)
					oper2 = makereg(ip->oper1->preg);
				else if (oper3 == nullptr)
					oper3 = makereg(ip->oper1->preg);
				else if (oper4 == nullptr)
					oper4 = makereg(ip->oper1->preg);
				ip->MarkRemove();
				if (ip) {
					ip = ip->fwd;
					if (ip) {
						if (ip->opcode == op_pop && cpu.pushpop_multiple > 2) {
							if (ip->oper1->mode == am_reg) {
								if (oper2 == nullptr)
									oper2 = makereg(ip->oper1->preg);
								else if (oper3 == nullptr)
									oper3 = makereg(ip->oper1->preg);
								else if (oper4 == nullptr)
									oper4 = makereg(ip->oper1->preg);
								ip->MarkRemove();
								if (ip) {
									ip = ip->fwd;
									if (ip) {
										if (ip->opcode == op_pop && cpu.pushpop_multiple > 3) {
											if (ip->oper1->mode == am_reg) {
												if (oper2 == nullptr)
													oper2 = makereg(ip->oper1->preg);
												else if (oper3 == nullptr)
													oper3 = makereg(ip->oper1->preg);
												else if (oper4 == nullptr)
													oper4 = makereg(ip->oper1->preg);
												ip->MarkRemove();
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

// Remove SXW after LDW
// No point in extending r0.
void OCODE::OptSxw()
{
	OCODE* ip;

	ip = back;
	if (oper1->preg != oper2->preg)
		return;
	if (oper1->preg == regZero) {
		MarkRemove();
		optimized++;
		return;
	}
	if (ip->opcode == op_ldw) {
		if (ip->oper1->preg == oper1->preg) {
			MarkRemove();
			optimized++;
		}
	}
}

// Remove ZXW after LDWU
void OCODE::OptZxw()
{
	OCODE* ip;

	ip = back;
	if (oper1->preg != oper2->preg)
		return;
	if (ip->opcode == op_ldwu) {
		if (ip->oper1->preg == oper1->preg) {
			MarkRemove();
			optimized++;
		}
	}
}

// Remove ZXB after LDBU
void OCODE::OptZxb()
{
	OCODE* ip;

	ip = back;
	if (oper1->preg != oper2->preg)
		return;
	if (ip->opcode == op_ldbu) {
		if (ip->oper1->preg == oper1->preg) {
			MarkRemove();
			optimized++;
		}
	}
}



void OCODE::storeHex(txtoStream& ofs)
{
	ENODE *ep;

	switch (opcode) {
	case op_label:
		if (str) {
			ofs.printf((char *)oper1);
			ofs.printf(GetNamespace());
		}
		else {
			ofs.printf("L");
			ofs.printf("%05X", (int)oper1);
			ofs.printf(GetNamespace());
		}
		ofs.printf("\n");
		break;
	case op_fnname:
		ep = (ENODE *)oper1->offset;
		ofs.printf("F%s:\n", (char *)ep->sp->c_str());
		//ofs.printf("F%s .proc\n", (char *)ep->sp->c_str());
		break;
	default:
		ofs.printf("C");
		insn->storeHex(ofs);
		ofs.printf("L%01d", length);
		if (oper1) oper1->storeHex(ofs);
		if (oper2) oper2->storeHex(ofs);
		if (oper3) oper3->storeHex(ofs);
		if (oper4) oper4->storeHex(ofs);
		ofs.printf("\n");
	}
}

OCODE *OCODE::loadHex(txtiStream& ifs)
{
	OCODE *cd;
	char buf[20];

	cd = (OCODE *)allocx(sizeof(OCODE));
	ifs.read(buf, 1);
	if (buf[0] != 'I') {
		while (!ifs.eof() && buf[0] != '\n')
			ifs.read(buf, 1);
		return (nullptr);
	}
	cd->insn = Instruction::loadHex(ifs);
	cd->opcode = cd->insn->opcode;
	ifs.read(buf, 1);
	if (buf[0] == 'L') {
		ifs.read(buf, 1);
		buf[1] = '\0';
		cd->length = atoi(buf);
		ifs.read(buf, 1);
	}
	cd->oper1 = nullptr;
	cd->oper2 = nullptr;
	cd->oper3 = nullptr;
	cd->oper4 = nullptr;
	switch (buf[0]) {
	case '1': cd->oper1 = Operand::loadHex(ifs); break;
	case '2': cd->oper2 = Operand::loadHex(ifs); break;
	case '3': cd->oper3 = Operand::loadHex(ifs); break;
	case '4': cd->oper4 = Operand::loadHex(ifs); break;
	default:
		while (!ifs.eof() && buf[0] != '\n')
			ifs.read(buf, 1);
	}
	return (cd);
}

//
// Output a generic instruction.
//
void OCODE::store(txtoStream& ofs)
{
	static BasicBlock *b = nullptr;
	int op = opcode & 0x7fff;
	int dot = opcode & 0x8000;
	Operand *ap1, *ap2, *ap3, *ap4;
	ENODE *ep;
	int predreg = pregreg;
	char buf[8];
	int64_t nn;
	bool addi = false;
	char ccch;		// comment character

	switch (syntax) {
	case MOT:
		ccch = ';';
		break;
	default:
		ccch = comment_char;
	}

	nn = 0;
	ap1 = oper1;
	ap2 = oper2;
	ap3 = oper3;
	ap4 = oper4;
	if ((ap2 && ap2->mode == am_imm) || (ap3 && ap3->mode == am_imm) || (ap4 && ap4->mode == am_imm))
		addi = cpu.Addsi;
#ifdef RISCV
	if (isRiscv && opcode == op_l)
		addi = true;
#endif
	if (bb != b) {
		if (false && bb->num == 0) {
			switch (syntax) {
			case MOT:
				ofs.printf(";====================================================\n");
				ofs.printf("; Basic Block %d\n", bb->num);
				ofs.printf(";====================================================\n");
				break;
			default:
				ofs.printf("%c====================================================\n",comment_char);
				ofs.printf("%c ", comment_char);
				ofs.printf("Basic Block %d\n", bb->num);
				ofs.printf("%c====================================================\n",comment_char);
			}
		}
		b = bb;
	}
	if (comment) {
		switch (syntax) {
		case MOT:
			ofs.printf(";%s\n", (char*)comment->oper1->offset->sp->c_str());
			break;
		default:
			ofs.printf("%c", comment_char);
			ofs.printf("%s\n", (char*)comment->oper1->offset->sp->c_str());
		}
	}
	if (remove)
		ofs.printf("%c-1", ccch);
	if (remove2)
		ofs.printf("%c-2", ccch);
	if (op != op_fnname)
	{
		if (op == op_rem2) {
			ofs.printf("%c\t", ccch);
			ofs.printf((char *)"%6.6s\t", (char *)"");
			ofs.printf(ap1->offset->sp->c_str());
			ofs.printf("\n");
			return;
		}
		else {
			//ofs.printf("\t");
			//ofs.printf("%6.6s\t", "");
			ofs.printf("  ");	// 2 spaces
			if (insn) {
				if (op == op_string) {
					switch (syntax) {
					case MOT:
						ofs.printf("dc.w");
						break;
					default:
						ofs.printf(".2byte");
					}
				}
				else {
					nn = insn->store(ofs);
					if (addi) {
						ofs.write("i");
						nn++;
					}
				}
			}
			if (insn2) {
				ofs.printf(".");
				nn = nn + insn2->store(ofs) + 1;
			}
			buf[0] = '\0';
			if (length) {
				if (length <= 16) {
					switch (length) {
					case 1:	sprintf_s(buf, sizeof(buf), ".b"); nn += 2; break;
					case 2:	sprintf_s(buf, sizeof(buf), ".w"); nn += 2; break;
					case 4:	sprintf_s(buf, sizeof(buf), ".t"); nn += 2; break;
					case 8:	sprintf_s(buf, sizeof(buf), ".o"); nn += 2; break;
					case 16:	sprintf_s(buf, sizeof(buf), ".h"); nn += 2; break;
					}
				}
				else {
					if (length && length != ' ') {
						sprintf_s(buf, sizeof(buf), ".%c", length);
						nn += 2;
					}
				}
			}
			if (ext && ext->length() > 0) {
				ofs << *ext;
			}
			if (dot) {
				ofs.printf(".");
				nn = nn + 1;
			}
			ofs.write(buf);
			// The longest mnemonic is 7 chars
			{
				ofs.write(" ");
				nn++;
			}
		}
	}
	if (op == op_fnname) {
		ep = (ENODE *)oper1->offset;
		ofs.printf("%s:", (char*)ep->sp->c_str());
//		ofs.printf("%s .proc", (char *)ep->sp->c_str());
	}
	else if (ap1 != 0)
	{
		ap1->store(ofs);
		if (ap2 != 0)
		{
			if (op == op_push || op == op_pop)
				ofs.printf(",");
			else
				ofs.printf(",");
			//if (op == op_cmp && ap2->mode != am_reg)
			//	printf("aha\r\n");
			ap2->store(ofs);
			if (ap3 != NULL) {
				if (op == op_push || op == op_pop)
					ofs.printf(",");
				else
					ofs.printf(",");
				ap3->store(ofs);
				if (ap4 != NULL) {
					if (op == op_push || op == op_pop)
						ofs.printf(",");
					else
						ofs.printf(",");
					ap4->store(ofs);
				}
			}
		}
	}
	ofs.printf("\n");
}



