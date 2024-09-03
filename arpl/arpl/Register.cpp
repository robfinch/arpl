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

extern void initFPRegStack();
extern void ReleaseTempFPRegister(Operand *);
/*
int tmpregs[] = {3,4,5,6,7,8,9,10};
int regstack[8];
int rsp=7;
int regmask=0;

int tmpbregs[] = {3,4,5,6,7,8};
int bregstack[6];
int brsp=5;
int bregmask = 0;
*/
CSet* stacked_regs_set;
CSet stacked_ccr_regs_set;
static unsigned short int next_reg;
static unsigned short int next_fpreg;
static unsigned short int next_preg;
static unsigned short int next_vreg;
static unsigned short int next_vmreg;
static unsigned short int nextCrReg;
static short int next_breg;
int max_reg_alloc_ptr;
int maxCrRegAllocPtr;
int max_stack_use;
int max_freg_alloc_ptr;
int max_fstack_use;
int max_vreg_alloc_ptr;
int max_vstack_use;
#define MAX_REG 4			/* max. scratch data	register (D2) */
#define	MAX_REG_STACK	30
#define VR_COUNT 2048
#define VR_MASK VR_COUNT-1

// Only registers 5,6,7 and 8 are used for temporaries
static short int fpreg_in_use[VR_COUNT];	// 0 to 15
static short int preg_in_use[VR_COUNT];	// 0 to 15
static short int breg_in_use[16];	// 0 to 15
static short int save_reg_in_use[VR_COUNT];
static short int CrRegInUse[16];
static short int saveCrRegInUse[VR_COUNT];
static short int save_fpreg_in_use[VR_COUNT];
static short int save_preg_in_use[VR_COUNT];
static short int vreg_in_use[VR_COUNT];	// 0 to 15
static short int save_vreg_in_use[VR_COUNT];
static short int vmreg_in_use[VR_COUNT];	// 0 to 15

static int wrapno, save_wrapno;
static int wrapno_1;
static int CrWrapno, saveCrWrapno;
static int CrWrapno_1;

static struct {
	Operand *Operand;
  int reg;
	struct {
	char isPushed;	/* flags if pushed or corresponding reg_alloc * number */
	char allocnum;
	} f;
} 
	reg_stack[MAX_REG_STACK + 1],
	reg_alloc[MAX_REG_STACK + 1],
	save_reg_alloc[MAX_REG_STACK + 1],
	fpreg_stack[MAX_REG_STACK + 1],
	fpreg_alloc[MAX_REG_STACK + 1],
	save_fpreg_alloc[MAX_REG_STACK + 1],
	preg_stack[MAX_REG_STACK + 1],
	preg_alloc[MAX_REG_STACK + 1],
	save_preg_alloc[MAX_REG_STACK + 1],

	CrRegStack[MAX_REG_STACK + 1],
	CrRegAlloc[MAX_REG_STACK + 1],
	stackedCrRegs[MAX_REG_STACK + 1],
	saveCrRegAlloc[MAX_REG_STACK + 1],

	stacked_regs[MAX_REG_STACK + 1],
	stacked_fpregs[MAX_REG_STACK + 1],
	stacked_pregs[MAX_REG_STACK + 1],
	breg_stack[MAX_REG_STACK + 1],
	breg_alloc[MAX_REG_STACK + 1],
	vreg_stack[MAX_REG_STACK + 1],
	vreg_alloc[MAX_REG_STACK + 1],
	vmreg_stack[MAX_REG_STACK + 1],
	vmreg_alloc[MAX_REG_STACK + 1],
	save_vreg_alloc[MAX_REG_STACK + 1],
	save_vmreg_alloc[MAX_REG_STACK + 1],
	stacked_vregs[MAX_REG_STACK + 1],
	stacked_vmregs[MAX_REG_STACK + 1]
;

static short int reg_stack_ptr;
static short int reg_alloc_ptr;
static short int save_reg_alloc_ptr;
static short int fpreg_stack_ptr;
static short int fpreg_alloc_ptr;
static short int save_fpreg_alloc_ptr;
static short int preg_stack_ptr;
static short int preg_alloc_ptr;
static short int save_preg_alloc_ptr;
static short int vreg_stack_ptr;
static short int vreg_alloc_ptr;
static short int save_vreg_alloc_ptr;
static short int vmreg_stack_ptr;
static short int vmreg_alloc_ptr;
static short int save_vmreg_alloc_ptr;
static short int breg_stack_ptr;
static short int breg_alloc_ptr;

static short int CrRegStackPtr;
static short int CrRegAllocPtr;
static short int saveCrRegAllocPtr;

//char tmpregs[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
char tmpfpregs[] = {1,2,3,4,5,6,7,8,9,10};
char tmppregs[] = { 1,2,3,4,5,6,7,8,9,10 };
char tmpvregs[] = {1,2,3,4,5,6,7,8,9,10};
char tmpvmregs[] = {1,2,3};
char tmpbregs[] = {5,6,7};
char tmpCrRegs[] = { 2,3,4,5 };
char regstack[18];
char fpregstack[18];
char pregstack[18];
char bregstack[18];
int rsp=17;
int regmask=0;
int brsp=17;
int bregmask=0;
int rap[20];
int Crrap[20];
int save_rap[20];
int save_Crrap[20];
int CrMask;
int Crsp = 17;

int NumTempRegs()
{
	return (cpu.NumTmpRegs);
}

int NumSavedRegs()
{
	return (cpu.NumSavedRegs);
}

int NumTempCrRegs()
{
	return (cpu.NumTmpCrRegs);
}

int NumSavedCrRegs()
{
	return (cpu.NumSavedCrRegs);
}

