// ============================================================================
//        __
//   \\__/ o\    (C) 2020-2024  Robert Finch, Waterloo
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

CPU::CPU() {
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
}

void CPU::SetRealRegisters()
{
	int n;

	regSP = 63;
	regFP = 62;
	regLR = 56;
	regGP = 61;
	regTP = 26;
	regCLP = 25;                // class pointer
	regPP = 24;					// program pointer
	regZero = 0;
	regXoffs = 10;
	nregs = 64;
	for (n = 0; n < nregs; n++) {
		::regs[n].number = n;
		::regs[n].assigned = false;
		::regs[n].isConst = false;
		::regs[n].modified = false;
		::regs[n].offset = nullptr;
		::regs[n].IsArg = false;
		::regs[n].isGP = (n < 32);
		::regs[n].isFP = (n >= 32 && n < 64);
		::regs[n].isPosit = (n >= 64 && n < 96);

		vregs[n].number = n;
		vregs[n].assigned = false;
		vregs[n].isConst = false;
		vregs[n].modified = false;
		vregs[n].offset = nullptr;
		vregs[n].IsArg = false;
	}
	MachineReg::MarkColorable();
}

void CPU::SetVirtualRegisters()
{
	int n;

	regSP = 1023;
	regFP = 1022;
	regLR = 1021;
	regGP = 1019;
	regTP = 1018;
	regCLP = 1017;              // class pointer
	regPP = 1016;				// program pointer
	regAsm = 1015;
	regXoffs = 1014;
	regZero = 0;
	nregs = 1024;
	for (n = 0; n < nregs; n++) {
		::regs[n].number = n;
		::regs[n].assigned = false;
		::regs[n].isConst = false;
		::regs[n].modified = false;
		::regs[n].offset = nullptr;
		::regs[n].IsArg = false;

		vregs[n].number = n;
		vregs[n].assigned = false;
		vregs[n].isConst = false;
		vregs[n].modified = false;
		vregs[n].offset = nullptr;
		vregs[n].IsArg = false;
	}
	MachineReg::MarkColorable();
}

int CPU::GetTypePrecision(e_bt typ)
{
	switch (typ) {
	case bt_bit: return (1);
	case bt_byte:	return (8);
	case bt_char:
	case bt_uchar:
	case bt_ichar:
	case bt_iuchar:
	case bt_exception:
	case bt_enum:
		return (16);
	case bt_bool:	return (1);
	case bt_short: return (sizeOfInt * 4);
	case bt_int:	return (sizeOfInt * 8);
	case bt_long:	return (sizeOfInt * 16);
	case bt_float: return (sizeOfFPS * 8);
	case bt_double: return (sizeOfFPD * 8);
	case bt_quad: return (sizeOfFPQ * 8);
	case bt_pointer: return (sizeOfPtr * 8);
	case bt_decimal: return (sizeOfDecimal * 8);
	case bt_void: return (0);
	case bt_vector: return (64 * 8);
	case bt_vector_mask: return (sizeOfWord * 8);
	default: return (sizeOfWord * 8);
	}
}

int CPU::GetTypeSize(e_bt typ)
{
	return (GetTypePrecision(typ) / 8);
}

char* CPU::RegMoniker(int32_t regno)
{
	static char buf[4][20];
	static int n;
	int rg;
	bool invert = false;
	bool vector = false;
	bool group = false;
	bool is_float = false;
	bool is_cr = false;

	if (regno & rt_group) {
		group = true;
		regno &= 0xff;
	}
	if (regno & rt_invert) {
		invert = true;
		regno &= 0xbf;
	}
	if (regno & rt_vector) {
		vector = true;
		regno &= 0x3f;
	}
	if (regno & rt_float) {
		is_float = true;
		regno &= 0x3f;
	}
	if (IsTempCrReg(regno)) {
		is_cr = true;
		regno &= 0xfffL;
	}
	n = (n + 1) & 3;
	if (vector) {
		if (invert)
			sprintf_s(&buf[n][0], 20, "~v%d", regno);
		else
			sprintf_s(&buf[n][0], 20, "v%d", regno);
		return (&buf[n][0]);
	}
	if (group) {
		if (invert)
			sprintf_s(&buf[n][0], 20, "~g%d", regno);
		else
			sprintf_s(&buf[n][0], 20, "g%d", regno);
		return (&buf[n][0]);
	}
	if (is_float) {
		if (rg = IsFtmpReg(regno))
			sprintf_s(&buf[n][0], 20, "~ft%d", rg - 1);
		else if (rg = IsFargReg(regno))
			sprintf_s(&buf[n][0], 20, "~ft%d", rg - 1);
		else if (rg = IsFsavedReg(regno))
			sprintf_s(&buf[n][0], 20, "~fs%d", rg - 1);
		return (invert ? &buf[n][0] : &buf[n][1]);
	}
	if (is_cr) {
		if (regno == 0xfffL)
			sprintf_s(&buf[n][0], 20, "crg");
		else
			sprintf_s(&buf[n][0], 20, "cr%d", regno & 7);
	}
	else if (rg = IsTempReg(regno)) {
		if (invert)
			sprintf_s(&buf[n][0], 20, "~t%d", rg - 1);// tmpregs[rg - 1]);
		else
			sprintf_s(&buf[n][0], 20, "t%d", rg - 1);// tmpregs[rg - 1]);
	}
	else if (rg = IsArgReg(regno)) {
		if (invert)
			sprintf_s(&buf[n][0], 20, "~a%d", rg - 1);// tmpregs[rg - 1]);
		else
			sprintf_s(&buf[n][0], 20, "a%d", rg - 1);// tmpregs[rg - 1]);
	}
	else if (rg = IsSavedReg(regno)) {
		if (invert)
			sprintf_s(&buf[n][0], 20, "~s%d", rg - 1);
		else
			sprintf_s(&buf[n][0], 20, "s%d", rg - 1);
	}
	else
		if (regno == regFP)
			sprintf_s(&buf[n][0], 20, "fp");
	//		else if (regno == regAFP)
	//			sprintf_s(&buf[n][0], 20, "$afp");
		else if (regno == regGP)
			sprintf_s(&buf[n][0], 20, "gp");
		else if (regno == regGP1)
			sprintf_s(&buf[n][0], 20, "gp1");
	//	else if (regno==regPC)
//		sprintf_s(&buf[n][0], 20, "$pc");
		else if (regno == regSP)
			sprintf_s(&buf[n][0], 20, "sp");
		else if (regno == regLR)
			sprintf_s(&buf[n][0], 20, "lr0");
		else if (regno == regLR + 1)
			sprintf_s(&buf[n][0], 20, "lr1");
		else if (regno == regLR + 2)
			sprintf_s(&buf[n][0], 20, "lr2");
		else if (regno == regLR + 3)
			sprintf_s(&buf[n][0], 20, "lr3");
		else if (regno == 0) {
			if (invert)
				sprintf_s(&buf[n][0], 20, "~r%d", regno);
			else
				sprintf_s(&buf[n][0], 20, "r%d", regno);
		}
		else if (regno == 2)
			sprintf_s(&buf[n][0], 20, "r%d", regno);
		else {
			if ((regno & 0x70) == 0x040)
				sprintf_s(&buf[n][0], 20, "$p%d", regno & 0x1f);
			else if ((regno & 0x70) == 0x070)
				sprintf_s(&buf[n][0], 20, "$cr%d", regno & 0x3);
			else
				sprintf_s(&buf[n][0], 20, "r%d", regno);
		}
	return &buf[n][0];
}