void CPU::InitRegs()
{
	int ii;

	for (ii = 0; ii < 64; ii++) {
		cpu.regs[ii].number = 0;
		cpu.regs[ii].isCr = false;
		cpu.regs[ii].isCrg = false;
		cpu.regs[ii].isArg = false;
		cpu.regs[ii].isTemp = false;
		cpu.regs[ii].isSaved = false;
		cpu.regs[ii].inUse.clear();
		cpu.regs[ii].isPushed.clear();
		if (ii < 32)
			cpu.regs[ii].cls = rc_gpr;
		else if (ii < 40)
			cpu.regs[ii].cls = rc_ccr;
		else if (ii == 40)
			cpu.regs[ii].cls = rc_ccrg;
	}

#ifdef QUPLS
	cpu.NumRegs = 32;
	cpu.NumArgRegs = 8;
	cpu.argregs[0] = 1;
	cpu.argregs[1] = 2;
	cpu.argregs[2] = 3;
	cpu.argregs[3] = 4;
	cpu.argregs[4] = 5;
	cpu.argregs[5] = 6;
	cpu.argregs[6] = 7;
	cpu.argregs[7] = 8;
	cpu.argregs[8] = 0;
	cpu.argregs[9] = 0;
	cpu.argregs[10] = 0;

	cpu.NumvArgRegs = 10;
	cpu.vargregs[0] = 1;
	cpu.vargregs[1] = 2;
	cpu.vargregs[2] = 3;
	cpu.vargregs[3] = 40;
	cpu.vargregs[4] = 41;
	cpu.vargregs[5] = 42;
	cpu.vargregs[6] = 43;
	cpu.vargregs[7] = 44;
	cpu.vargregs[8] = 45;
	cpu.vargregs[9] = 46;
	cpu.vargregs[10] = 47;

#endif
#ifdef QUPLS40
	cpu.NumRegs = 16;
	cpu.NumArgRegs = 3;
	cpu.argregs[0] = 1;
	cpu.argregs[1] = 2;
	cpu.argregs[2] = 3;
	cpu.argregs[3] = 4;
	cpu.argregs[4] = 5;
	cpu.argregs[5] = 6;
	cpu.argregs[6] = 7;
	cpu.argregs[7] = 8;
	cpu.argregs[8] = 0;
	cpu.argregs[9] = 0;
	cpu.argregs[10] = 0;

	cpu.NumvArgRegs = 10;
	cpu.vargregs[0] = 1;
	cpu.vargregs[1] = 2;
	cpu.vargregs[2] = 3;
	cpu.vargregs[3] = 40;
	cpu.vargregs[4] = 41;
	cpu.vargregs[5] = 42;
	cpu.vargregs[6] = 43;
	cpu.vargregs[7] = 44;
	cpu.vargregs[8] = 45;
	cpu.vargregs[9] = 46;
	cpu.vargregs[10] = 47;

#endif
#ifdef THOR
	cpu.NumArgRegs = 11;
	cpu.argregs[0] = 1;
	cpu.argregs[1] = 2;
	cpu.argregs[2] = 3;
	cpu.argregs[3] = 40;
	cpu.argregs[4] = 41;
	cpu.argregs[5] = 42;
	cpu.argregs[6] = 43;
	cpu.argregs[7] = 44;
	cpu.argregs[8] = 45;
	cpu.argregs[9] = 46;
	cpu.argregs[10] = 47;

	cpu.NumvArgRegs = 10;
	cpu.vargregs[0] = 1;
	cpu.vargregs[1] = 2;
	cpu.vargregs[2] = 3;
	cpu.vargregs[3] = 40;
	cpu.vargregs[4] = 41;
	cpu.vargregs[5] = 42;
	cpu.vargregs[6] = 43;
	cpu.vargregs[7] = 44;
	cpu.vargregs[8] = 45;
	cpu.vargregs[9] = 46;
	cpu.vargregs[10] = 47;

#endif
#ifdef RISCV
	cpu.NumRegs = 32;
	cpu.NumArgRegs = 8;
	cpu.argregs[0] = 10;
	cpu.argregs[1] = 11;
	cpu.argregs[2] = 12;
	cpu.argregs[3] = 13;
	cpu.argregs[4] = 14;
	cpu.argregs[5] = 15;
	cpu.argregs[6] = 16;
	cpu.argregs[7] = 17;

	cpu.NumFargRegs = 8;
	cpu.fargregs[0] = 10 | rt_float;
	cpu.fargregs[1] = 11 | rt_float;
	cpu.fargregs[2] = 12 | rt_float;
	cpu.fargregs[3] = 13 | rt_float;
	cpu.fargregs[4] = 14 | rt_float;
	cpu.fargregs[5] = 15 | rt_float;
	cpu.fargregs[6] = 16 | rt_float;
	cpu.fargregs[7] = 17 | rt_float;
#endif
#ifdef BIGFOOT
	cpu.NumRegs = 32;
	cpu.NumArgRegs = 8;
	cpu.argregs[0] = 1;
	cpu.argregs[1] = 2;
	cpu.argregs[2] = 3;
	cpu.argregs[3] = 4;
	cpu.argregs[4] = 5;
	cpu.argregs[5] = 6;
	cpu.argregs[6] = 7;
	cpu.argregs[7] = 8;
	cpu.argregs[8] = 0;
	cpu.argregs[9] = 0;
	cpu.argregs[10] = 0;

	cpu.NumvArgRegs = 10;
	cpu.vargregs[0] = 1;
	cpu.vargregs[1] = 2;
	cpu.vargregs[2] = 3;
	cpu.vargregs[3] = 40;
	cpu.vargregs[4] = 41;
	cpu.vargregs[5] = 42;
	cpu.vargregs[6] = 43;
	cpu.vargregs[7] = 44;
	cpu.vargregs[8] = 45;
	cpu.vargregs[9] = 46;
	cpu.vargregs[10] = 47;

#endif
#ifdef I386
	cpu.NumRegs = 32;
	cpu.NumArgRegs = 8;
	cpu.argregs[0] = 1;
	cpu.argregs[1] = 2;
	cpu.argregs[2] = 3;
	cpu.argregs[3] = 4;
	cpu.argregs[4] = 5;
	cpu.argregs[5] = 6;
	cpu.argregs[6] = 7;
	cpu.argregs[7] = 8;
	cpu.argregs[8] = 0;
	cpu.argregs[9] = 0;
	cpu.argregs[10] = 0;

	cpu.NumvArgRegs = 10;
	cpu.vargregs[0] = 1;
	cpu.vargregs[1] = 2;
	cpu.vargregs[2] = 3;
	cpu.vargregs[3] = 40;
	cpu.vargregs[4] = 41;
	cpu.vargregs[5] = 42;
	cpu.vargregs[6] = 43;
	cpu.vargregs[7] = 44;
	cpu.vargregs[8] = 45;
	cpu.vargregs[9] = 46;
	cpu.vargregs[10] = 47;

#endif

#ifdef QUPLS
	cpu.NumTmpRegs = 9;
	cpu.tmpregs[0] = 9;
	cpu.tmpregs[1] = 10;
	cpu.tmpregs[2] = 11;
	cpu.tmpregs[3] = 12;
	cpu.tmpregs[4] = 13;
	cpu.tmpregs[5] = 14;
	cpu.tmpregs[6] = 15;
	cpu.tmpregs[7] = 16;
	cpu.tmpregs[8] = 17;

	// These are saved regs.
	cpu.tmpregs[9] = 18;
	cpu.tmpregs[10] = 19;
	cpu.tmpregs[11] = 20;
	cpu.tmpregs[12] = 21;
	cpu.tmpregs[13] = 22;
	cpu.tmpregs[14] = 23;
	cpu.tmpregs[15] = 24;
	cpu.tmpregs[16] = 25;

	cpu.NumvTmpRegs = 12;
	cpu.vtmpregs[0] = 4;
	cpu.vtmpregs[1] = 5;
	cpu.vtmpregs[2] = 6;
	cpu.vtmpregs[3] = 7;
	cpu.vtmpregs[4] = 8;
	cpu.vtmpregs[5] = 9;
	cpu.vtmpregs[6] = 10;
	cpu.vtmpregs[7] = 11;
	cpu.vtmpregs[8] = 12;
	cpu.vtmpregs[9] = 13;
	cpu.vtmpregs[10] = 14;
	cpu.vtmpregs[11] = 15;

#endif
#ifdef QUPLS40
	cpu.NumTmpRegs = 4;
	cpu.tmpregs[0] = 4;
	cpu.tmpregs[1] = 5;
	cpu.tmpregs[2] = 6;
	cpu.tmpregs[3] = 7;
	// These are saved regs.
	cpu.tmpregs[4] = 8;
	cpu.tmpregs[5] = 9;
	cpu.tmpregs[6] = 10;
	cpu.tmpregs[7] = 11;
	cpu.tmpregs[8] = 12;

	cpu.NumvTmpRegs = 12;
	cpu.vtmpregs[0] = 4;
	cpu.vtmpregs[1] = 5;
	cpu.vtmpregs[2] = 6;
	cpu.vtmpregs[3] = 7;
	cpu.vtmpregs[4] = 8;
	cpu.vtmpregs[5] = 9;
	cpu.vtmpregs[6] = 10;
	cpu.vtmpregs[7] = 11;
	cpu.vtmpregs[8] = 12;
	cpu.vtmpregs[9] = 13;
	cpu.vtmpregs[10] = 14;
	cpu.vtmpregs[11] = 15;

#endif
#ifdef THOR
	cpu.NumTmpRegs = 12;
	cpu.tmpregs[0] = 4;
	cpu.tmpregs[1] = 5;
	cpu.tmpregs[2] = 6;
	cpu.tmpregs[3] = 7;
	cpu.tmpregs[4] = 8;
	cpu.tmpregs[5] = 9;
	cpu.tmpregs[6] = 10;
	cpu.tmpregs[7] = 11;
	cpu.tmpregs[8] = 12;
	cpu.tmpregs[9] = 13;
	cpu.tmpregs[10] = 14;
	cpu.tmpregs[11] = 15;

	cpu.NumvTmpRegs = 12;
	cpu.vtmpregs[0] = 4;
	cpu.vtmpregs[1] = 5;
	cpu.vtmpregs[2] = 6;
	cpu.vtmpregs[3] = 7;
	cpu.vtmpregs[4] = 8;
	cpu.vtmpregs[5] = 9;
	cpu.vtmpregs[6] = 10;
	cpu.vtmpregs[7] = 11;
	cpu.vtmpregs[8] = 12;
	cpu.vtmpregs[9] = 13;
	cpu.vtmpregs[10] = 14;
	cpu.vtmpregs[11] = 15;

#endif
#ifdef RISCV
	cpu.NumTmpRegs = 7;
	cpu.tmpregs[0] = 5;
	cpu.tmpregs[1] = 6;
	cpu.tmpregs[2] = 7;
	cpu.tmpregs[3] = 28;
	cpu.tmpregs[4] = 29;
	cpu.tmpregs[5] = 30;
	cpu.tmpregs[6] = 31;

	cpu.NumFtmpRegs = 12;
	cpu.ftmpregs[0] = 0 | rt_float;
	cpu.ftmpregs[1] = 1 | rt_float;
	cpu.ftmpregs[2] = 2 | rt_float;
	cpu.ftmpregs[3] = 3 | rt_float;
	cpu.ftmpregs[4] = 4 | rt_float;
	cpu.ftmpregs[5] = 5 | rt_float;
	cpu.ftmpregs[6] = 6 | rt_float;
	cpu.ftmpregs[7] = 7 | rt_float;
	cpu.ftmpregs[8] = 28 | rt_float;
	cpu.ftmpregs[9] = 29 | rt_float;
	cpu.ftmpregs[10] = 30 | rt_float;
	cpu.ftmpregs[11] = 31 | rt_float;
#endif
#ifdef BIGFOOT
	cpu.regs[1].isArg = true;
	cpu.regs[1].number = 1;
	cpu.regs[2].isArg = true;
	cpu.regs[2].number = 2;
	cpu.regs[3].isArg = true;
	cpu.regs[3].number = 3;
	cpu.regs[4].isArg = true;
	cpu.regs[4].number = 4;
	cpu.regs[5].isArg = true;
	cpu.regs[5].number = 5;
	cpu.regs[6].isArg = true;
	cpu.regs[6].number = 6;
	cpu.regs[7].isArg = true;
	cpu.regs[7].number = 7;
	cpu.regs[8].isArg = true;
	cpu.regs[8].number = 8;
	cpu.regs[9].isTemp = true;
	cpu.regs[9].isFirstTemp = true;
	cpu.regs[9].number = 9;
	cpu.regs[9].prevreg = 18;
	cpu.regs[9].nextreg = 10;
	cpu.regs[10].isTemp = true;
	cpu.regs[10].number = 10;
	cpu.regs[10].prevreg = 9;
	cpu.regs[10].nextreg = 11;
	cpu.regs[11].isTemp = true;
	cpu.regs[11].number = 11;
	cpu.regs[11].prevreg = 10;
	cpu.regs[11].nextreg = 12;
	cpu.regs[12].isTemp = true;
	cpu.regs[12].number = 12;
	cpu.regs[12].prevreg = 11;
	cpu.regs[12].nextreg = 13;
	cpu.regs[13].isTemp = true;
	cpu.regs[13].number = 13;
	cpu.regs[13].prevreg = 12;
	cpu.regs[13].nextreg = 14;
	cpu.regs[14].isTemp = true;
	cpu.regs[14].number = 14;
	cpu.regs[14].prevreg = 13;
	cpu.regs[14].nextreg = 15;
	cpu.regs[15].isTemp = true;
	cpu.regs[15].number = 15;
	cpu.regs[15].prevreg = 14;
	cpu.regs[15].nextreg = 16;
	cpu.regs[16].isTemp = true;
	cpu.regs[16].number = 16;
	cpu.regs[16].prevreg = 15;
	cpu.regs[16].nextreg = 17;
	cpu.regs[17].isTemp = true;
	cpu.regs[17].number = 17;
	cpu.regs[17].prevreg = 16;
	cpu.regs[17].nextreg = 18;
	cpu.regs[18].isTemp = true;
	cpu.regs[18].isLastTemp = true;
	cpu.regs[18].number = 18;
	cpu.regs[18].prevreg = 17;
	cpu.regs[18].nextreg = 9;
	cpu.regs[19].isSaved = true;
	cpu.regs[19].number = 19;
	cpu.regs[20].isSaved = true;
	cpu.regs[20].number = 20;
	cpu.regs[21].isSaved = true;
	cpu.regs[21].number = 21;
	cpu.regs[22].isSaved = true;
	cpu.regs[22].number = 2;
	cpu.regs[23].isSaved = true;
	cpu.regs[23].number = 23;
	cpu.regs[24].isSaved = true;
	cpu.regs[24].number = 24;
	cpu.regs[25].isSaved = true;
	cpu.regs[25].number = 25;
	cpu.regs[26].isSaved = true;
	cpu.regs[26].number = 26;
	cpu.regs[27].isSaved = true;
	cpu.regs[27].number = 27;
	cpu.regs[32].isCr = true;
	cpu.regs[32].number = 32;
	cpu.regs[33].isCr = true;
	cpu.regs[33].number = 33;
	cpu.regs[34].isCr = true;
	cpu.regs[34].number = 34;
	cpu.regs[34].nextreg = 35;
	cpu.regs[34].prevreg = 37;
	cpu.regs[34].isTemp = true;
	cpu.regs[34].isFirstTemp = true;
	cpu.regs[35].isCr = true;
	cpu.regs[35].number = 35;
	cpu.regs[35].nextreg = 36;
	cpu.regs[35].prevreg = 34;
	cpu.regs[35].isTemp = true;
	cpu.regs[36].isCr = true;
	cpu.regs[36].number = 36;
	cpu.regs[36].nextreg = 37;
	cpu.regs[36].prevreg = 35;
	cpu.regs[36].isTemp = true;
	cpu.regs[37].isCr = true;
	cpu.regs[37].number = 37;
	cpu.regs[37].nextreg = 34;
	cpu.regs[37].prevreg = 36;
	cpu.regs[37].isTemp = true;
	cpu.regs[37].isLastTemp = true;
	cpu.regs[38].isCr = true;
	cpu.regs[38].number = 38;
	cpu.regs[39].isCr = true;
	cpu.regs[39].number = 39;
	cpu.regs[40].isCrg = true;
	cpu.regs[40].number = 40;

	cpu.NumTmpRegs = 9;
	cpu.tmpregs[0] = 9;
	cpu.tmpregs[1] = 10;
	cpu.tmpregs[2] = 11;
	cpu.tmpregs[3] = 12;
	cpu.tmpregs[4] = 13;
	cpu.tmpregs[5] = 14;
	cpu.tmpregs[6] = 15;
	cpu.tmpregs[7] = 16;
	cpu.tmpregs[8] = 17;
	cpu.availableTemps.add(9);
	cpu.availableTemps.add(10);
	cpu.availableTemps.add(11);
	cpu.availableTemps.add(12);
	cpu.availableTemps.add(13);
	cpu.availableTemps.add(14);
	cpu.availableTemps.add(15);
	cpu.availableTemps.add(16);
	cpu.availableTemps.add(17);
	cpu.availableTemps.add(18);

	cpu.NumTmpCrRegs = 4;
	cpu.tmpCrRegs[0].number = 2;
	cpu.tmpCrRegs[1].number = 3;
	cpu.tmpCrRegs[2].number = 4;
	cpu.tmpCrRegs[3].number = 5;
	/*
	cpu.tmpCrRegs[4].number = 0;
	cpu.tmpCrRegs[5].number = 0;
	cpu.tmpCrRegs[6].number = 0;
	cpu.tmpCrRegs[7].number = 0;
	cpu.tmpCrRegs[8].number = 0;
	*/
	// These are saved regs.
	cpu.tmpregs[9] = 18;
	cpu.tmpregs[10] = 19;
	cpu.tmpregs[11] = 20;
	cpu.tmpregs[12] = 21;
	cpu.tmpregs[13] = 22;
	cpu.tmpregs[14] = 23;
	cpu.tmpregs[15] = 24;
	cpu.tmpregs[16] = 25;

	cpu.NumvTmpRegs = 12;
	cpu.vtmpregs[0] = 4;
	cpu.vtmpregs[1] = 5;
	cpu.vtmpregs[2] = 6;
	cpu.vtmpregs[3] = 7;
	cpu.vtmpregs[4] = 8;
	cpu.vtmpregs[5] = 9;
	cpu.vtmpregs[6] = 10;
	cpu.vtmpregs[7] = 11;
	cpu.vtmpregs[8] = 12;
	cpu.vtmpregs[9] = 13;
	cpu.vtmpregs[10] = 14;
	cpu.vtmpregs[11] = 15;

#endif
#ifdef I386
	cpu.NumTmpRegs = 9;
	cpu.tmpregs[0] = 9;
	cpu.tmpregs[1] = 10;
	cpu.tmpregs[2] = 11;
	cpu.tmpregs[3] = 12;
	cpu.tmpregs[4] = 13;
	cpu.tmpregs[5] = 14;
	cpu.tmpregs[6] = 15;
	cpu.tmpregs[7] = 16;
	cpu.tmpregs[8] = 17;

	cpu.NumTmpCrRegs = 3;
	cpu.tmpCrRegs[0] = 2;
	cpu.tmpCrRegs[1] = 3;
	cpu.tmpCrRegs[2] = 4;
	cpu.tmpCrRegs[3] = 0;
	cpu.tmpCrRegs[4] = 0;
	cpu.tmpCrRegs[5] = 0;
	cpu.tmpCrRegs[6] = 0;
	cpu.tmpCrRegs[7] = 0;
	cpu.tmpCrRegs[8] = 0;

	// These are saved regs.
	cpu.tmpregs[9] = 18;
	cpu.tmpregs[10] = 19;
	cpu.tmpregs[11] = 20;
	cpu.tmpregs[12] = 21;
	cpu.tmpregs[13] = 22;
	cpu.tmpregs[14] = 23;
	cpu.tmpregs[15] = 24;
	cpu.tmpregs[16] = 25;

	cpu.NumvTmpRegs = 12;
	cpu.vtmpregs[0] = 4;
	cpu.vtmpregs[1] = 5;
	cpu.vtmpregs[2] = 6;
	cpu.vtmpregs[3] = 7;
	cpu.vtmpregs[4] = 8;
	cpu.vtmpregs[5] = 9;
	cpu.vtmpregs[6] = 10;
	cpu.vtmpregs[7] = 11;
	cpu.vtmpregs[8] = 12;
	cpu.vtmpregs[9] = 13;
	cpu.vtmpregs[10] = 14;
	cpu.vtmpregs[11] = 15;

#endif

#ifdef QUPLS
	cpu.NumSavedRegs = 8;
	cpu.saved_regs[0] = 18;
	cpu.saved_regs[1] = 19;
	cpu.saved_regs[2] = 20;
	cpu.saved_regs[3] = 21;
	cpu.saved_regs[4] = 22;
	cpu.saved_regs[5] = 23;
	cpu.saved_regs[6] = 24;
	cpu.saved_regs[7] = 25;
	cpu.saved_regs[8] = 26;
	cpu.saved_regs[9] = 0;
	cpu.saved_regs[10] = 0;
	cpu.saved_regs[11] = 0;
	cpu.saved_regs[12] = 0;
	cpu.saved_regs[13] = 0;
	cpu.saved_regs[14] = 0;
	cpu.saved_regs[15] = 0;

	cpu.NumvSavedRegs = 16;
	cpu.vsaved_regs[0] = 16;
	cpu.vsaved_regs[1] = 17;
	cpu.vsaved_regs[2] = 18;
	cpu.vsaved_regs[3] = 19;
	cpu.vsaved_regs[4] = 20;
	cpu.vsaved_regs[5] = 21;
	cpu.vsaved_regs[6] = 22;
	cpu.vsaved_regs[7] = 23;
	cpu.vsaved_regs[8] = 24;
	cpu.vsaved_regs[9] = 25;
	cpu.vsaved_regs[10] = 26;
	cpu.vsaved_regs[11] = 27;
	cpu.vsaved_regs[12] = 28;
	cpu.vsaved_regs[13] = 29;
	cpu.vsaved_regs[14] = 30;
	cpu.vsaved_regs[15] = 31;

#endif
#ifdef QUPLS40
	cpu.NumSavedRegs = 5;
	cpu.saved_regs[0] = 8;
	cpu.saved_regs[1] = 9;
	cpu.saved_regs[2] = 10;
	cpu.saved_regs[3] = 11;
	cpu.saved_regs[4] = 12;

	cpu.NumvSavedRegs = 16;
	cpu.vsaved_regs[0] = 16;
	cpu.vsaved_regs[1] = 17;
	cpu.vsaved_regs[2] = 18;
	cpu.vsaved_regs[3] = 19;
	cpu.vsaved_regs[4] = 20;
	cpu.vsaved_regs[5] = 21;
	cpu.vsaved_regs[6] = 22;
	cpu.vsaved_regs[7] = 23;
	cpu.vsaved_regs[8] = 24;
	cpu.vsaved_regs[9] = 25;
	cpu.vsaved_regs[10] = 26;
	cpu.vsaved_regs[11] = 27;
	cpu.vsaved_regs[12] = 28;
	cpu.vsaved_regs[13] = 29;
	cpu.vsaved_regs[14] = 30;
	cpu.vsaved_regs[15] = 31;

#endif
#ifdef THOR
	cpu.NumSavedRegs = 16;
	cpu.saved_regs[0] = 16;
	cpu.saved_regs[1] = 17;
	cpu.saved_regs[2] = 18;
	cpu.saved_regs[3] = 19;
	cpu.saved_regs[4] = 20;
	cpu.saved_regs[5] = 21;
	cpu.saved_regs[6] = 22;
	cpu.saved_regs[7] = 23;
	cpu.saved_regs[8] = 24;
	cpu.saved_regs[9] = 25;
	cpu.saved_regs[10] = 26;
	cpu.saved_regs[11] = 27;
	cpu.saved_regs[12] = 28;
	cpu.saved_regs[13] = 29;
	cpu.saved_regs[14] = 30;
	cpu.saved_regs[15] = 31;

	cpu.NumvSavedRegs = 16;
	cpu.vsaved_regs[0] = 16;
	cpu.vsaved_regs[1] = 17;
	cpu.vsaved_regs[2] = 18;
	cpu.vsaved_regs[3] = 19;
	cpu.vsaved_regs[4] = 20;
	cpu.vsaved_regs[5] = 21;
	cpu.vsaved_regs[6] = 22;
	cpu.vsaved_regs[7] = 23;
	cpu.vsaved_regs[8] = 24;
	cpu.vsaved_regs[9] = 25;
	cpu.vsaved_regs[10] = 26;
	cpu.vsaved_regs[11] = 27;
	cpu.vsaved_regs[12] = 28;
	cpu.vsaved_regs[13] = 29;
	cpu.vsaved_regs[14] = 30;
	cpu.vsaved_regs[15] = 31;

#endif
#ifdef RISCV
	cpu.NumSavedRegs = 10;
	cpu.saved_regs[0] = 9;
	cpu.saved_regs[1] = 18;
	cpu.saved_regs[2] = 19;
	cpu.saved_regs[3] = 20;
	cpu.saved_regs[4] = 21;
	cpu.saved_regs[5] = 22;
	cpu.saved_regs[6] = 23;
	cpu.saved_regs[7] = 24;
	cpu.saved_regs[8] = 25;
	cpu.saved_regs[9] = 26;
//	cpu.saved_regs[10] = 27; used for GP1
	cpu.NumFsavedRegs = 12;
	cpu.fsaved_regs[0] = 8 | rt_float;
	cpu.fsaved_regs[1] = 9 | rt_float;
	cpu.fsaved_regs[2] = 18 | rt_float;
	cpu.fsaved_regs[3] = 19 | rt_float;
	cpu.fsaved_regs[4] = 20 | rt_float;
	cpu.fsaved_regs[5] = 21 | rt_float;
	cpu.fsaved_regs[6] = 22 | rt_float;
	cpu.fsaved_regs[7] = 23 | rt_float;
	cpu.fsaved_regs[8] = 24 | rt_float;
	cpu.fsaved_regs[9] = 25 | rt_float;
	cpu.fsaved_regs[10] = 26 | rt_float;
	cpu.fsaved_regs[11] = 27 | rt_float;
#endif
#ifdef BIGFOOT
	cpu.NumSavedRegs = 8;
	cpu.saved_regs[0] = 18;
	cpu.saved_regs[1] = 19;
	cpu.saved_regs[2] = 20;
	cpu.saved_regs[3] = 21;
	cpu.saved_regs[4] = 22;
	cpu.saved_regs[5] = 23;
	cpu.saved_regs[6] = 24;
	cpu.saved_regs[7] = 25;
	cpu.saved_regs[8] = 26;
	cpu.saved_regs[9] = 0;
	cpu.saved_regs[10] = 0;
	cpu.saved_regs[11] = 0;
	cpu.saved_regs[12] = 0;
	cpu.saved_regs[13] = 0;
	cpu.saved_regs[14] = 0;
	cpu.saved_regs[15] = 0;

	cpu.NumvSavedRegs = 16;
	cpu.vsaved_regs[0] = 16;
	cpu.vsaved_regs[1] = 17;
	cpu.vsaved_regs[2] = 18;
	cpu.vsaved_regs[3] = 19;
	cpu.vsaved_regs[4] = 20;
	cpu.vsaved_regs[5] = 21;
	cpu.vsaved_regs[6] = 22;
	cpu.vsaved_regs[7] = 23;
	cpu.vsaved_regs[8] = 24;
	cpu.vsaved_regs[9] = 25;
	cpu.vsaved_regs[10] = 26;
	cpu.vsaved_regs[11] = 27;
	cpu.vsaved_regs[12] = 28;
	cpu.vsaved_regs[13] = 29;
	cpu.vsaved_regs[14] = 30;
	cpu.vsaved_regs[15] = 31;

	cpu.NumSavedCrRegs = 3;
	cpu.savedCrRegs[0] = 5;
	cpu.savedCrRegs[1] = 6;
	cpu.savedCrRegs[2] = 7;
	cpu.savedCrRegs[3] = 0;
	cpu.savedCrRegs[4] = 0;
	cpu.savedCrRegs[5] = 0;
	cpu.savedCrRegs[6] = 0;
	cpu.savedCrRegs[7] = 0;
	cpu.savedCrRegs[8] = 0;
	cpu.savedCrRegs[9] = 0;
	cpu.savedCrRegs[10] = 0;
	cpu.savedCrRegs[11] = 0;
	cpu.savedCrRegs[12] = 0;
	cpu.savedCrRegs[13] = 0;
	cpu.savedCrRegs[14] = 0;
	cpu.savedCrRegs[15] = 0;
#endif
#ifdef I386
	cpu.NumSavedRegs = 8;
	cpu.saved_regs[0] = 18;
	cpu.saved_regs[1] = 19;
	cpu.saved_regs[2] = 20;
	cpu.saved_regs[3] = 21;
	cpu.saved_regs[4] = 22;
	cpu.saved_regs[5] = 23;
	cpu.saved_regs[6] = 24;
	cpu.saved_regs[7] = 25;
	cpu.saved_regs[8] = 26;
	cpu.saved_regs[9] = 0;
	cpu.saved_regs[10] = 0;
	cpu.saved_regs[11] = 0;
	cpu.saved_regs[12] = 0;
	cpu.saved_regs[13] = 0;
	cpu.saved_regs[14] = 0;
	cpu.saved_regs[15] = 0;

	cpu.NumvSavedRegs = 16;
	cpu.vsaved_regs[0] = 16;
	cpu.vsaved_regs[1] = 17;
	cpu.vsaved_regs[2] = 18;
	cpu.vsaved_regs[3] = 19;
	cpu.vsaved_regs[4] = 20;
	cpu.vsaved_regs[5] = 21;
	cpu.vsaved_regs[6] = 22;
	cpu.vsaved_regs[7] = 23;
	cpu.vsaved_regs[8] = 24;
	cpu.vsaved_regs[9] = 25;
	cpu.vsaved_regs[10] = 26;
	cpu.vsaved_regs[11] = 27;
	cpu.vsaved_regs[12] = 28;
	cpu.vsaved_regs[13] = 29;
	cpu.vsaved_regs[14] = 30;
	cpu.vsaved_regs[15] = 31;

	cpu.NumSavedCrRegs = 3;
	cpu.savedCrRegs[0] = 5;
	cpu.savedCrRegs[1] = 6;
	cpu.savedCrRegs[2] = 7;
	cpu.savedCrRegs[3] = 0;
	cpu.savedCrRegs[4] = 0;
	cpu.savedCrRegs[5] = 0;
	cpu.savedCrRegs[6] = 0;
	cpu.savedCrRegs[7] = 0;
	cpu.savedCrRegs[8] = 0;
	cpu.savedCrRegs[9] = 0;
	cpu.savedCrRegs[10] = 0;
	cpu.savedCrRegs[11] = 0;
	cpu.savedCrRegs[12] = 0;
	cpu.savedCrRegs[13] = 0;
	cpu.savedCrRegs[14] = 0;
	cpu.savedCrRegs[15] = 0;
#endif
}

void initRegStack()
{
	int i;
	Function *sym = currentFn;

	stacked_regs_set = new CSet();
	stacked_regs_set->clear();
	next_reg = 9;
	next_fpreg = 0;// regFirstTemp;
	next_preg = 0;// regFirstTemp;
	next_vreg = 0;// regFirstTemp;
	next_vmreg = 0;
  next_breg = 0;
	nextCrReg = 34;
	//for (rsp=0; rsp < 3; rsp=rsp+1)
	//	regstack[rsp] = tmpregs[rsp];
	//rsp = 0;
	for (i = 0; i < VR_COUNT; i++) {
		compiler.reg_in_use[i] = -1;
		fpreg_in_use[i] = -1;
		preg_in_use[i] = -1;
		vreg_in_use[i] = -1;
		vmreg_in_use[i] = -1;
		breg_in_use[i&15] = -1;
		CrRegInUse[i & 15] = -1;
	}
    reg_stack_ptr = 0;
    reg_alloc_ptr = 0;
    fpreg_stack_ptr = 0;
    fpreg_alloc_ptr = 0;
		preg_stack_ptr = 0;
		preg_alloc_ptr = 0;
		vreg_stack_ptr = 0;
    vreg_alloc_ptr = 0;
    vmreg_stack_ptr = 0;
    vmreg_alloc_ptr = 0;
    breg_stack_ptr = 0;
    breg_alloc_ptr = 0;
		CrRegStackPtr = 0;
		CrRegAllocPtr = 0;
		//    act_scratch = 0;
    memset(reg_stack,0,sizeof(reg_stack));
    memset(reg_alloc,0,sizeof(reg_alloc));
    memset(fpreg_stack,0,sizeof(fpreg_stack));
    memset(fpreg_alloc,0,sizeof(fpreg_alloc));
		memset(preg_stack, 0, sizeof(preg_stack));
		memset(preg_alloc, 0, sizeof(preg_alloc));
		memset(vreg_stack,0,sizeof(vreg_stack));
    memset(vreg_alloc,0,sizeof(vreg_alloc));
    memset(vmreg_stack,0,sizeof(vmreg_stack));
    memset(vmreg_alloc,0,sizeof(vmreg_alloc));
    memset(breg_stack,0,sizeof(breg_stack));
    memset(breg_alloc,0,sizeof(breg_alloc));
    memset(stacked_regs,0,sizeof(stacked_regs));
    memset(stacked_fpregs,0,sizeof(stacked_fpregs));
    memset(save_reg_alloc,0,sizeof(save_reg_alloc));
    memset(save_fpreg_alloc,0,sizeof(save_fpreg_alloc));
    memset(stacked_vregs,0,sizeof(stacked_vregs));
    memset(save_vreg_alloc,0,sizeof(save_vreg_alloc));
    memset(save_vmreg_alloc,0,sizeof(save_vmreg_alloc));
		memset(CrRegStack, 0, sizeof(CrRegStack));
		memset(CrRegAlloc, 0, sizeof(CrRegAlloc));
		wrapno = 0;
		wrapno_1 = 0;
		CrWrapno = 0;
		CrWrapno_1 = 0;
	ZeroMemory(rap, sizeof(rap));
	for (i = 0; i < 64; i++) {
		cpu.tmpCrRegs[i].inUse.clear();
		cpu.tmpCrRegs[i].isPushed.clear();
	}

}

int IsTempReg(int rg)
{
	int nn;

	return ((cpu.regs[rg].isTemp && !cpu.regs[rg].isCr) ? rg : 0);

	for (nn = 0; nn < cpu.NumTmpRegs; nn++) {
		if (rg == cpu.tmpregs[nn])// || rg==cpu.vtmpregs[nn])
			return (nn+1);
	}
	return (0);
}

int IsTempCrReg(int rg)
{
	int nn;

	return (cpu.regs[rg].isCr && cpu.regs[rg].isTemp);
	for (nn = 0; nn < cpu.NumTmpCrRegs; nn++) {
		if (rg == cpu.tmpCrRegs[nn].number)// || rg==cpu.vtmpregs[nn])
			return (nn + 1);
	}
	return (0);
}

int IsFtmpReg(int rg)
{
	int nn;

	for (nn = 0; nn < cpu.NumFtmpRegs; nn++) {
		if (rg == cpu.ftmpregs[nn])// || rg==cpu.vtmpregs[nn])
			return (nn + 1);
	}
	return (0);
}

int IsArgReg(int rg)
{
	int nn;

	return (cpu.regs[rg].isArg);
	for (nn = 0; nn < cpu.NumArgRegs; nn++) {
		if (rg == cpu.argregs[nn])// || rg == cpu.vargregs[nn])
			return (nn + 1);
	}
	return (0);
}

int IsFargReg(int rg)
{
	int nn;

	for (nn = 0; nn < cpu.NumFargRegs; nn++) {
		if (rg == cpu.fargregs[nn])// || rg == cpu.vargregs[nn])
			return (nn + 1);
	}
	return (0);
}

int IsSavedReg(int rg)
{
	int nn;

	return (cpu.regs[rg].isSaved && !cpu.regs[rg].isCr);
	for (nn = 0; nn < cpu.NumSavedRegs; nn++) {
		if (rg == cpu.saved_regs[nn])// || rg == cpu.vsaved_regs[nn])
			return (nn + 1);
	}
	return (0);
}

int IsSavedCrReg(int rg)
{
	int nn;

	return (cpu.regs[rg].isSaved && cpu.regs[rg].isCr);
	if (((rg >> 30) & 3) != 3)
		return (0);
	rg &= 0x3fffffffL;
	for (nn = 0; nn < cpu.NumSavedCrRegs; nn++) {
		if (rg == cpu.savedCrRegs[nn])// || rg == cpu.vsaved_regs[nn])
			return (nn + 1);
	}
	return (0);
}

int IsFsavedReg(int rg)
{
	int nn;

	for (nn = 0; nn < cpu.NumFsavedRegs; nn++) {
		if (rg == cpu.fsaved_regs[nn])// || rg == cpu.vsaved_regs[nn])
			return (nn + 1);
	}
	return (0);
}

// Spill a register to memory.

void SpillRegister(Operand *ap, int number)
{
	if (pass == 1)
		max_stack_use = max(max_stack_use, (ap->deep + 1) * cpu.sizeOfWord);
#if 0
	cg.GenerateStore(ap,cg.MakeIndexed(currentFn->GetTempBot()+ap->deep*cpu.sizeOfWord,regFP), cpu.sizeOfWord);
  //reg_stack[reg_stack_ptr].Operand = ap;
  //reg_stack[reg_stack_ptr].f.allocnum = number;
  if (reg_alloc[number].f.isPushed=='T')
	fatal("SpillRegister(): register already spilled");
  reg_alloc[number].f.isPushed = 'T';
	compiler.reg_in_use[ap->preg&VR_MASK] = -1;
#endif

	GenerateMonadic(op_push, 0, ap);
	if (cpu.regs[ap->preg & VR_MASK].isPushed.isMember(wrapno))
		fatal("SpillRegister(): cr register already spilled");
	cpu.regs[ap->preg & VR_MASK].inUse.remove(wrapno);
	cpu.regs[ap->preg & VR_MASK].isPushed.add(wrapno);
}

void SpillCrRegister(Operand* ap, int number)
{
	Operand* ap1;
	int ndx;

	GenerateMonadic(op_push, 0, ap);
	if (pass == 1)
		max_stack_use = max(max_stack_use, (ap->deep + 1) * cpu.sizeOfWord);
	if (cpu.regs[ap->preg & VR_MASK].isPushed.isMember(CrWrapno))
		fatal("SpillCrRegister(): cr register already spilled");
	cpu.regs[ap->preg & VR_MASK].inUse.remove(CrWrapno);
	cpu.regs[ap->preg & VR_MASK].isPushed.add(CrWrapno);
}

void SpillVectorRegister(Operand* ap, int number)
{
	GenerateDiadic(op_store, 0, ap, cg.MakeIndexed(currentFn->GetTempBot() + ap->deep * (cpu.sizeOfWord * 4), regFP));
	if (pass == 1)
		max_stack_use = max(max_stack_use, (ap->deep + 1) * (cpu.sizeOfWord * 4));
	//reg_stack[reg_stack_ptr].Operand = ap;
	//reg_stack[reg_stack_ptr].f.allocnum = number;
	if (vreg_alloc[number].f.isPushed == 'T')
		fatal("SpillVectorRegister(): register already spilled");
	vreg_alloc[number].f.isPushed = 'T';
	vreg_in_use[ap->preg&VR_MASK] = -1;
}

void SpillFPRegister(Operand *ap, int number)
{
	GenerateDiadic(op_store,0,ap,cg.MakeIndexed(currentFn->GetTempBot()-ap->deep*cpu.sizeOfWord,regFP));
	if (pass==1)
		max_stack_use = max(max_stack_use, (ap->deep+1) * cpu.sizeOfWord);
	fpreg_stack[fpreg_stack_ptr].Operand = ap; 
	fpreg_stack[fpreg_stack_ptr].f.allocnum = number;
   if (fpreg_alloc[number].f.isPushed=='T')
		fatal("SpillRegister(): register already spilled");
  fpreg_alloc[number].f.isPushed = 'T';
}

void SpillPositRegister(Operand* ap, int number)
{
	GenerateDiadic(op_store, 0, ap, cg.MakeIndexed(currentFn->GetTempBot() + ap->deep * cpu.sizeOfWord, regFP));
	if (pass == 1)
		max_stack_use = max(max_stack_use, (ap->deep + 1) * cpu.sizeOfWord);
	preg_stack[preg_stack_ptr].Operand = ap;
	preg_stack[preg_stack_ptr].f.allocnum = number;
	if (preg_alloc[number].f.isPushed == 'T')
		fatal("SpillRegister(): register already spilled");
	reg_alloc[number].f.isPushed = 'T';
}

// Load register from memory.

void LoadRegister(int regno, int number)
{
#if 0
	if (compiler.reg_in_use[regno] >= 0)
		fatal("LoadRegister():register still in use");
	compiler.reg_in_use[regno&VR_MASK] = number;
	cg.GenerateLoad(makereg(regno&VR_MASK),cg.MakeIndexed(currentFn->GetTempBot()+number*cpu.sizeOfWord,regFP), cpu.sizeOfWord, cpu.sizeOfWord);
    reg_alloc[number].f.isPushed = 'F';
#endif
	if (cpu.regs[regno & VR_MASK].inUse.isMember(CrWrapno))
		fatal("LoadRegister():register still in use");
	GenerateMonadic(op_pop, 0, makereg(regno));
	cpu.regs[regno & VR_MASK].isPushed.remove(wrapno);
	cpu.regs[regno & VR_MASK].inUse.add(wrapno);
}

static void LoadCrRegister(int regno)
{
	if (cpu.regs[regno & VR_MASK].inUse.isMember(CrWrapno))
		fatal("LoadCrRegister():register still in use");
	GenerateMonadic(op_pop, 0, makeCrReg(regno));
	cpu.regs[regno & VR_MASK].isPushed.remove(CrWrapno);
	cpu.regs[regno & VR_MASK].inUse.add(CrWrapno);
}

// Load vector register from memory.

void LoadVectorRegister(int regno, int number)
{
	if (vreg_in_use[regno&VR_MASK] >= 0)
		fatal("LoadVectorRegister():register still in use");
	vreg_in_use[regno&VR_MASK] = number;
	GenerateDiadic(op_load, 0, makevreg(regno), cg.MakeIndexed(currentFn->GetTempBot() + number * (cpu.sizeOfWord * 4), regFP));
	vreg_alloc[number].f.isPushed = 'F';
}

void LoadFPRegister(int regno, int number)
{
	if (fpreg_in_use[regno & 0x3f] >= 0)
		fatal("LoadRegister():register still in use");
	fpreg_in_use[regno & 0x3f] = number;
	GenerateDiadic(op_fld,0,makefpreg(regno),cg.MakeIndexed(currentFn->GetTempBot()-number*cpu.sizeOfWord,regFP));
    fpreg_alloc[number].f.isPushed = 'F';
}

void LoadPositRegister(int regno, int number)
{
	if (preg_in_use[regno] >= 0)
		fatal("LoadRegister():register still in use");
	preg_in_use[regno] = number;
	GenerateDiadic(op_pldo, 0, makefpreg(regno), cg.MakeIndexed(currentFn->GetTempBot() - number * cpu.sizeOfWord, regFP));
	preg_alloc[number].f.isPushed = 'F';
}

void GenerateTempRegPush(int reg, int rmode, int number, int stkpos)
{
	Operand *ap1;
    ap1 = allocOperand();
    ap1->preg = reg;
    ap1->mode = rmode;

	GenerateMonadic(op_push,0,ap1);
	TRACE(printf("pushing r%d\r\n", reg);)
    reg_stack[reg_stack_ptr].Operand = ap1;
    reg_stack[reg_stack_ptr].reg = reg;
    reg_stack[reg_stack_ptr].f.allocnum = number;
    if (reg_alloc[number].f.isPushed=='T')
		fatal("GenerateTempRegPush(): register already pushed");
    reg_alloc[number].f.isPushed = 'T';
	if (++reg_stack_ptr > MAX_REG_STACK)
		fatal("GenerateTempRegPush(): register stack overflow");
}

void GenerateTempVectorRegPush(int reg, int rmode, int number, int stkpos)
{
	Operand *ap1;
    ap1 = allocOperand();
    ap1->preg = reg;
    ap1->mode = rmode;

	GenerateMonadic(op_push,0,ap1);
	TRACE(printf("pushing r%d\r\n", reg);)
    vreg_stack[vreg_stack_ptr].Operand = ap1;
    vreg_stack[vreg_stack_ptr].reg = reg;
    vreg_stack[vreg_stack_ptr].f.allocnum = number;
    if (vreg_alloc[number].f.isPushed=='T')
		fatal("GenerateTempRegPush(): register already pushed");
    vreg_alloc[number].f.isPushed = 'T';
	if (++vreg_stack_ptr > MAX_REG_STACK)
		fatal("GenerateTempRegPush(): register stack overflow");
}

void GenerateTempRegPop(int reg, int rmode, int number, int stkpos)
{
	Operand *ap1;
 
    if (reg_stack_ptr-- == -1)
		fatal("GenerateTempRegPop(): register stack underflow");
    /* check if the desired register really is on stack */
    if (reg_stack[reg_stack_ptr].f.allocnum != number)
		fatal("GenerateTempRegPop()/2");
	if (compiler.reg_in_use[reg] >= 0)
		fatal("GenerateTempRegPop():register still in use");
	TRACE(printf("popped r%d\r\n", reg);)
	compiler.reg_in_use[reg] = number;
	ap1 = allocOperand();
	ap1->preg = reg;
	ap1->mode = rmode;
	GenerateMonadic(op_pop,0,ap1);
    reg_alloc[number].f.isPushed = 'F';
}

void initstack()
{
	ExpressionHasReference = false;
	compiler.expr_depth = 0;
	initRegStack();
	//initFPRegStack();
}

#if 0
Operand *GetTempRegister()
{
	Operand *ap;
  Function *sym = currentFn;
	int number;

	number = compiler.reg_in_use[cpu.tmpregs[next_reg]];
	if (number >= 0) {// && number < rap[wrapno]) {
		/*
		nr = next_reg;
		for (nn = regFirstTemp; nn <= regLastTemp; nn++) {
			if (compiler.reg_in_use[nn] < 0) {
				compiler.reg_in_use[nn] = reg_alloc_ptr;
				ap = allocOperand();
				ap->mode = am_reg;
				ap->preg = next_reg;
				ap->pdeep = ap->deep;
				ap->deep = reg_alloc_ptr;
				return (ap);
			}
		}
		*/
		SpillRegister(makereg(cpu.tmpregs[next_reg]),number);
	}
	TRACE(printf("GetTempRegister:r%d\r\n", next_reg);)
  compiler.reg_in_use[cpu.tmpregs[next_reg]] = reg_alloc_ptr;
  ap = allocOperand();
  ap->mode = am_reg;
  ap->preg = cpu.tmpregs[next_reg];
	ap->pdeep = ap->deep;
  ap->deep = reg_alloc_ptr;
	if (IsTempReg(ap->preg&VR_MASK))
		compiler.temp_in_use.add(ap->preg&VR_MASK);
	else if (IsSavedReg(ap->preg&VR_MASK))
		compiler.saved_in_use.add(ap->preg&VR_MASK);
	reg_alloc[reg_alloc_ptr].reg = cpu.tmpregs[next_reg];
  reg_alloc[reg_alloc_ptr].Operand = ap;
  reg_alloc[reg_alloc_ptr].f.isPushed = 'F';
	next_reg++;
	if (next_reg >= NumTempRegs() + NumSavedRegs()) {// regLastTemp) {
		wrapno++;
		rap[wrapno] = reg_alloc_ptr;
		next_reg = 0;// regFirstTemp;		/* wrap around */
	}
  if (reg_alloc_ptr++ == MAX_REG_STACK)
		fatal("GetTempRegister(): register stack overflow");
	max_reg_alloc_ptr = max(max_reg_alloc_ptr, reg_alloc_ptr);
	return (ap);
}
#endif

Operand* GetTempRegister()
{
	Operand* ap;
	Function* sym = currentFn;

	TRACE(printf("GetTempCrRegister:r%d\r\n", next_reg);)
		if (wrapno >= MAX_REG_STACK)
			fatal("GetTempRegister(): register stack overflow");
	if (cpu.regs[next_reg].inUse.isMember(wrapno))
		SpillRegister(makereg(next_reg), 0);
	if (cpu.regs[next_reg].isLastTemp) {// regLastTemp) {
		wrapno++;
		wrapno_1 = 0;
	}
	ap = makereg(next_reg);
	ap->deep = wrapno * cpu.NumTmpRegs + wrapno_1;
	if (IsTempReg(ap->preg & VR_MASK))
		compiler.temp_in_use.add(ap->preg & VR_MASK);
	else if (IsSavedReg(ap->preg & VR_MASK))
		compiler.saved_in_use.add(ap->preg & VR_MASK);
	cpu.regs[next_reg].inUse.add(wrapno);
	cpu.regs[next_reg].isPushed.remove(wrapno);
	next_reg = cpu.regs[next_reg].nextreg;
	wrapno_1++;
	return (ap);
}


Operand* GetTempCrRegister()
{
	return (Register::GetTempCrRegister());
}

Operand* Register::GetTempCrRegister()
{
	Operand* ap;
	Function* sym = currentFn;

	TRACE(printf("GetTempCrRegister:r%d\r\n", nextCrReg);)
	if (CrWrapno >= MAX_REG_STACK)
		fatal("GetTempRegister(): register stack overflow");
	if (cpu.regs[nextCrReg].inUse.isMember(CrWrapno))
		SpillCrRegister(makeCrReg(nextCrReg), 0);
	if (cpu.regs[nextCrReg].isLastTemp) {// regLastTemp) {
		CrWrapno++;
		CrWrapno_1 = 0;
	}
	ap = makeCrReg(nextCrReg);
	ap->deep = CrWrapno * cpu.NumTmpCrRegs + CrWrapno_1;
	compiler.tempCrInUse.add(ap->preg & VR_MASK);
	if (IsSavedCrReg(ap->preg & VR_MASK))
		compiler.savedCrInUse.add(ap->preg & VR_MASK);
	cpu.regs[nextCrReg].inUse.add(CrWrapno);
	cpu.regs[nextCrReg].isPushed.remove(CrWrapno);
	cpu.regs[40].inUse.add(CrWrapno);
	nextCrReg = cpu.regs[nextCrReg].nextreg;
	CrWrapno_1++;
	return (ap);
}

Operand *GetTempVectorRegister()
{
	Operand *ap;
  Function *sym = currentFn;
	int number;

	number = vreg_in_use[cpu.vtmpregs[next_vreg]];
	if (vreg_in_use[next_vreg] >= 0) {
//		if (isThor)	
//			GenerateTriadic(op_addui,0,makereg(regSP),makereg(regSP),MakeImmediate(-8));
		SpillVectorRegister(makereg(cpu.vtmpregs[next_vreg]), number);
	}
	TRACE(printf("GetTempVectorRegister:r%d\r\n", next_vreg);)
  vreg_in_use[next_vreg] = vreg_alloc_ptr;
  ap = allocOperand();
  ap->mode = am_vreg;
  ap->preg = next_vreg | rt_vector;
  ap->deep = vreg_alloc_ptr;
//	ap->typep = &stdvector;
  vreg_alloc[vreg_alloc_ptr].reg = next_vreg;
  vreg_alloc[vreg_alloc_ptr].Operand = ap;
  vreg_alloc[vreg_alloc_ptr].f.isPushed = 'F';
	next_vreg++;
  if (next_vreg >= cpu.NumvTmpRegs)
	next_vreg = 0;		/* wrap around */
  if (vreg_alloc_ptr++ == MAX_REG_STACK)
	fatal("GetTempVectorRegister(): register stack overflow");
	max_vreg_alloc_ptr = max(max_vreg_alloc_ptr, vreg_alloc_ptr);
	return (ap);
}

Operand *GetTempVectorMaskRegister()
{
	Operand *ap;
  Symbol *sym = currentFn->sym;

	if (vmreg_in_use[next_vmreg] >= 0) {
//		GenerateTempVectorMaskRegPush(next_vreg, am_reg, vreg_in_use[next_vreg],0);
	}
	TRACE(printf("GetTempRegister:r%d\r\n", next_vmreg);)
    vmreg_in_use[next_vreg] = vmreg_alloc_ptr;
    ap = allocOperand();
    ap->mode = am_vmreg;
    ap->preg = next_vmreg;
    ap->deep = vmreg_alloc_ptr;
		ap->typep = &stdvectormask;
    vmreg_alloc[vmreg_alloc_ptr].reg = next_vmreg;
    vmreg_alloc[vmreg_alloc_ptr].Operand = ap;
    vmreg_alloc[vmreg_alloc_ptr].f.isPushed = 'F';
    if (next_vmreg++ >= 3)
		next_vmreg = 1;		/* wrap around */
    if (vmreg_alloc_ptr++ == MAX_REG_STACK)
		fatal("GetTempVectorRegister(): register stack overflow");
	return (ap);
}

Operand *GetTempFPRegister()
{
	Operand *ap;
  Function *sym = currentFn;
	int number;

#ifdef QUPLS
	return (GetTempRegister());
#endif
#ifdef QUPLS40
	return (GetTempRegister());
#endif
#ifdef THOR
	return (GetTempRegister());
#endif
	// Dead code follows
	number = fpreg_in_use[next_fpreg];
	if (number >= 0) {
		SpillFPRegister(fpreg_alloc[number].Operand,number);
	}
//	if (compiler.reg_in_use[next_reg] >= 0) {
//		GenerateTempRegPush(next_reg, am_reg, compiler.reg_in_use[next_reg],0);
//	}
	TRACE(printf("GetTempFPRegister:r%d\r\n", next_fpreg);)
    fpreg_in_use[next_fpreg] = fpreg_alloc_ptr;
    ap = allocOperand();
    ap->mode = am_fpreg;
    ap->preg = cpu.ftmpregs[next_fpreg] | rt_float;
    ap->deep = fpreg_alloc_ptr;
		ap->typep = &stddouble;
//    fpreg_alloc[fpreg_alloc_ptr].reg = regs[next_fpreg];
    fpreg_alloc[fpreg_alloc_ptr].Operand = ap;
    fpreg_alloc[fpreg_alloc_ptr].f.isPushed = 'F';
    if (next_fpreg++ >= cpu.NumFtmpRegs)
    	next_fpreg = 0;		/* wrap around */
    if (fpreg_alloc_ptr++ == MAX_REG_STACK)
		fatal("GetTempFPRegister(): register stack overflow");
	return (ap);
}

Operand* GetTempPositRegister()
{
	Operand* ap;
	Function* sym = currentFn;
	int number;

	return (GetTempRegister());
	// Dead code follows
	number = preg_in_use[next_preg];
	if (number >= 0) {
		SpillPositRegister(preg_alloc[number].Operand, number);
	}
	//	if (compiler.reg_in_use[next_reg] >= 0) {
	//		GenerateTempRegPush(next_reg, am_reg, compiler.reg_in_use[next_reg],0);
	//	}
	TRACE(printf("GetTempPositRegister:r%d\r\n", next_preg);)
		preg_in_use[next_preg] = preg_alloc_ptr;
	ap = allocOperand();
	ap->mode = am_preg;
	ap->preg = next_preg;
	ap->deep = preg_alloc_ptr;
	ap->typep = &stdposit;
	ap->tp = &stdposit;
	preg_alloc[preg_alloc_ptr].reg = next_preg;
	preg_alloc[preg_alloc_ptr].Operand = ap;
	preg_alloc[preg_alloc_ptr].f.isPushed = 'F';
//	if (next_preg++ >= (regLastTemp|0x40))
//		next_preg = (regFirstTemp|0x40);		/* wrap around */
	if (preg_alloc_ptr++ == MAX_REG_STACK)
		fatal("GetTempFPRegister(): register stack overflow");
	return (ap);
}
//void RestoreTempRegs(int rgmask)
//{
//	int nn;
//	int rm;
//	int i;
//
//	nn = 0;
// 
//	for(nn = rgmask; nn > 0; nn--)
//		GenerateTempRegPop(0,0,0);
//	//if (rgmask != 0) {
//	//	for (nn = 1, rm = rgmask; nn <= 15; nn = nn + 1)
//	//		if ((rm>>nn) & 1) {
//	//			GenerateMonadic(op_pop,0,makereg(nn));
//	//			compiler.reg_in_use[nn] = 0;
//	//		}
//	//}
//}

//void RestoreTempBrRegs(int brgmask)
//{
//	int nn;
//	int rm;
//	int i;
//
//	nn = 0;
// 
//	for(nn = brgmask; nn > 0; nn--)
//		GenerateTempBrRegPop(0,0,0);
//}

/*
 * this routines checks if all allocated registers were freed
 */
void checkstack()
{
    int i;
    Function *sym = currentFn;

    for (i=1; i<= cpu.NumTmpRegs; i++)
        if (compiler.reg_in_use[i] != -1)
            fatal("checkstack()/1");
	if (next_reg != 0) {//sym->IsLeaf ? 1 : cpu.tmpregs[0]) {
		//printf("Nextreg: %d\r\n", next_reg);
        fatal("checkstack()/3");
	}
    if (reg_stack_ptr != 0)
        fatal("checkstack()/5");
    if (reg_alloc_ptr != 0)
        fatal("checkstack()/6");
}

void checkbrstack()
{
    int i;
    for (i=5; i<= 8; i++)
        if (breg_in_use[i] != -1)
            fatal("checkbstack()/1");
	if (next_breg != 5) {
		//printf("Nextreg: %d\r\n", next_breg);
        fatal("checkbstack()/3");
	}
    if (breg_stack_ptr != 0)
        fatal("checkbstack()/5");
    if (breg_alloc_ptr != 0)
        fatal("checkbstack()/6");
}

/*
 * validate will make sure that if a register within an address mode has been
 * pushed onto the stack that it is popped back at this time.
 */
void validate(Operand *ap)
{
	Function *sym = currentFn;
	unsigned int frg = (unsigned)0;// regFirstTemp;
	int ndx;

	if (ap->typep!=&stdvector)
    switch (ap->mode) {
	case am_reg:
		if (wrapno > 0) {
			if (IsTempReg(ap->preg & VR_MASK) && cpu.regs[ap->preg & VR_MASK].isPushed.isMember(wrapno-1))//reg_alloc[ap->pdeep].f.isPushed == 'T') {
				LoadRegister(ap->preg, (int)ap->pdeep);
		}
		break;
	case amCrReg:
		if (CrWrapno > 0) {
			if (cpu.regs[ap->preg & VR_MASK].isPushed.isMember(CrWrapno-1))
				LoadCrRegister(ap->preg);
		}
		break;
	case am_fpreg:
		if (IsFtmpReg(ap->preg) && fpreg_alloc[ap->pdeep].f.isPushed == 'T') {
			LoadFPRegister(ap->preg, (int)ap->pdeep);
		}
		break;
	case am_preg:
		/*
		if ((ap->preg >= (frg|0x40) && ap->preg <= (unsigned)(regLastTemp|0x40)) && preg_alloc[ap->deep].f.isPushed == 'T') {
			LoadPositRegister(ap->preg, (int)ap->deep);
		}
		*/
		break;
	case am_indx2:
		if (wrapno > 0) {
			if (IsTempReg(ap->preg & VR_MASK) && cpu.regs[ap->preg & VR_MASK].isPushed.isMember(wrapno - 1))//reg_alloc[ap->pdeep].f.isPushed == 'T') {
				LoadRegister(ap->preg, (int)ap->pdeep);
			if (IsTempReg(ap->sreg & VR_MASK) && cpu.regs[ap->sreg & VR_MASK].isPushed.isMember(wrapno - 1))//reg_alloc[ap->pdeep].f.isPushed == 'T') {
				LoadRegister(ap->sreg, (int)ap->pdeep);
		}
#if 0
		if (IsTempReg(ap->preg) && reg_alloc[ap->deep].f.isPushed == 'T') {
			LoadRegister(ap->preg, (int) ap->deep);
		}
		if (IsTempReg(ap->sreg) && reg_alloc[ap->deep2].f.isPushed  == 'T') {
			LoadRegister(ap->sreg, (int) ap->deep2);
		}
#endif
		break;
  case am_ind:
  case am_indx:
  case am_ainc:
  case am_adec:
		if (wrapno > 0) {
			if (IsTempReg(ap->preg & VR_MASK) && cpu.regs[ap->preg & VR_MASK].isPushed.isMember(wrapno - 1))//reg_alloc[ap->pdeep].f.isPushed == 'T') {
				LoadRegister(ap->preg, (int)ap->pdeep);
		}
#if 0
		if (IsTempReg(ap->preg) && reg_alloc[ap->deep].f.isPushed == 'T') {
			LoadRegister(ap->preg, (int) ap->deep);
		}
#endif
		break;
  }
}

#if 0
static void ReleaseTempRegisterReg(Operand* ap, int regno)
{
	int number;

	if (IsTempReg(regno)) {
		if (compiler.reg_in_use[regno & VR_MASK] == -1)
			return;
		if (next_reg == 0) {
			next_reg = cpu.NumTmpRegs - 1;// regLastTemp;
			wrapno--;
		}
		else
			next_reg--;
		number = compiler.reg_in_use[regno & VR_MASK];
		compiler.reg_in_use[regno & VR_MASK] = -1;
		if (reg_alloc_ptr-- == 0 && ap->mode == am_reg)
			fatal("ReleaseTempRegister(): no registers are allocated");
	}
	return;
}
#endif

static void ReleaseTempRegisterReg(Operand* ap, int regno)
{
	int number;

	if (!cpu.regs[ap->preg & VR_MASK].inUse.isMember(wrapno))
		return;
	cpu.regs[ap->preg & VR_MASK].inUse.remove(wrapno);
	if (cpu.regs[next_reg].isFirstTemp) {
		wrapno--;
		wrapno_1 = cpu.regs[next_reg].prevreg - next_reg;
	}
	next_reg = cpu.regs[next_reg].prevreg;
	if (wrapno < 0)
		fatal("ReleaseTempRegister(): no cr registers are allocated");
}

static void ReleaseTempRegisterCrReg(Operand* ap, int regno)
{
	int number;
	int ii;

	if (!cpu.regs[ap->preg & VR_MASK].inUse.isMember(CrWrapno))
		return;
	cpu.regs[ap->preg & VR_MASK].inUse.remove(CrWrapno);
	// Detect if all ccr regs are no longer in use. If none are in use then the
	// ccr group register is not in use either. Used to suppress push/pop of
	// ccr group register.
	for (ii = 0; ii < cpu.NumRegs; ii++) {
		if (cpu.regs[ii].cls == rc_ccr) {
			if (cpu.regs[ii].inUse.isMember(CrWrapno))
				break;
		}
	}
	if (ii == cpu.NumRegs)
		cpu.regs[40].inUse.remove(CrWrapno);

	if (cpu.regs[nextCrReg].isFirstTemp)
		CrWrapno--;
	nextCrReg = cpu.regs[nextCrReg].prevreg;
	if (CrWrapno < 0)
		fatal("ReleaseTempRegister(): no cr registers are allocated");
}

/*
 * release any temporary registers used in an addressing mode.
 */
void ReleaseTempRegister(Operand *ap)
{
	int nn, ndx;
  int number;
	bool inUse;
  Function *sym = currentFn;
	unsigned int frg = 0;// regFirstTemp;

	TRACE(printf("ReleaseTempRegister:r%d r%d\r\n", ap->preg, ap->sreg);)

	if (ap==NULL) {
		printf("DIAG - NULL pointer in ReleaseTempRegister\r\n");
		return;
	}

	// Kludgy here. The register is being release so at the moment it
	// is in use until it's released. The in_use flag will cause
	// validate not to work. Need to keep the value of in_use for later.
	if (ap->mode == amCrReg) {
		nn = compiler.CrRegInUse[ap->preg & VR_MASK];
		ndx = IsTempCrReg(ap->preg);
		inUse = cpu.regs[ap->preg & VR_MASK].inUse.isMember(CrWrapno);
		if (ap->typep != &stdvector) {
			compiler.CrRegInUse[ap->preg & VR_MASK] = -1;
			cpu.regs[ap->preg & VR_MASK].inUse.remove(CrWrapno);
		}
		validate(ap);
		compiler.CrRegInUse[ap->preg & VR_MASK] = nn;
		if (inUse)
			cpu.regs[ap->preg & VR_MASK].inUse.add(CrWrapno);
		else
			cpu.regs[ap->preg & VR_MASK].inUse.remove(CrWrapno);
	}
	else {
		nn = compiler.CrRegInUse[ap->preg & VR_MASK];
		ndx = IsTempReg(ap->preg);
		inUse = cpu.regs[ap->preg & VR_MASK].inUse.isMember(wrapno);
		if (ap->typep != &stdvector) {
			compiler.CrRegInUse[ap->preg & VR_MASK] = -1;
			cpu.regs[ap->preg & VR_MASK].inUse.remove(wrapno);
	}
		validate(ap);
		compiler.CrRegInUse[ap->preg & VR_MASK] = nn;
		if (inUse)
			cpu.regs[ap->preg & VR_MASK].inUse.add(wrapno);
		else
			cpu.regs[ap->preg & VR_MASK].inUse.remove(wrapno);
#if 0
		nn = compiler.reg_in_use[ap->preg];
		if (ap->typep != &stdvector && ap->mode != am_fpreg)
			compiler.reg_in_use[ap->preg&VR_MASK] = -1;
		validate(ap);
		compiler.reg_in_use[ap->preg&VR_MASK] = nn;
#endif
	}

	if (ap->typep==&stdvector) {
		switch (ap->mode) {
		case am_vmreg:
			if (ap->preg >= 0 && ap->preg <= cpu.NumvTmpRegs) {
				if (vmreg_in_use[ap->preg]==-1)
					return;
				next_vreg--;
				if (next_vmreg < 0)
					next_vmreg = cpu.NumvTmpRegs;
				number = vmreg_in_use[ap->preg];
				vmreg_in_use[ap->preg] = -1;
				break;
			}
			return;
		case am_ind:
		case am_indx:
		case am_ainc:
		case am_adec:
		case am_reg:
		case amCrReg:
	commonv:
			if (ap->preg >= frg && ap->preg <= cpu.NumvTmpRegs) {
				if (vreg_in_use[ap->preg]==-1)
					return;
				next_vreg--;
				if (next_vreg < 0)
					next_vreg = cpu.NumvTmpRegs;
				number = vreg_in_use[ap->preg];
				vreg_in_use[ap->preg] = -1;
				break;
			}
			return;
		case am_indx2:
			if (ap->sreg >= frg && ap->sreg <= cpu.NumvTmpRegs) {
				if (vreg_in_use[ap->sreg]==-1)
					return;
				next_vreg--;
				if (next_vreg < 0)
					next_vreg = cpu.NumvTmpRegs;
				number = vreg_in_use[ap->sreg];
				vreg_in_use[ap->sreg] = -1;
				//break;
			}
			goto commonv;
		default:
			return;
		}
		if (vreg_alloc_ptr-- == 0)
			fatal("ReleaseTempRegister(): no vector registers are allocated");
	  //  if (reg_alloc_ptr != number)
			//fatal("ReleaseTempRegister()/3");
		if (vreg_alloc[number].f.isPushed=='T')
			fatal("ReleaseTempRegister(): vector register on stack");
		return;
	}
	else
    switch (ap->mode) {
	case am_fpreg:
		if (IsFtmpReg(ap->preg)) {
			if (fpreg_in_use[ap->preg]==-1)
				return;
			next_fpreg--;
			if (next_fpreg < 0)
				next_fpreg = cpu.NumFtmpRegs-1;
			number = fpreg_in_use[ap->preg];
			fpreg_in_use[ap->preg] = -1;
			if (fpreg_alloc_ptr-- == 0)
				fatal("ReleaseTempRegister(): no registers are allocated");
		  //  if (reg_alloc_ptr != number)
				//fatal("ReleaseTempRegister()/3");
			if (fpreg_alloc[number].f.isPushed=='T')
				fatal("ReleaseTempRegister(): register on stack");
			return;
		}
		return;
	case am_preg:
		/*
		if (ap->preg >= (frg|0x40) && ap->preg <= (unsigned)(regLastTemp|0x40)) {
			if (preg_in_use[ap->preg] == -1)
				return;
			if (next_preg-- <= (frg|0x40))
				next_preg = regLastTemp|0x40;
			number = preg_in_use[ap->preg];
			preg_in_use[ap->preg] = -1;
			if (preg_alloc_ptr-- == 0)
				fatal("ReleaseTempRegister(): no registers are allocated");
			//  if (reg_alloc_ptr != number)
				//fatal("ReleaseTempRegister()/3");
			if (preg_alloc[number].f.isPushed == 'T')
				fatal("ReleaseTempRegister(): register on stack");
			return;
		}
		*/
		return;

	case amCrReg:
		ReleaseTempRegisterCrReg(ap, ap->preg);
		return;

	case am_ind:
	case am_indx:
	case am_ainc:
	case am_adec:
	case am_reg:
	common:
		ReleaseTempRegisterReg(ap, ap->preg);
		return;
  case am_indx2:
		ReleaseTempRegisterReg(ap, ap->sreg);
		ReleaseTempRegisterReg(ap, ap->preg);
		return;
  default:
		return;
  }
 //   /* some consistency checks */
	//if (number != ap->deep) {
	//	printf("number %d ap->deep %d\r\n", number, ap->deep);
	//	//fatal("ReleaseTempRegister()/1");
	//}
	//  if (reg_alloc_ptr != number)
		//fatal("ReleaseTempRegister()/3");
	//if (reg_alloc[number].f.isPushed=='T')
	//	fatal("ReleaseTempRegister(): register on stack");
}

void ReleaseTempVectorMaskRegister()
{
}

void ReleaseTempVectorRegister(Operand* ap)
{
}

// The following is used to save temporary registers across function calls.
// Save the list of allocated registers and registers in use.
// Go through the allocated register list and generate a push instruction to
// put the register on the stack if it isn't already on the stack.

int64_t smask;

int TempInvalidate(int *fsp, int* psp, int* vsp)
{
  int i;
	int sp;
	int64_t mask = 0;
	int64_t fpmask = 0;
	int64_t vmask = 0;
	int mode;
	int64_t msk = 0;

	sp = 0;
	stacked_regs_set->clear();
	stacked_ccr_regs_set.clear();
	smask = 0;
	TRACE(printf("TempInvalidate()\r\n");)
	save_wrapno = wrapno;
	saveCrWrapno = CrWrapno;

	save_reg_alloc_ptr = reg_alloc_ptr;
	memcpy(save_reg_alloc, reg_alloc, sizeof(save_reg_alloc));
	memcpy(save_reg_in_use, compiler.reg_in_use, sizeof(save_reg_in_use));
	memcpy(save_rap, rap, sizeof(rap));

	saveCrRegAllocPtr = CrRegAllocPtr;
	memcpy(saveCrRegAlloc, CrRegAlloc, sizeof(saveCrRegAlloc));
	memcpy(saveCrRegInUse, compiler.CrRegInUse, sizeof(saveCrRegInUse));
	memcpy(save_Crrap, Crrap, sizeof(Crrap));

	save_fpreg_alloc_ptr = fpreg_alloc_ptr;
	memcpy(save_fpreg_alloc, fpreg_alloc, sizeof(save_fpreg_alloc));
	memcpy(save_fpreg_in_use, fpreg_in_use, sizeof(save_fpreg_in_use));

	save_vreg_alloc_ptr = vreg_alloc_ptr;
	memcpy(save_vreg_alloc, vreg_alloc, sizeof(save_vreg_alloc));
	memcpy(save_vreg_in_use, vreg_in_use, sizeof(save_vreg_in_use));
#if 0
	for (sp = i = 0; i < reg_alloc_ptr; i++) {
		if (reg_alloc[i].f.isPushed == 'F') {
			mode = reg_alloc[i].Operand->mode;
			reg_alloc[i].Operand->mode = am_reg;
			if (!(mask & (1LL << (reg_alloc[i].Operand->preg & 0x3f)))) {
				SpillRegister(reg_alloc[i].Operand, i);
				mask = mask | (1LL << (reg_alloc[i].Operand->preg & 0x3f));
			}
			reg_alloc[i].Operand->mode = mode;
			//GenerateTempRegPush(reg_alloc[i].reg, /*reg_alloc[i].Operand->mode*/am_reg, i, sp);
			stacked_regs[sp].reg = reg_alloc[i].reg;
			stacked_regs[sp].Operand = reg_alloc[i].Operand;
			stacked_regs[sp].f.allocnum = i;
			sp++;
			// mark the register void
			compiler.reg_in_use[reg_alloc[i].reg&VR_MASK] = -1;
    }
	}
#endif
	for (i = 0; i < cpu.NumRegs; i++) {
		if (!cpu.regs[i].isPushed.isMember(wrapno) && cpu.regs[i].inUse.isMember(wrapno)) {
			stacked_regs_set->add(i);
			smask = smask | (1LL << i);
		}
	}
	if (stacked_regs_set->NumMember() > 2)
		GenerateMonadic(op_pushm, 0, cg.MakeImmediate(smask));
	else {
		for (i = 0; i < cpu.NumRegs; i++) {
			if (!cpu.regs[i].isPushed.isMember(wrapno) && cpu.regs[i].inUse.isMember(wrapno)) {
				SpillRegister(makereg(cpu.regs[i].number), i);
			}
		}
	}
	for (i = 0; i < cpu.NumRegs; i++) {
		if (!cpu.regs[i].isPushed.isMember(wrapno) && cpu.regs[i].inUse.isMember(wrapno)) {
			cpu.regs[i].inUse.remove(wrapno);
			cpu.regs[i].isPushed.add(wrapno);
		}
	}
	/*
	for (i = 0; i < cpu.NumTmpCrRegs; i++) {
		if (!cpu.tmpCrRegs[i].isPushed.isMember(CrWrapno)) {
			SpillCrRegister(makeCrReg(cpu.tmpCrRegs[i].number), i);
		}
	}
	*/
	/*
	for (sp = i = 0; i < CrRegAllocPtr; i++) {
		if (CrRegAlloc[i].f.isPushed == 'F') {
			mode = CrRegAlloc[i].Operand->mode;
			CrRegAlloc[i].Operand->mode = amCrReg;
			if (!(mask & (1LL << (CrRegAlloc[i].Operand->preg & 0x3f)))) {
				SpillCrRegister(CrRegAlloc[i].Operand, i);
				mask = mask | (1LL << (CrRegAlloc[i].Operand->preg & 0x3f));
			}
			CrRegAlloc[i].Operand->mode = mode;
			//GenerateTempRegPush(reg_alloc[i].reg, /reg_alloc[i].Operand->mode/am_reg, i, sp);
			stackedCrRegs[sp].reg = CrRegAlloc[i].reg;
			stackedCrRegs[sp].Operand = CrRegAlloc[i].Operand;
			stackedCrRegs[sp].f.allocnum = i;
			sp++;
			// mark the register void
			compiler.CrRegInUse[CrRegAlloc[i].reg&VR_MASK] = -1;
		}
	}
	*/
	for (*fsp = i = 0; i < fpreg_alloc_ptr; i++) {
		if (fpreg_alloc[i].f.isPushed == 'F') {
			mode = fpreg_alloc[i].Operand->mode;
			fpreg_alloc[i].Operand->mode = am_fpreg;
			if (!(fpmask & (1LL << (fpreg_alloc[i].Operand->preg & 0x3f)))) {
				SpillFPRegister(reg_alloc[i].Operand, i);
				fpmask = fpmask | (1LL << (reg_alloc[i].Operand->preg & 0x3f));
			}
			fpreg_alloc[i].Operand->mode = mode;
			//GenerateTempRegPush(reg_alloc[i].reg, /*reg_alloc[i].Operand->mode*/am_reg, i, sp);
			stacked_fpregs[sp].reg = fpreg_alloc[i].reg;
			stacked_fpregs[sp].Operand = fpreg_alloc[i].Operand;
			stacked_fpregs[sp].f.allocnum = i;
			(*fsp)++;
			// mark the register void
			fpreg_in_use[fpreg_alloc[i].reg] = -1;
		}
	}
	for (*vsp = i = 0; i < vreg_alloc_ptr; i++) {
		if (vreg_alloc[i].f.isPushed == 'F') {
			mode = reg_alloc[i].Operand->mode;
			reg_alloc[i].Operand->mode = am_vreg;
			if (!(vmask & (1LL << reg_alloc[i].Operand->preg))) {
				SpillVectorRegister(reg_alloc[i].Operand, i);
				vmask = vmask | (1LL << vreg_alloc[i].Operand->preg);
			}
			vreg_alloc[i].Operand->mode = mode;
			//GenerateTempRegPush(reg_alloc[i].reg, /*reg_alloc[i].Operand->mode*/am_reg, i, sp);
			stacked_vregs[sp].reg = vreg_alloc[i].reg;
			stacked_vregs[sp].Operand = vreg_alloc[i].Operand;
			stacked_vregs[sp].f.allocnum = i;
			(*vsp)++;
			// mark the register void
			vreg_in_use[vreg_alloc[i].reg] = -1;
		}
	}
	memset(compiler.reg_in_use, -1, sizeof(compiler.reg_in_use));
	memset(fpreg_in_use, -1, sizeof(fpreg_in_use));

	memset(vreg_in_use, -1, sizeof(vreg_in_use));
	/*
	save_fpreg_alloc_ptr = fpreg_alloc_ptr;
	memcpy(save_fpreg_alloc, fpreg_alloc, sizeof(save_fpreg_alloc));
	memcpy(save_fpreg_in_use, fpreg_in_use, sizeof(save_fpreg_in_use));
	for (*fsp = i = 0; i < fpreg_alloc_ptr; i++) {
        if (fpreg_in_use[fpreg_alloc[i].reg] != -1) {
    		if (fpreg_alloc[i].f.isPushed == 'F') {
				// ToDo: fix this line
				mode = fpreg_alloc[i].Operand->mode;
				fpreg_alloc[i].Operand->mode = am_fpreg;
				SpillFPRegister(fpreg_alloc[i].Operand, i);
				fpreg_alloc[i].Operand->mode = mode;
					stacked_fpregs[sp].reg = fpreg_alloc[i].reg;
    			stacked_fpregs[sp].Operand = fpreg_alloc[i].Operand;
    			stacked_fpregs[sp].f.allocnum = i;
    			(*fsp)++;
    			// mark the register void
    			fpreg_in_use[fpreg_alloc[i].reg] = -1;
    		}
        }
	}
	*/
	/*
	save_preg_alloc_ptr = preg_alloc_ptr;
	memcpy(save_preg_alloc, preg_alloc, sizeof(save_preg_alloc));
	memcpy(save_preg_in_use, preg_in_use, sizeof(save_preg_in_use));
	for (*psp = i = 0; i < preg_alloc_ptr; i++) {
		if (preg_in_use[preg_alloc[i].reg] != -1) {
			if (preg_alloc[i].f.isPushed == 'F') {
				// ToDo: fix this line
				mode = preg_alloc[i].Operand->mode;
				preg_alloc[i].Operand->mode = am_preg;
				SpillPositRegister(preg_alloc[i].Operand, i);
				preg_alloc[i].Operand->mode = mode;
				stacked_pregs[sp].reg = preg_alloc[i].reg;
				stacked_pregs[sp].Operand = preg_alloc[i].Operand;
				stacked_pregs[sp].f.allocnum = i;
				(*psp)++;
				// mark the register void
				preg_in_use[preg_alloc[i].reg] = -1;
			}
		}
	}
	*/
	// Scalar regs
	//wrapno = 0;
	reg_alloc_ptr = 0;
	memset(compiler.reg_in_use, -1, sizeof(compiler.reg_in_use));
	ZeroMemory(reg_alloc, sizeof(reg_alloc));
	ZeroMemory(rap, sizeof(rap));
	// Condition regs
//	CrWrapno = 0;
	// Float
	fpreg_alloc_ptr = 0;
	memset(fpreg_in_use, -1, sizeof(fpreg_in_use));
	ZeroMemory(fpreg_alloc, sizeof(fpreg_alloc));
	// Vector regs
	vreg_alloc_ptr = 0;
	memset(vreg_in_use, -1, sizeof(vreg_in_use));
	ZeroMemory(vreg_alloc, sizeof(vreg_alloc));
	return (sp);
}

// Pop back any temporary registers that were pushed before the function call.
// Restore the allocated and in use register lists.

void TempRevalidate(int sp, int fsp, int psp, int vsp)
{
	int nn;
	CSet mask;
	CSet fpmask;
	CSet vmask;
	CSet CrMask;

	/*
	for (nn = psp - 1; nn >= 0; nn--) {
		if (stacked_pregs[nn].Operand)
			LoadPositRegister(stacked_pregs[nn].Operand->preg, stacked_pregs[nn].f.allocnum);
	}
	preg_alloc_ptr = save_preg_alloc_ptr;
	memcpy(preg_alloc, save_preg_alloc, sizeof(preg_alloc));
	memcpy(preg_in_use, save_preg_in_use, sizeof(preg_in_use));

	for (nn = fsp-1; nn >= 0; nn--) {
		if (stacked_fpregs[nn].Operand)
			LoadFPRegister(stacked_fpregs[nn].Operand->preg, stacked_fpregs[nn].f.allocnum);
	}
	fpreg_alloc_ptr = save_fpreg_alloc_ptr;
	memcpy(fpreg_alloc, save_fpreg_alloc, sizeof(fpreg_alloc));
	memcpy(fpreg_in_use, save_fpreg_in_use, sizeof(fpreg_in_use));
	*/
	mask.clear();
	if (stacked_regs_set->NumMember() > 2)
		GenerateMonadic(op_popm, 0, cg.MakeImmediate(smask));
	else {
		for (nn = 63; nn >= 0; nn--)
			if (stacked_regs_set->isMember(nn))
				LoadRegister(nn & VR_MASK, 0);
#if 0
		for (nn = sp - 1; nn >= 0; nn--) {
			if (!(mask.isMember(stacked_regs[nn].Operand->preg & VR_MASK)))
				LoadRegister(stacked_regs[nn].Operand->preg & VR_MASK, stacked_regs[nn].f.allocnum);
			mask.add(stacked_regs[nn].Operand->preg & VR_MASK);
			//GenerateTempRegPop(stacked_regs[nn].reg, /*stacked_regs[nn].Operand->mode*/am_reg, stacked_regs[nn].f.allocnum,sp-nn-1);
		}
#endif
	}
	for (nn = 0; nn < cpu.NumRegs; nn++) {
		if (stacked_regs_set->isMember(nn)) {
			cpu.regs[nn].isPushed.remove(wrapno);
			cpu.regs[nn].inUse.add(wrapno);
		}
	}
	//CrWrapno = saveCrWrapno;
	//CrMask.clear();
	/*
	for (nn = 0; nn < cpu.NumTmpCrRegs; nn++) {
		if (cpu.tmpCrRegs[nn].isPushed.isMember(CrWrapno)) {
			LoadCrRegister(cpu.tmpCrRegs[nn].number, nn);// stackedCrRegs[nn].f.allocnum);
		}
	}
	*/
	/*
	for (nn = sp - 1; nn >= 0; nn--) {
		if (!(CrMask.isMember(stackedCrRegs[nn].Operand->preg&VR_MASK)))
			LoadCrRegister(stackedCrRegs[nn].Operand->preg&7, stackedCrRegs[nn].f.allocnum);
		CrMask.add(stackedCrRegs[nn].Operand->preg&VR_MASK);
		//GenerateTempRegPop(stacked_regs[nn].reg, /*stacked_regs[nn].Operand->mode /am_reg, stacked_regs[nn].f.allocnum,sp-nn-1);
	}
	*/
	fpmask.clear();
	for (nn = fsp - 1; nn >= 0; nn--) {
		if (stacked_fpregs[nn].Operand) {
			if (!(fpmask.isMember(stacked_fpregs[nn].Operand->preg & VR_MASK)))
				LoadFPRegister(stacked_fpregs[nn].Operand->preg, stacked_fpregs[nn].f.allocnum);
			fpmask.add(stacked_regs[nn].Operand->preg & VR_MASK);
		}
	}
	vmask.clear();
	for (nn = vsp - 1; nn >= 0; nn--) {
		if (stacked_vregs[nn].Operand) {
			if (!(vmask.isMember(stacked_vregs[nn].Operand->preg & VR_MASK)))
				LoadVectorRegister(stacked_vregs[nn].Operand->preg, stacked_vregs[nn].f.allocnum);
			vmask.add(stacked_vregs[nn].Operand->preg & VR_MASK);
		}
	}
	wrapno = save_wrapno;
	reg_alloc_ptr = save_reg_alloc_ptr;
	memcpy(reg_alloc, save_reg_alloc, sizeof(reg_alloc));
	memcpy(compiler.reg_in_use, save_reg_in_use, sizeof(compiler.reg_in_use));
	memcpy(rap, save_rap, sizeof(rap));
	// Condition
	CrWrapno = saveCrWrapno;
	// Float
	fpreg_alloc_ptr = save_fpreg_alloc_ptr;
	memcpy(fpreg_alloc, save_fpreg_alloc, sizeof(fpreg_alloc));
	memcpy(fpreg_in_use, save_fpreg_in_use, sizeof(fpreg_in_use));
	// Vector
	vreg_alloc_ptr = save_vreg_alloc_ptr;
	memcpy(vreg_alloc, save_vreg_alloc, sizeof(vreg_alloc));
	memcpy(vreg_in_use, save_vreg_in_use, sizeof(vreg_in_use));
}

/*
void initRegStack()
{
	for (rsp=0; rsp < 8; rsp=rsp+1)
		regstack[rsp] = tmpregs[rsp];
	for (brsp = 0; brsp < 6; brsp++)
		bregstack[brsp] = tmpbregs[brsp];
	rsp = 0;
	brsp = 0;
}

void ReleaseTempRegister(Operand *ap)
{
	if (ap==NULL) {
		printf("DIAG - NULL pointer in ReleaseTempRegister\r\n");
		return;
	}
	if( ap->mode == am_imm || ap->mode == am_direct )
        return;         // no registers used
	if (ap->mode == am_breg || ap->mode==am_brind) {
		if (ap->preg < 9 && ap->preg >= 3)
			PushOnBrstk(ap->preg);
		return;
	}
	if(ap->preg < 11 && ap->preg >= 3)
		PushOnRstk(ap->preg);
}
*/
Operand *GetTempReg(TYP* typ)
{
	if (typ==&stdvectormask)
		return (GetTempVectorMaskRegister());
	else if (typ==&stdvector)
		return (GetTempVectorRegister());
	else if (typ==&stddouble)
		return (GetTempFPRegister());
	else
		return (GetTempRegister());
}

void ReleaseTempFPRegister(Operand *ap)
{
     ReleaseTempRegister(ap);
}

void ReleaseTempPositRegister(Operand* ap)
{
	ReleaseTempRegister(ap);
}

void ReleaseTempReg(Operand *ap)
{
	if (ap==nullptr)
		return;
	if (ap->typep==&stdvectormask)
		ReleaseTempVectorMaskRegister();
//	else if (ap->typep==&stdvector)
//		ReleaseTempVectorRegister(ap);
	else if (ap->typep->IsFloatType())
		ReleaseTempFPRegister(ap);
	else if (ap->typep == &stdposit)
		ReleaseTempPositRegister(ap);
	else
		ReleaseTempRegister(ap);
	if (ap->toRelease)
		ReleaseTempReg(ap->toRelease);
}

int GetTempMemSpace()
{
	return (max_reg_alloc_ptr * cpu.sizeOfWord);
}

bool IsArgumentReg(int regno)
{
	return (IsArgReg(regno));
}

bool IsCalleeSave(int regno)
{
	if (IsTempReg(regno))
		return (true);
	if (regno==regSP || regno==regFP)
		return (true);
	if (regno==regTP)
		return (true);
	return(false);
}

