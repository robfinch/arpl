// ============================================================================
//        __
//   \\__/ o\    (C) 2023-2025  Robert Finch, Waterloo
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
#include "..\..\stdafx.h"

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
extern void GenLoad(Operand *ap1, Operand *ap3, int ssize, int size);

// Please keep table in alphabetical order.
// Instruction.cpp has the number of table elements hard-coded in it.
//
static Instruction starkInsnTbl[] =
{
{ ";", op_remark },
{ ";asm",op_asm,300 },
{ ";empty",op_empty },
{ ";fname", op_fnname },
{ ";string", op_string },
{ "abs", op_abs,2,1,false,am_reg,am_reg,0,0 },
{ "add",op_add,1,1,false,am_reg,am_reg,am_reg | am_imm,0 },
{ "adds",op_adds,1,1,false,am_reg,am_reg,am_reg, 0 },
{ "addu", op_addu,1,1 },
{ "and",op_and,1,1,false,am_reg | amCrReg,am_reg | amCrReg,am_reg | amCrReg | am_imm,0 },
{ "andcm",op_andcm,1,1,false,am_reg,am_reg,am_reg,0 },
{ "asl", op_asl,2,1,false,am_reg,am_reg,am_reg | am_ui6,0 },
{ "aslx", op_aslx,2,1,false,am_reg,am_reg,am_reg | am_ui6,0 },
{ "asr",op_asr,2,1,false,am_reg,am_reg,am_reg | am_ui6,0 },
{ "b",op_b,3,0,false,am_direct|am_brind,0,0,0 },
{ "band", op_band,2,0,false,am_reg,am_reg,0,0 },
{ "base", op_base,1,0,false,am_reg,am_reg,am_reg | am_ui6,0 },
{ "bchk", op_bchk,3,0 },
{ "beq", op_beq,3,0,false,amCrReg,am_reg,am_direct|am_brind,0 },
{ "bex", op_bex,0,0,false,0,0,0,0 },
{ "bf", op_bf,3,0,false,am_reg,am_direct,0,0 },
{ "bfclr", op_bfclr,2,1,false,am_reg,am_reg | am_ui6,am_reg | am_ui6,0 },
{ "bfext", op_bfext,2,1,false,am_reg },
{ "bfextu", op_bfextu,2,1,false,am_reg, },
{ "bfins", op_bfins,2,1,false,am_reg },
{ "bfset", op_bfset,2,1,false,am_reg,am_reg | am_ui6,am_reg | am_ui6,0 },
{ "bge", op_bge,3,0,false,amCrReg,am_reg,am_direct|am_brind,0 },
{ "bgt", op_bgt,3,0,false,amCrReg,am_reg,am_direct|am_brind,0 },
{ "bit",op_bit,1,1,false,am_creg,am_reg,am_reg | am_imm,0 },
{ "bl", op_bl,3,0,false,am_direct,0,0,0 },
{ "ble", op_ble, 3,0,false,amCrReg,am_reg,am_direct|am_brind,0 },
{ "blt", op_blt,3,0,false,amCrReg,am_reg,am_direct|am_brind,0 },
{ "bmap", op_bmap,1,0,false,am_reg,am_reg,am_reg | am_imm,0 },
{ "bne", op_bne,3,0,false,amCrReg,am_reg,am_direct|am_brind,0 },
{ "branch",op_bra,3,0,false,0,0,0,0 },
//{ "branch",op_branch,3,0,false,am_direct,0,0,0 },
{ "brk", op_brk,1,0 },
{ "bun", op_bun,2,0 },
{ "bytndx", op_bytendx,1,1,false,am_reg,am_reg,am_reg | am_imm,0 },
{ "cache",op_cache,1,0 },
{ "call", op_call,4,1,false,0,0,0,0 },
{ "chk", op_chk,1,0 },
{ "clr",op_clr,1,1,false,am_reg,am_reg,am_reg | am_imm, am_imm },
{ "cmovenz", op_cmovenz,1,1,false,am_reg,am_reg,am_reg,am_reg },
{ "cmp",op_cmp,1,1,false,am_reg,am_reg | am_imm,am_reg | am_imm,0 },
{ "cmpa",op_cmpa,1,1,false,am_reg,am_reg,am_reg | am_imm,0 },
{ "com", op_com,2,1,false,am_reg,am_reg,0,0 },
{ "cr2cr", op_cr2cr,1,0,false,amCrReg,amCrReg,0,0},
{ "crand",op_crand,1,0,false,amCrReg,amCrReg,amCrReg,0 },
{ "crdep", op_crdep,1,0,false,amCrReg,am_reg,0,0},
{ "creor",op_creor,1,0,false,amCrReg,amCrReg,amCrReg,0 },
{ "crext", op_crext,1,0,false,am_reg,amCrReg,am_imm,0},
{ "crg2reg", op_crg2reg,1,0,false,am_reg,amCrReg,0,0},
{ "cror",op_cror,1,0,false,amCrReg,amCrReg,amCrReg,0 },
{ "csrrd", op_csrrd,1,1,false,am_reg,am_reg,am_imm },
{ "csrrw", op_csrrw,1,1,false,am_reg,am_reg,am_imm },
{ "dec", op_dec,4,0,true,am_i5 },
{ "defcat", op_defcat,12,1,false,am_reg,am_reg,0 ,0 },
{ "dep",op_dep,1,1,false,am_reg,am_reg,am_reg | am_imm,am_reg | am_imm },
{ "di", op_di,1,1,false,am_reg | am_ui6,0,0,0 },
{ "div", op_div,68,1,false,am_reg,am_reg,am_reg | am_imm,0 },
{ "divu",op_divu,68,1,false,am_reg,am_reg,am_reg | am_imm,0 },
{ "dw", op_dw },
{ "enter", op_enter,40,0,true,am_imm,0,0,0 },
{ "eor",op_eor,1,1,false,am_reg,am_reg,am_reg | am_imm,0 },
{ "eq",op_eq, 1, 1, false, am_reg, am_reg, am_reg | am_imm,0 },
{ "ext", op_ext,1,1,false,am_reg,am_reg,am_reg | am_imm | am_imm0, am_reg | am_imm | am_imm0 },
{ "extu", op_extu,1,1,false,am_reg,am_reg,am_reg | am_imm | am_imm0, am_reg | am_imm | am_imm0 },
{ "fadd", op_fadd, 6, 1, false, am_reg, am_reg, am_reg, 0 },
{ "fadd.d", op_fdadd,6,1,false,am_reg,am_reg,am_reg,0 },
{ "fadd.s", op_fsadd,6,1,false,am_reg,am_reg,am_reg,0 },
{ "fbeq", op_fbeq,3,0,false,am_reg,am_reg,0,0 },
{ "fbge", op_fbge,3,0,false,am_reg,am_reg,0,0 },
{ "fbgt", op_fbgt,3,0,false,am_reg,am_reg,0,0 },
{ "fble", op_fble,3,0,false,am_reg,am_reg,0,0 },
{ "fblt", op_fblt,3,0,false,am_reg,am_reg,0,0 },
{ "fbne", op_fbne,3,0,false,am_reg,am_reg,0,0 },
{ "fbor", op_fbor,3,0,false,am_reg,am_reg,0,0 },
{ "fbun", op_fbun,3,0,false,am_reg,am_reg,0,0 },
{ "fcmp", op_fcmp, 1,1,false,am_reg,am_reg | am_imm,am_reg | am_imm,0 },
{ "fcvt.q.d", op_fcvtqdd,2,1,false,am_reg,am_reg,0,0 },
{ "fcvtdq", op_fcvtdq,2,1,false,am_reg,am_reg,0,0 },
{ "fcvtsq", op_fcvtsq,2,1,false,am_reg,am_reg,0,0 },
{ "fcvttq", op_fcvttq,2,1,false,am_reg,am_reg,0,0 },
{ "fdiv", op_fdiv, 160, 1, false, am_reg, am_reg | am_imm,am_reg | am_imm, 0 },
{ "fdiv.s", op_fsdiv,80,1,false },
{ "fi2d", op_i2d,2,1,false },
{ "fix2flt", op_fix2flt },
{ "fld", op_fld, 4, 1, true, am_reg, am_mem, 0, 0 },
{ "fload", op_fload, 4, 1, true, am_reg, am_mem, 0, 0 },
{ "flq", op_flq, 4, 1, true, am_reg, am_mem, 0, 0 },
{ "flt2fix",op_flt2fix },
{ "flw", op_flw, 4, 1, true, am_reg, am_mem, 0, 0 },
{ "fmov", op_fmov,1,1 },
{ "fmov.d", op_fdmov,1,1 },
{ "fmul", op_fdmul,10,1,false,am_reg,am_reg,am_reg,0 },
{ "fmul", op_fmul, 10, 1, false, am_reg | am_vreg, am_reg | am_vreg, am_reg | am_vreg, 0 },
{ "fmul.s", op_fsmul,10,1,false },
{ "fneg", op_fneg,2,1,false,am_reg,am_reg,0,0 },
{ "fs2d", op_fs2d,2,1,false,am_reg,am_reg,0,0 },
{ "fseq", op_fseq, 1, 1, false, am_creg, am_reg, am_reg, 0 },
{ "fsle", op_fsle, 1, 1, false, am_creg, am_reg, am_reg, 0 },
{ "fslt", op_fslt, 1, 1, false, am_creg, am_reg, am_reg, 0 },
{ "fsne", op_fsne, 1, 1, false, am_creg, am_reg, am_reg, 0 },
{ "fstore", op_fsto, 4, 0, true, am_reg, am_mem, 0, 0 },
{ "fsub", op_fdsub,6,1,false,am_reg,am_reg,am_reg,0 },
{ "fsub", op_fsub, 6, 1, false, am_reg, am_reg, am_reg, 0 },
{ "fsub.s", op_fssub,6,1,false },
{ "ftadd", op_ftadd },
{ "ftdiv", op_ftdiv },
{ "ftmul", op_ftmul },
{ "ftoi", op_ftoi, 2, 1, false, am_reg, am_reg, 0, 0 },
{ "ftsub", op_ftsub },
{ "gcsub",op_gcsub,1,1,false,am_reg,am_reg,am_reg | am_imm,0 },
{ "ge",op_ge, 1, 1, false, am_reg,am_reg, am_reg | am_imm,0 },
{ "geu", op_geu, 1, 1, false, am_reg, am_reg,am_reg | am_imm,0 },
{ "gt",op_gt, 1, 1, false, am_reg, am_reg, am_reg | am_imm,0 },
{ "gtu",op_gtu, 1, 1, false, am_reg, am_reg, am_reg | am_imm,0 },
{ "hint", op_hint,0 },
{ "hint2",op_hint2,0 },
{ "iblt", op_iblt,3,0,false,am_reg,am_reg,am_direct,0 },
{ "inc", op_inc,4,0,true,am_i5,am_mem,0,0 },
{ "iret", op_iret,2,0,false,0,0,0,0 },
{ "isnull", op_isnullptr,1,1,false,am_reg,am_reg,0,0 },
{ "itof", op_itof, 2, 1, false, am_reg, am_reg, 0, 0 },
{ "itop", op_itop, 2, 1, false, am_reg, am_reg, 0, 0 },
{ "jmp",op_jmp,1,0,false,am_mem,0,0,0 },
{ "jsr", op_jsr,1,1,false },
{ "l",op_l,1,1,false,am_reg,am_imm,0,0 },
{ "la",op_la,1,1,false,am_reg,am_mem,0,0 },
{ "lb", op_lb,4,1,true,am_reg,am_mem,0,0 },
{ "lbu", op_lbu,4,1,true,am_reg,am_mem,0,0 },
{ "ld", op_ld,4,1,true,am_reg,am_mem,0,0 },
{ "ldb", op_ldb,4,1,true,am_reg,am_mem,0,0 },
{ "ldbu", op_ldbu,4,1,true,am_reg,am_mem,0,0 },
{ "ldd", op_ldd,4,1,true,am_reg,am_mem,0,0 },
{ "lddr", op_lddr,4,1,true,am_reg,am_mem,0,0 },
{ "ldfd", op_ldfd,4,1,true, am_reg, am_mem,0,0 },
{ "ldft", op_ldft,4,1,true, am_reg, am_mem,0,0 },
{ "ldh", op_ldh,4,1,true,am_reg,am_mem,0,0 },
{ "ldhs", op_ldhs,4,1,true,am_reg,am_mem,0,0 },
{ "ldm", op_ldm,20,1,true,am_mem,0,0,0 },
{ "ldo", op_ldo,4,1,true,am_reg,am_mem,0,0 },
{ "ldos", op_ldos,4,1,true,am_reg,am_mem,0,0 },
{ "ldou", op_ldou,4,1,true,am_reg,am_mem,0,0 },
{ "ldp", op_ldp,4,1,true,am_reg,am_mem,0,0 },
{ "ldpu", op_ldpu,4,1,true,am_reg,am_mem,0,0 },
{ "ldt", op_ldt,4,1,true,am_reg,am_mem,0,0 },
{ "ldtu", op_ldtu,4,1,true,am_reg,am_mem,0,0 },
{ "ldw", op_ldw,4,1,true,am_reg,am_mem,0,0 },
{ "ldwu", op_ldwu,4,1,true,am_reg,am_mem,0,0 },
{ "le",op_le, 1, 1, false, am_reg, am_reg, am_reg | am_imm,0 },
{ "lea",op_lea,1,1,false,am_reg,am_mem,0,0 },
{ "leave", op_leave,40,0,true,am_imm,0,0,0 },
{ "leu",op_leu, 1, 1, false, am_reg, am_reg, am_reg | am_imm,0 },
{ "lh", op_lh,4,1,true,am_reg,am_mem,0,0 },
{ "lhu", op_lhu,4,1,true,am_reg,am_mem,0,0 },
{ "lm", op_lm },
{ "load", op_load,4,1,true,am_reg,am_mem,0,0 },
{ "loada",op_lda,1,1,false,am_reg,am_mem,0,0 },
{ "loadi",op_loadi,1,1,false,am_reg,am_imm,0,0 },
{ "loadm", op_loadm,32,1,true,am_imm,am_mem,0,0 },
{ "lsr", op_lsr,2,1,false,am_reg,am_reg,am_reg | am_ui6,0 },
{ "lt",op_lt, 1, 1, false, am_reg, am_reg, am_reg | am_imm,0 },
{ "ltu", op_ltu, 1, 1, false, am_reg, am_reg, am_reg | am_imm,0 },
{ "lui",op_lui,1,1,false,am_reg,am_imm,0,0 },
{ "lvbu", op_lvbu,4,1,true ,am_reg,am_mem,0,0 },
{ "lvcu", op_lvcu,4,1,true ,am_reg,am_mem,0,0 },
{ "lvhu", op_lvhu,4,1,true ,am_reg,am_mem,0,0 },
{ "lw", op_lw,4,1,true,am_reg,am_mem,0,0 },
{ "lwu", op_lwu,4,1,true,am_reg,am_mem,0,0 },
{ "lws", op_ldds,4,1,true },
{ "mfbase", op_mfbase,1,0,false,am_reg,am_reg | am_ui6,0,0 },
{ "mffp",op_mffp },
{ "mflk", op_mflk,1,0,false,am_reg,am_reg,0,0 },
{ "mod", op_mod,68,1, false,am_reg,am_reg,am_reg | am_imm,0 },
{ "modu", op_modu,68,1,false,am_reg,am_reg,am_reg,0 },
{ "mov", op_mov,1,1,false,am_reg,am_reg,0,0 },
{ "movb", op_movb,1,1,false,am_reg,am_reg,0,0 },
{ "move",op_move,1,1,false,am_reg,am_reg,0,0 },
{ "movs", op_movs },
{ "movsxb", op_movsxb,1,1,false,am_reg,am_reg,0,0 },
{ "movsxt", op_movsxt,1,1,false,am_reg,am_reg,0,0 },
{ "movsxw", op_movsxw,1,1,false,am_reg,am_reg,0,0 },
{ "movw", op_movw,1,1,false,am_reg,am_reg,0,0 },
{ "movzxb", op_movzxb,1,1,false,am_reg,am_reg,0,0 },
{ "movzxt", op_movzxt,1,1,false,am_reg,am_reg,0,0 },
{ "movzxw", op_movzxw,1,1,false,am_reg,am_reg,0,0 },
{ "mtbase", op_mtbase,1,0,false,am_reg,am_reg | am_ui6,0,0 },
{ "mul",op_mul,18,1,false,am_reg,am_reg,am_reg | am_imm,0 },
{ "mulf",op_mulf,1,1,false,am_reg,am_reg,am_reg | am_imm,0 },
{ "mulu", op_mulu, 10, 1, false, am_reg, am_reg, am_reg | am_imm, 0 },
{ "mv", op_mv,1,1,false,am_reg,am_reg,0,0 },
{ "nand",op_nand,1,1,false,am_reg | amCrReg,am_reg | amCrReg,am_reg | amCrReg,0 },
{ "ne",op_ne,1,1,false,am_reg,am_reg,am_reg | am_imm,0 },
{ "neg",op_neg, 1, 1, false,am_reg,am_reg,0,0 },
{ "nop", op_nop,0,0,false },
{ "nor",op_nor,1,1,false,am_reg | amCrReg,am_reg | amCrReg,am_reg | amCrReg,0 },
{ "not", op_not,2,1,false,am_reg,am_reg,0,0 },
{ "not",op_not,2,1, false,am_reg, am_reg,0,0 },
{ "or",op_or,1,1,false,am_reg | amCrReg,am_reg | amCrReg,am_reg | amCrReg | am_imm,0 },
{ "padd", op_padd, 6, 1, false, am_reg, am_reg, am_reg, 0 },
{ "pdiv", op_pdiv, 10, 1, false, am_reg, am_reg, am_reg, 0 },
{ "pea", op_pea },
{ "pea",op_pea },
{ "pfi", op_pfi, 1, 1, false, 0, 0, 0, 0 },
{ "pfx0", op_pfx0, 1, 0, false, am_imm, 0, 0, 0 },
{ "pfx1", op_pfx1, 1, 0, false, am_imm, 0, 0, 0 },
{ "pfx2", op_pfx2, 1, 0, false, am_imm, 0, 0, 0 },
{ "pfx3", op_pfx3, 1, 0, false, am_imm, 0, 0, 0 },
{ "phi", op_phi },
{ "pldo", op_pldo,4,1,true,am_reg,am_mem,0,0 },
{ "pldt", op_pldt,4,1,true,am_reg,am_mem,0,0 },
{ "pldw", op_pldw,4,1,true,am_reg,am_mem,0,0 },
{ "pmul", op_pmul, 8, 1, false, am_reg, am_reg, am_reg, 0 },
{ "pop", op_pop,4,2,true,am_reg | amCrReg | amCrgReg,am_reg,0,0 },
{ "popf", op_popf,4,2,true,am_reg,am_reg,0,0 },
{ "popm", op_popm,4,1,true,am_imm,0,0,0 },
{ "psto", op_psto,4,1,true,am_reg,am_mem,0,0 },
{ "pstt", op_pstt,4,1,true,am_reg,am_mem,0,0 },
{ "pstw", op_pstw,4,1,true,am_reg,am_mem,0,0 },
{ "psub", op_psub, 6, 1, false, am_reg, am_reg, am_reg, 0 },
{ "ptrdif",op_ptrdif,1,1,false,am_reg,am_reg,am_reg,am_imm },
{ "push",op_push,4,1,true,am_reg | amCrReg | amCrgReg | am_imm,am_reg,0,0 },
{ "pushf",op_pushf,4,0,true,am_reg,0,0,0 },
{ "pushm", op_pushm,4,0,true,am_imm,0,0,0 },
{ "redor", op_redor,2,1,false,am_reg,am_reg,am_reg,0 },
{ "reg2crg", op_reg2crg,1,0,false,amCrReg,am_reg,0,0 },
{ "regs", op_reglist,1,1,false,am_imm,0,0,0 },
{ "rem", op_rem,68,1,false,am_reg,am_reg,am_reg | am_imm,0 },
{ "remu",op_remu,68,1,false,am_reg,am_reg,am_reg | am_imm,0 },
{ "ret", op_ret,1,0,false,am_imm,0,0,0 },
{ "retd", op_retd,1,0,false,am_reg,am_reg,am_reg,am_reg | am_imm },
{ "rol", op_rol,2,1,false,am_reg,am_reg,am_reg | am_ui6,0 },
{ "ror", op_ror,2,1,false,am_reg,am_reg,am_reg | am_ui6,0 },
{ "rte", op_rte,2,0 },
{ "rti", op_rti,2,0 },
{ "rtl", op_rtl,1,0,false,am_imm,0,0,0 },
{ "rts", op_rts,1,0,false,am_imm,0,0,0 },
{ "rtx", op_rtx,1,0,0,0,0,0 },
{ "sand",op_sand,1,1,false,am_reg,am_reg,am_reg | am_imm,0 },
{ "sb",op_sb,4,0,true,am_reg,am_mem,0,0 },
{ "sbx",op_sbx,1,1,false,am_reg,am_reg,am_imm,am_imm },
{ "sd",op_sd,4,0,true,am_reg,am_mem,0,0 },
{ "sei", op_sei,1,0,false,am_reg | am_ui6,0,0,0 },
{ "sh",op_sh,4,0,true,am_reg,am_mem,0,0 },
{ "shl", op_shl,2,1,false,am_reg,am_reg,am_reg | am_ui6,0 },
{ "shlu", op_stplu,2,1,false,am_reg,am_reg,am_reg | am_ui6,0 },
{ "shr", op_shr,2,1,false,am_reg,am_reg,am_reg | am_ui6,0 },
{ "shru", op_stpru,2,1,false,am_reg,am_reg,am_reg | am_ui6,0 },
{ "sll", op_sll,2,1,false,am_reg,am_reg,am_reg,0 },
{ "sllh", op_sllh,2,1,false,am_reg,am_reg,am_reg,0 },
{ "sllp", op_sllp,2,1,false,am_reg,am_reg,am_reg,am_reg | am_ui6 },
{ "sm",op_sm },
{ "sor",op_sor,1,1,false,am_reg,am_reg,am_reg | am_imm,0 },
{ "spt", op_spt,4,0,true ,am_reg,am_mem,0,0 },
{ "sptr", op_sptr,4,0,true,am_reg,am_mem,0,0 },
{ "sra", op_sra,2,1,false,am_reg,am_reg,am_reg | am_ui6,0 },
{ "sret", op_sret,1,0,0,0,0,0 },
{ "srl", op_srl,2,1,false,am_reg,am_reg,am_reg | am_ui6,0 },
{ "stb",op_stb,4,0,true,am_reg,am_mem,0,0 },
{ "std", op_std,4,0,true,am_reg,am_mem,0,0 },
{ "stdcr", op_stdc,4,0,true, am_reg, am_mem,0,0 },
{ "stfd", op_stfd,4,0,true, am_reg, am_mem,0,0 },
{ "stft", op_stft,4,0,true, am_reg, am_mem,0,0 },
{ "sth", op_sth,4,0,true,am_reg,am_mem,0,0 },
{ "sths",op_sths,4,0,true,am_reg,am_mem,0,0 },
{ "sti", op_sti,1,0 },
{ "stib",op_stib,4,0,true,am_imm,am_mem,0,0 },
{ "stio",op_stio,4,0,true,am_imm,am_mem,0,0 },
{ "stit",op_stit,4,0,true,am_imm,am_mem,0,0 },
{ "stiw",op_stiw,4,0,true,am_imm,am_mem,0,0 },
{ "stm", op_stm,20,1,true,am_mem,0,0,0 },
{ "sto",op_sto,4,0,true,am_reg,am_mem,0,0 },
{ "stop", op_stop },
{ "store",op_store,4,0,true,am_reg,am_mem,0,0 },
{ "storem", op_storem,32,0,true,am_imm,am_mem,0,0 },
{ "storev", op_storev,4,1, true, am_vreg, am_mem,0,0 },
{ "stos",op_stos,4,0,true,am_reg,am_mem,0,0 },
{ "stp",op_stp,4,0,true,am_reg,am_mem,0,0 },
{ "stt",op_stt,4,0,true,am_reg,am_mem,0,0 },
{ "stw",op_stw,4,0,true,am_reg,am_mem,0,0 },
{ "sub",op_sub,1,1,false,am_reg,am_reg,am_reg | am_imm,0 },
{ "subs",op_subs,1,1,false,am_reg,am_reg,am_reg, 0 },
{ "subtract",op_subtract,1,1,false,am_reg,am_reg,am_reg | am_imm,0 },
{ "subu", op_subu,1,1 },
{ "sv", op_sv,256,0 },
{ "sw",op_sw,4,0,true,am_reg,am_mem,0,0 },
{ "swap",op_stdap,1,1,false },
{ "swp", op_stdp, 8, false },
{ "sws", op_stds,4,0 },
{ "sxb",op_sxb,1,1,false,am_reg,am_reg,0,0 },
{ "sxc",op_sxc,1,1,false,am_reg,am_reg,0,0 },
{ "sxh",op_sxh,1,1,false,am_reg,am_reg,0,0 },
{ "sxo",op_sxo,1,1,false,am_reg,am_reg,0,0 },
{ "sxp",op_sxp,1,1,false,am_reg,am_reg,0,0 },
{ "sxt",op_sxt,1,1,false,am_reg,am_reg,0,0 },
{ "sxw",op_sxw,1,1,false,am_reg,am_reg,0,0 },
{ "sync",op_sync,1,0,false,am_imm,0,0,0 },
{ "tgt", op_calltgt,1 },
{ "tst",op_tst,1,1 },
{ "vadd", op_vadd,10,1,false, am_vreg,am_vreg,am_vreg,0 },
{ "vadds", op_vadds,1,1,false,am_vreg,am_vreg | am_reg,am_reg,am_vmreg },
{ "vdiv", op_vdiv,100,1,false, am_vreg,am_vreg,am_vreg,0 },
{ "vdivs", op_vdivs,100 },
{ "veins",op_veins,10 },
{ "ver", op_verbatium,0,1,false, 0,0,0,0 },
{ "vex", op_vex,10 },
{ "vfadd", op_vfadd,10,1,false, am_vreg,am_vreg,am_vreg,0 },
{ "vfadds", op_vfadds,10,1,false, am_vreg, am_vreg, am_reg,0 },
{ "vfmul", op_vfmul,10,1,false, am_vreg,am_vreg,am_vreg,0 },
{ "vfmuls", op_vfmuls,10,1,false, am_vreg, am_vreg, am_reg,0 },
{ "vmask", op_vmask,1,1,false, am_reg,am_reg,am_reg,am_reg },
{ "vmul", op_vmul,10,1,false, am_vreg,am_vreg,am_vreg,0 },
{ "vmuls", op_vmuls,10 },
{ "vseq", op_vseq,10,1,false, am_vreg,am_vreg,am_vreg,0 },
{ "vsge", op_vsge,10,1,false, am_vreg,am_vreg,am_vreg,0 },
{ "vsgt", op_vsgt,10,1,false, am_vreg,am_vreg,am_vreg,0 },
{ "vsle", op_vsle,10,1,false, am_vreg,am_vreg,am_vreg,0 },
{ "vslt", op_vslt,10,1,false, am_vreg,am_vreg,am_vreg,0 },
{ "vsne", op_vsne,10,1,false, am_vreg,am_vreg,am_vreg,0 },
{ "vsub", op_vsub,10,1,false, am_vreg,am_vreg,am_vreg,0 },
{ "vsubs", op_vsubs,10 },
{ "wydendx", op_wydendx,1,1,false,am_reg,am_reg,am_reg | am_imm,0 },
{ "xnor",op_xnor,1,1,false,am_reg,am_reg,am_reg,0 },
{ "xor",op_xor,1,1,false,am_reg,am_reg,am_reg | am_imm,0 },
{ "zseq", op_zseq,1,1,false,am_reg,am_reg,am_reg | am_i16,am_reg | am_imm },
{ "zsge",op_zsge,1,1,false,am_reg,am_reg,am_reg | am_i16,am_reg | am_imm },
{ "zsgeu",op_zsgeu,1,1,false,am_reg,am_reg,am_reg | am_i16,am_reg | am_imm },
{ "zsgt",op_zsgt,1,1,false,am_reg,am_reg,am_reg | am_i16,am_reg | am_imm },
{ "zsgtu",op_zsgtu,1,1,false,am_reg,am_reg,am_reg | am_i16,am_reg | am_imm },
{ "zsle",op_zsle,1,1,false,am_reg,am_reg,am_reg | am_i16,am_reg | am_imm },
{ "zsleu",op_zsleu,1,1,false,am_reg,am_reg,am_reg | am_i16,am_reg | am_imm },
{ "zslt", op_zslt,1,1,false,am_reg,am_reg,am_reg | am_i16,am_reg | am_imm },
{ "zsltu", op_zsltu,1,1,false,am_reg,am_reg,am_reg | am_i16,am_reg | am_imm },
{ "zsne",op_zsne,1,1,false,am_reg,am_reg,am_reg | am_i16,am_reg | am_imm },
{ "zxb",op_zxb,1,1,false,am_reg,am_reg,0,0 },
{ "zxt",op_zxt,1,1,false,am_reg,am_reg,0,0 },
{ "zxw",op_zxo,1,1,false,am_reg,am_reg,0,0 }
};

starkCPU::starkCPU() {
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
	RIimmSize = 32;
	itbl = starkInsnTbl;
	itbl_cnt = sizeof(starkInsnTbl) / sizeof(Instruction);
}

void starkCPU::InitRegs()
{
	int ii;

	for (ii = 0; ii < 96; ii++) {
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
		else if (ii < 88)
			cpu.regs[ii].cls = rc_ccr;
		else if (ii == 88)
			cpu.regs[ii].cls = rc_ccrg;
	}

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
	cpu.tmpregs[9] = 18;

	// These are saved regs.
	cpu.tmpregs[10] = 19;
	cpu.tmpregs[11] = 20;
	cpu.tmpregs[12] = 21;
	cpu.tmpregs[13] = 22;
	cpu.tmpregs[14] = 23;
	cpu.tmpregs[15] = 24;
	cpu.tmpregs[16] = 25;
	cpu.tmpregs[17] = 26;
	cpu.tmpregs[18] = 27;

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
	cpu.regs[19].prevreg = 27;
	cpu.regs[19].nextreg = 20;
	cpu.regs[20].isSaved = true;
	cpu.regs[20].number = 20;
	cpu.regs[20].prevreg = 19;
	cpu.regs[20].nextreg = 21;
	cpu.regs[21].isSaved = true;
	cpu.regs[21].number = 21;
	cpu.regs[21].prevreg = 20;
	cpu.regs[21].nextreg = 22;
	cpu.regs[22].isSaved = true;
	cpu.regs[22].number = 2;
	cpu.regs[22].prevreg = 21;
	cpu.regs[22].nextreg = 23;
	cpu.regs[23].isSaved = true;
	cpu.regs[23].number = 23;
	cpu.regs[23].prevreg = 22;
	cpu.regs[23].nextreg = 24;
	cpu.regs[24].isSaved = true;
	cpu.regs[24].number = 24;
	cpu.regs[24].prevreg = 23;
	cpu.regs[24].nextreg = 25;
	cpu.regs[25].isSaved = true;
	cpu.regs[25].number = 25;
	cpu.regs[25].prevreg = 24;
	cpu.regs[25].nextreg = 26;
	cpu.regs[26].isSaved = true;
	cpu.regs[26].number = 26;
	cpu.regs[26].prevreg = 25;
	cpu.regs[26].nextreg = 27;
	cpu.regs[27].isSaved = true;
	cpu.regs[27].number = 27;
	cpu.regs[27].prevreg = 26;
	cpu.regs[27].nextreg = 19;
	cpu.regs[80].isCr = true;
	cpu.regs[80].number = 80;
	cpu.regs[80].nextreg = 81;
	cpu.regs[80].prevreg = 87;
	cpu.regs[81].isCr = true;
	cpu.regs[81].number = 81;
	cpu.regs[81].nextreg = 82;
	cpu.regs[81].prevreg = 80;
	cpu.regs[82].isCr = true;
	cpu.regs[82].number = 82;
	cpu.regs[82].nextreg = 83;
	cpu.regs[82].prevreg = 81;
	cpu.regs[82].isTemp = true;
	cpu.regs[82].isFirstTemp = true;
	cpu.regs[83].isCr = true;
	cpu.regs[83].number = 83;
	cpu.regs[83].nextreg = 84;
	cpu.regs[83].prevreg = 82;
	cpu.regs[83].isTemp = true;
	cpu.regs[84].isCr = true;
	cpu.regs[84].number = 84;
	cpu.regs[84].nextreg = 85;
	cpu.regs[84].prevreg = 83;
	cpu.regs[84].isTemp = true;
	cpu.regs[85].isCr = true;
	cpu.regs[85].number = 85;
	cpu.regs[85].nextreg = 86;
	cpu.regs[85].prevreg = 84;
	cpu.regs[85].isTemp = true;
	cpu.regs[85].isLastTemp = true;
	cpu.regs[86].isCr = true;
	cpu.regs[86].number = 86;
	cpu.regs[87].isCr = true;
	cpu.regs[87].number = 87;
	cpu.regs[88].isCrg = true;
	cpu.regs[88].number = 40;

	cpu.NumSavedRegs = 9;
	cpu.saved_regs[0] = 19;
	cpu.saved_regs[1] = 20;
	cpu.saved_regs[2] = 21;
	cpu.saved_regs[3] = 22;
	cpu.saved_regs[4] = 23;
	cpu.saved_regs[5] = 24;
	cpu.saved_regs[6] = 25;
	cpu.saved_regs[7] = 26;
	cpu.saved_regs[8] = 27;
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

}

char* starkCPU::RegMoniker(int32_t regno)
{
	static char buf[4][20];
	static int n;
	int rg;
	bool invert = false;
	bool vector = false;
	bool group = false;
	bool is_float = false;
	bool is_cr = false;
	bool is_crg = false;

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
	if (regno == 40)
		is_crg = true;
	if (regs[regno].isCr) {
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
	if (is_crg) {
		sprintf_s(&buf[n][0], 20, "%%crg");
	}
	else if (is_cr) {
		sprintf_s(&buf[n][0], 20, "%%cr%d", regno & 7);
	}
	else if (rg = IsTempReg(regno)) {
		sprintf_s(&buf[n][0], 20, "%%t%d", rg - 1);// tmpregs[rg - 1]);
	}
	else if (rg = IsArgReg(regno)) {
		sprintf_s(&buf[n][0], 20, "%%a%d", rg - 1);// tmpregs[rg - 1]);
	}
	else if (rg = IsSavedReg(regno)) {
		sprintf_s(&buf[n][0], 20, "%%s%d", rg - 1);
	}
	else
		if (regno == regFP)
			sprintf_s(&buf[n][0], 20, "%%fp");
	//		else if (regno == regAFP)
	//			sprintf_s(&buf[n][0], 20, "$afp");
		else if (regno == regGP)
			sprintf_s(&buf[n][0], 20, "%%gp");
		else if (regno == regGP1)
			sprintf_s(&buf[n][0], 20, "%%gp1");
	//	else if (regno==regPC)
//		sprintf_s(&buf[n][0], 20, "$pc");
		else if (regno == regSP)
			sprintf_s(&buf[n][0], 20, "%%sp");
		else if (regno == regLR)
			sprintf_s(&buf[n][0], 20, "%%lr0");
		else if (regno == regLR + 1)
			sprintf_s(&buf[n][0], 20, "%%lr1");
		else if (regno == regLR + 2)
			sprintf_s(&buf[n][0], 20, "%%lr2");
		else if (regno == regLR + 3)
			sprintf_s(&buf[n][0], 20, "%%lr3");
		else if (regno == 0) {
			sprintf_s(&buf[n][0], 20, "%%r%d", regno);
		}
		else if (regno == 2)
			sprintf_s(&buf[n][0], 20, "%%r%d", regno);
		else {
			if ((regno & 0x70) == 0x040)
				sprintf_s(&buf[n][0], 20, "$p%d", regno & 0x1f);
			else
				sprintf_s(&buf[n][0], 20, "%%r%d", regno);
		}
	return &buf[n][0];
}

void starkCodeGenerator::banner()
{
	printf("stark Code Generator v0.03\n");
};


void starkCodeGenerator::SignExtendBitfield(Operand* ap3, uint64_t mask)
{
	Operand* ap2;
	uint64_t umask;

	umask = 0x8000000000000000LL | ~(mask >> 1);
	ap2 = GetTempRegister();
	GenerateDiadic(cpu.ldi_op, 0, ap2, cg.MakeImmediate((int64_t)umask));
	GenerateTriadic(op_add, 0, ap3, ap3, ap2);
	GenerateTriadic(op_xor, 0, ap3, ap3, ap2);
	ReleaseTempRegister(ap2);
}

// Convert a value to a Boolean.
Operand* starkCodeGenerator::MakeBoolean(Operand* ap)
{
	Operand* ap1;
	OCODE* ip;

	ap1 = GetTempRegister();
	ip = currentFn->pl.tail;
	if (ip->opcode & 0x8000)
		return (ap1);
	GenerateTriadic(op_cmp, 0, ap1, ap, MakeImmediate(0));
	Generate4adic(op_extu, 0, ap1, ap1, MakeImmediate(1), MakeImmediate(0));
	ap1->isBool = true;
	return (ap1);
}

void starkCodeGenerator::GenerateLea(Operand* ap1, Operand* ap2)
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

Operand* starkCodeGenerator::GenerateSafeLand(ENODE *node, int flags, int op)
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


void starkCodeGenerator::GenerateBitfieldInsert(Operand* ap1, Operand* ap2, int offset, int width)
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


void starkCodeGenerator::GenerateBitfieldInsert(Operand* ap1, Operand* ap2, Operand* offset, Operand* width)
{
	Operand* op_begin;
	Operand* op_end;

	ConvertOffsetWidthToBeginEnd(offset, width, &op_begin, &op_end);
	Generate4adic(op_dep, 0, ap1, ap2, op_begin, op_end);
	ReleaseTempReg(op_end);
	ReleaseTempReg(op_begin);
}


void starkCodeGenerator::GenerateBitfieldInsert(Operand* ap1, Operand* ap2, ENODE* offset, ENODE* width)
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

void starkCodeGenerator::ConvertOffsetWidthToBeginEnd(Operand* offset, Operand* width, Operand** op_begin, Operand** op_end)
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

Operand* starkCodeGenerator::GenerateBitfieldExtract(Operand* ap, Operand* offset, Operand* width)
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

Operand* starkCodeGenerator::GenerateBitfieldExtract(Operand* ap, ENODE* offset, ENODE* width)
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

Operand* starkCodeGenerator::GenerateEq(ENODE *node)
{
	Operand* ap1, * ap2, * ap3;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempCrRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_seq)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_seq)->amclass3, node->p[1]->GetNaturalSize(), 1);
	GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	return (ap3);
}

Operand* starkCodeGenerator::GenerateNe(ENODE* node)
{
	Operand* ap1, * ap2, * ap3;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_sne)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_sne)->amclass3, node->p[1]->GetNaturalSize(), 1);
	if (ap2->mode == am_i16 || ap2->mode==am_imm)
		GenerateTriadic(op_sne, 0, ap3, ap1, ap2);
	else
		Generate4adic(op_sne, 0, ap3, ap1, ap2, MakeImmediate(1));
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	return (ap3);
}

Operand* starkCodeGenerator::GenerateLt(ENODE* node)
{
	Operand* ap1, * ap2, * ap3;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_slt)->amclass3, node->p[1]->GetNaturalSize(), 1);
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_slt)->amclass2, node->p[0]->GetNaturalSize(), 1);
	if (ap2->mode == am_i16 || ap2->mode==am_imm)
		GenerateTriadic(op_slt, 0, ap3, ap1, ap2);
	else
		Generate4adic(op_slt, 0, ap3, ap1, ap2, MakeImmediate(1));
	ReleaseTempRegister(ap1);
	ReleaseTempRegister(ap2);
	return (ap3);
}

Operand* starkCodeGenerator::GenerateLe(ENODE* node)
{
	Operand* ap1, * ap2, * ap3;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_sle)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_sle)->amclass3, node->p[1]->GetNaturalSize(), 1);
	if (ap2->mode == am_i16)
		GenerateTriadic(op_sle, 0, ap3, ap1, ap2);
	else if (ap2->mode == am_imm)
		GenerateTriadic(op_sle, 0, ap3, ap1, ap2);
	else
		Generate4adic(op_sle, 0, ap3, ap1, ap2, MakeImmediate(1));
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	return (ap3);
}

Operand* starkCodeGenerator::GenerateGt(ENODE* node)
{
	Operand* ap1, * ap2, * ap3;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_sgt)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_sgt)->amclass3, node->p[1]->GetNaturalSize(), 1);
	if (ap2->mode == am_i16 || ap2->mode==am_imm)
		GenerateTriadic(op_sgt, 0, ap3, ap1, ap2);
	else
		Generate4adic(op_sgt, 0, ap3, ap1, ap2, MakeImmediate(1));
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	//		GenerateDiadic(op_sgt,0,ap3,ap3);
	return (ap3);
}

Operand* starkCodeGenerator::GenerateGe(ENODE* node)
{
	Operand* ap1, * ap2, * ap3;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_sge)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_sge)->amclass3, node->p[1]->GetNaturalSize(), 1);
	if (ap2->mode == am_i16 || ap2->mode==am_imm)
		GenerateTriadic(op_sge, 0, ap3, ap1, ap2);
	else
		Generate4adic(op_sge, 0, ap3, ap1, ap2, MakeImmediate(1));
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	return (ap3);
}

Operand* starkCodeGenerator::GenerateLtu(ENODE* node)
{
	Operand* ap1, * ap2, * ap3;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_sltu)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_sltu)->amclass3, node->p[1]->GetNaturalSize(), 1);
	if (ap2->mode == am_i16 || ap2->mode == am_imm)
		GenerateTriadic(op_sltu, 0, ap3, ap1, ap2);
	else
		Generate4adic(op_sltu, 0, ap3, ap1, ap2, MakeImmediate(1));
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	return (ap3);
}

Operand* starkCodeGenerator::GenerateLeu(ENODE* node)
{
	Operand* ap1, * ap2, * ap3;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_sleu)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_sleu)->amclass3, node->p[1]->GetNaturalSize(), 1);
	if (ap2->mode == am_i16 || ap2->mode == am_imm)
		GenerateTriadic(op_sleu, 0, ap3, ap1, ap2);
	else
		Generate4adic(op_sleu, 0, ap3, ap1, ap2, MakeImmediate(1));
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	return (ap3);
}

Operand* starkCodeGenerator::GenerateGtu(ENODE* node)
{
	Operand* ap1, * ap2, * ap3;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_sgtu)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_sgtu)->amclass3, node->p[1]->GetNaturalSize(), 1);
	if (ap2->mode == am_i16 || ap2->mode==am_imm)
		GenerateTriadic(op_sgtu, 0, ap3, ap1, ap2);
	else
		Generate4adic(op_sgtu, 0, ap3, ap1, ap2, MakeImmediate(1));
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	//		GenerateDiadic(op_sgt,0,ap3,ap3);
	return (ap3);
}

Operand* starkCodeGenerator::GenerateGeu(ENODE* node)
{
	Operand* ap1, * ap2, * ap3;
	int64_t size;

	size = node->GetNaturalSize();
	ap3 = GetTempRegister();
	ap1 = cg.GenerateExpression(node->p[0], Instruction::Get(op_sgeu)->amclass2, node->p[0]->GetNaturalSize(), 1);
	ap2 = cg.GenerateExpression(node->p[1], Instruction::Get(op_sgeu)->amclass3, node->p[1]->GetNaturalSize(), 1);
	if (ap2->mode == am_i16 || ap2->mode==am_imm)
		GenerateTriadic(op_sgeu, 0, ap3, ap1, ap2);
	else
		Generate4adic(op_sgeu, 0, ap3, ap1, ap2, MakeImmediate(1));
	ReleaseTempRegister(ap2);
	ReleaseTempRegister(ap1);
	return (ap3);
}

Operand* starkCodeGenerator::GenerateFeq(ENODE* node)
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

Operand* starkCodeGenerator::GenerateFne(ENODE* node)
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

Operand* starkCodeGenerator::GenerateFlt(ENODE* node)
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

Operand* starkCodeGenerator::GenerateFle(ENODE* node)
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

Operand* starkCodeGenerator::GenerateFgt(ENODE* node)
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

Operand* starkCodeGenerator::GenerateFge(ENODE* node)
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

Operand *starkCodeGenerator::GenExpr(ENODE *node)
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

void starkCodeGenerator::GenerateBranchTrue(Operand* ap, int64_t label)
{
	gHeadif = currentFn->pl.tail;
	GenerateBne(ap, makereg(regZero), label);
}

void starkCodeGenerator::GenerateBranchFalse(Operand* ap, int64_t label)
{
	gHeadif = currentFn->pl.tail;
	GenerateBeq(ap, makereg(regZero), label);
}

void starkCodeGenerator::GenerateBeq(Operand* ap1, Operand* ap2, int64_t label)
{
	Int128 i;
	OCODE* ip;
	Operand* ap3;

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
			// If comparing to zero, go back an instruction and set the record bit.
			if (Int128::IsEQ(&ap2->offset->i128, Int128::Zero())) {
				ip = currentFn->pl.tail;
				if (ip == nullptr)
					return;
				ip->opcode |= 0x8000;	// Add '.'
				GenerateDiadic(op_beq, 0, makeCrReg(CR0), MakeCodeLabel(label));
				return;
			}
			else {
				ap3 = GetTempRegister();
				GenerateLoadConst(ap2, ap3);
				GenerateTriadic(op_beq, 0, ap1, ap3, MakeCodeLabel(label));
				ReleaseTempRegister(ap3);
				return;
			}
		}
	}
	else {
		// If comparing to zero, go back an instruction and set the record bit.
		if (ap2->preg == 0) {
			ip = currentFn->pl.tail;
			if (ip == nullptr)
				return;
			ip->opcode |= 0x8000;	// Add '.'
			GenerateDiadic(op_beq, 0, makeCrReg(CR0), MakeCodeLabel(label));
			return;
		}
	}
	ap3 = GetTempCrRegister();
	GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
	GenerateDiadic(op_beq, 0, ap3, MakeCodeLabel(label));
	ReleaseTempRegister(ap3);
}

void starkCodeGenerator::GenerateBne(Operand* ap1, Operand* ap2, int64_t label)
{
	Int128 i;
	OCODE* ip;
	Operand* ap3;

	i = regs[ap1->preg].val128;
	if (ap2->mode == am_imm) {
		// Check if branch is always false. If the branch is always false, do nothing.
		// If the branch is always true, generate unconditional branch. Unconditional
		// branches are faster than conditional ones.
		if (regs[ap1->preg].isConst) {
			if (Int128::IsEQ(&i, &ap2->offset->i128))
				;
			else
				GenerateMonadic(op_b, 0, MakeCodeLabel(label));
		}
		else {
			ap3 = GetTempCrRegister();
			GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
			GenerateDiadic(op_bne, 0, ap3, MakeCodeLabel(label));
			ReleaseTempRegister(ap3);
			return;
		}
	}
	else {
		// If comparing to zero, go back an instruction and set the record bit.
		if (ap2->preg == 0) {
			ip = currentFn->pl.tail;
			if (ip == nullptr)
				return;
			ip->opcode |= 0x8000;	// Add '.'
			GenerateDiadic(op_bne, 0, makeCrReg(CR0), MakeCodeLabel(label));
			return;
		}
	}
	ap3 = GetTempCrRegister();
	GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
	GenerateDiadic(op_bne, 0, ap3, MakeCodeLabel(label));
	ReleaseTempRegister(ap3);
}

void starkCodeGenerator::GenerateBlt(Operand* ap1, Operand* ap2, int64_t label)
{
	Int128 i;
	OCODE* ip;

	i = regs[ap1->preg].val128;
	if (ap2->mode == am_imm) {
		// Check if branch is always false. If the branch is always false, do nothing.
		// If the branch is always true, generate unconditional branch. Unconditional
		// branches are faster than conditional ones.
		if (regs[ap1->preg].isConst) {
			if (!Int128::IsLT(&i, &ap2->offset->i128))
				;
			else
				GenerateMonadic(op_b, 0, MakeCodeLabel(label));
		}
		else {
			Operand* ap3 = GetTempCrRegister();
			GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
			GenerateDiadic(op_blt, 0, ap3, MakeCodeLabel(label));
			ReleaseTempRegister(ap3);
		}
	}
	else {
		// If comparing to zero, go back an instruction and set the record bit.
		if (ap2->preg == 0) {
			ip = currentFn->pl.tail;
			if (ip == nullptr)
				return;
			ip->opcode |= 0x8000;	// Add '.'
			GenerateDiadic(op_blt, 0, makeCrReg(CR0), MakeCodeLabel(label));
			return;
		}
		Operand* ap3 = GetTempCrRegister();
		GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
		GenerateDiadic(op_blt, 0, ap3, MakeCodeLabel(label));
		ReleaseTempRegister(ap3);
	}
}

void starkCodeGenerator::GenerateBge(Operand* ap1, Operand* ap2, int64_t label)
{
	OCODE* ip;

	if (ap2->mode == am_imm) {
		Int128 i;

		i = regs[ap1->preg].val128;
		// Check if branch is always false. If the branch is always false, do nothing.
		if (regs[ap1->preg].isConst && !Int128::IsGE(&i, &ap2->offset->i128))
			;
		else {
			Operand* ap3 = GetTempCrRegister();
			GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
			GenerateDiadic(op_bge, 0, ap3, MakeCodeLabel(label));
			ReleaseTempRegister(ap3);
		}
	}
	else {
		// If comparing to zero, go back an instruction and set the record bit.
		if (ap2->preg == 0) {
			ip = currentFn->pl.tail;
			if (ip == nullptr)
				return;
			ip->opcode |= 0x8000;	// Add '.'
			GenerateDiadic(op_bge, 0, makeCrReg(CR0), MakeCodeLabel(label));
			return;
		}
		Operand* ap3 = GetTempCrRegister();
		GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
		GenerateDiadic(op_bge, 0, ap3, MakeCodeLabel(label));
		ReleaseTempRegister(ap3);
	}
}

void starkCodeGenerator::GenerateBle(Operand* ap1, Operand* ap2, int64_t label)
{
	Int128 i;
	OCODE* ip;

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
			Operand* ap3 = GetTempRegister();
			GenerateLoadConst(ap2, ap3);
			GenerateTriadic(op_ble, 0, ap1, ap3, MakeCodeLabel(label));
			ReleaseTempRegister(ap3);
		}
	}
	else {
		// If comparing to zero, go back an instruction and set the record bit.
		if (ap2->preg == 0) {
			ip = currentFn->pl.tail;
			if (ip == nullptr)
				return;
			ip->opcode |= 0x8000;	// Add '.'
			GenerateDiadic(op_bge, 0, makeCrReg(CR0), MakeCodeLabel(label));
			return;
		}
		Operand* ap3 = GetTempCrRegister();
		GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
		GenerateDiadic(op_ble, 0, ap3, MakeCodeLabel(label));
		ReleaseTempRegister(ap3);
	}
}

void starkCodeGenerator::GenerateBgt(Operand* ap1, Operand* ap2, int64_t label)
{
	Int128 i;
	OCODE* ip;

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
			Operand* ap3 = GetTempRegister();
			GenerateLoadConst(ap2, ap3);
			GenerateTriadic(op_bgt, 0, ap1, ap3, MakeCodeLabel(label));
			ReleaseTempRegister(ap3);
		}
	}
	else {
		// If comparing to zero, go back an instruction and set the record bit.
		if (ap2->preg == 0) {
			ip = currentFn->pl.tail;
			if (ip == nullptr)
				return;
			ip->opcode |= 0x8000;	// Add '.'
			GenerateDiadic(op_bge, 0, makeCrReg(CR0), MakeCodeLabel(label));
			return;
		}
		Operand* ap3 = GetTempCrRegister();
		GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
		GenerateDiadic(op_bgt, 0, ap3, MakeCodeLabel(label));
		ReleaseTempRegister(ap3);
	}
}

void starkCodeGenerator::GenerateBltu(Operand* ap1, Operand* ap2, int64_t label)
{
	Int128 i;
	OCODE* ip;

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
			Operand* ap3 = GetTempRegister();
			GenerateLoadConst(ap2, ap3);
			GenerateTriadic(op_bltu, 0, ap1, ap3, MakeCodeLabel(label));
			ReleaseTempRegister(ap3);
		}
	}
	else {
		// If comparing to zero, go back an instruction and set the record bit.
		if (ap2->preg == 0) {
			ip = currentFn->pl.tail;
			if (ip == nullptr)
				return;
			ip->opcode |= 0x8000;	// Add '.'
			GenerateDiadic(op_blt, 0, makeCrReg(CR0), MakeCodeLabel(label));
			return;
		}
		Operand* ap3 = GetTempCrRegister();
		GenerateTriadic(op_cmpa, 0, ap3, ap1, ap2);
		GenerateDiadic(op_blt, 0, ap3, MakeCodeLabel(label));
		ReleaseTempRegister(ap3);
	}
}

void starkCodeGenerator::GenerateBgeu(Operand* ap1, Operand* ap2, int64_t label)
{
	Int128 i;
	OCODE* ip;

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
			Operand* ap3 = GetTempRegister();
			GenerateLoadConst(ap2, ap3);
			GenerateTriadic(op_bgeu, 0, ap1, ap3, MakeCodeLabel(label));
			ReleaseTempRegister(ap3);
		}
	}
	else {
		// If comparing to zero, go back an instruction and set the record bit.
		if (ap2->preg == 0) {
			ip = currentFn->pl.tail;
			if (ip == nullptr)
				return;
			ip->opcode |= 0x8000;	// Add '.'
			GenerateDiadic(op_bge, 0, makeCrReg(CR0), MakeCodeLabel(label));
			return;
		}
		Operand* ap3 = GetTempCrRegister();
		GenerateTriadic(op_cmpa, 0, ap3, ap1, ap2);
		GenerateDiadic(op_bge, 0, ap3, MakeCodeLabel(label));
		ReleaseTempRegister(ap3);
	}
}

void starkCodeGenerator::GenerateBleu(Operand* ap1, Operand* ap2, int64_t label)
{
	Int128 i;
	OCODE* ip;

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
			Operand* ap3 = GetTempRegister();
			GenerateLoadConst(ap2, ap3);
			GenerateTriadic(op_bleu, 0, ap1, ap3, MakeCodeLabel(label));
			ReleaseTempRegister(ap3);
		}
	}
	else {
		// If comparing to zero, go back an instruction and set the record bit.
		if (ap2->preg == 0) {
			ip = currentFn->pl.tail;
			if (ip == nullptr)
				return;
			ip->opcode |= 0x8000;	// Add '.'
			GenerateDiadic(op_ble, 0, makeCrReg(CR0), MakeCodeLabel(label));
			return;
		}
		Operand* ap3 = GetTempCrRegister();
		GenerateTriadic(op_cmpa, 0, ap3, ap1, ap2);
		GenerateDiadic(op_ble, 0, ap3, MakeCodeLabel(label));
		ReleaseTempRegister(ap3);

	}
}

void starkCodeGenerator::GenerateBgtu(Operand* ap1, Operand* ap2, int64_t label)
{
	Int128 i;
	OCODE* ip;

	i = regs[ap1->preg].val128;
	if (ap2->mode == am_imm) {
		// Check if branch is always false. If the branch is always false, do nothing.
		// If the branch is always true, generate unconditional branch. Unconditional
		// branches are faster than conditional ones.
		if (regs[ap1->preg].isConst) {
			if (Int128::IsULE(&i, &ap2->offset->i128))
				;
			else
				GenerateMonadic(op_b, 0, MakeCodeLabel(label));
		}
		else {
			Operand* ap3 = GetTempRegister();
			GenerateLoadConst(ap2, ap3);
			GenerateTriadic(op_bgtu, 0, ap1, ap3, MakeCodeLabel(label));
			ReleaseTempRegister(ap3);
		}
	}
	else {
		// If comparing to zero, go back an instruction and set the record bit.
		if (ap2->preg == 0) {
			ip = currentFn->pl.tail;
			if (ip == nullptr)
				return;
			ip->opcode |= 0x8000;	// Add '.'
			GenerateDiadic(op_bgt, 0, makeCrReg(CR0), MakeCodeLabel(label));
			return;
		}
		Operand* ap3 = GetTempCrRegister();
		GenerateTriadic(op_cmpa, 0, ap3, ap1, ap2);
		GenerateDiadic(op_bgt, 0, ap3, MakeCodeLabel(label));
		ReleaseTempRegister(ap3);

	}
}

void starkCodeGenerator::GenerateBand(Operand* ap1, Operand* ap2, int64_t label)
{
	Operand* ap3;

	ap3 = GetTempCrRegister();
	GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
	GenerateDiadic(op_band, 0, ap3, MakeCodeLabel(label));
	ReleaseTempReg(ap3);
}

void starkCodeGenerator::GenerateBor(Operand* ap1, Operand* ap2, int64_t label)
{
	Operand* ap3;

	ap3 = GetTempCrRegister();
	GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
	GenerateDiadic(op_bor, 0, ap3, MakeCodeLabel(label));
	ReleaseTempReg(ap3);
}

void starkCodeGenerator::GenerateBnand(Operand* ap1, Operand* ap2, int64_t label)
{
	Operand* ap3;

	ap3 = GetTempCrRegister();
	GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
	GenerateDiadic(op_bnand, 0, ap3, MakeCodeLabel(label));
	ReleaseTempReg(ap3);
}

void starkCodeGenerator::GenerateBnor(Operand* ap1, Operand* ap2, int64_t label)
{
	Operand* ap3;

	ap3 = GetTempCrRegister();
	GenerateTriadic(op_cmp, 0, ap3, ap1, ap2);
	GenerateDiadic(op_bnor, 0, ap3, MakeCodeLabel(label));
	ReleaseTempReg(ap3);
}

bool starkCodeGenerator::GenerateBranch(ENODE *node, int op, int64_t label, int predreg, unsigned int prediction, bool limit)
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

void starkCodeGenerator::SaveRegisterVars(CSet *rmask)
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
#ifdef STARK
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
#endif

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
int64_t starkCodeGenerator::PushArgument(ENODE *ep, int regno, int stkoffs, bool *isFloat, int* push_count, bool large_argcount)
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
					GenerateDiadic(cpu.ldi_op,0,makereg(regno & 0x7fff), ap);
					if (regno & 0x8000) {
						GenerateTriadic(op_sub,0,makereg(regSP),makereg(regSP),MakeImmediate(cpu.sizeOfWord));
						nn = 1;
					}
				}
				else if (ap->mode==am_fpreg) {
					*isFloat = true;
					GenerateDiadic(cpu.mov_op,0,makefpreg(regno & 0x7fff), ap);
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
							ap3 = GetTempRegister();
							regs[ap3->preg].IsArg = true;
							GenerateLoadConst(ap, ap3);
	         		cg.GenerateStore(ap3,MakeIndexed(stkoffs,regSP),cpu.sizeOfWord);
							ReleaseTempReg(ap3);
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
int64_t starkCodeGenerator::PushArguments(Function *sym, ENODE *plist)
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

void starkCodeGenerator::PopArguments(Function *fnc, int howMany, bool isPascal)
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


void starkCodeGenerator::LinkAutonew(ENODE *node)
{
	if (node->isAutonew) {
		currentFn->hasAutonew = true;
	}
}

void starkCodeGenerator::GenerateDirectJump(ENODE* node, Operand* ap, Function* sym, int flags, int lab)
{
	char buf[500];

	if (sym && sym->IsLeaf) {
		sprintf_s(buf, sizeof(buf), "%s_ip", sym->sym->name->c_str());
		if (flags & am_jmp)
			GenerateMonadic(sym->sym->storage_class == sc_static ? op_b : op_b, 0, MakeIndexed(node->p[0], BR7));
		else
			GenerateMonadic(sym->sym->storage_class == sc_static ? op_bl : op_bl, 0, MakeDirect(node->p[0]));
		currentFn->doesJAL = true;
	}
	else if (sym) {
		if (flags & am_jmp)
			GenerateMonadic(sym->sym->storage_class == sc_static ? op_b : op_b, 0, MakeIndexed(node->p[0], BR7));
		else
			GenerateMonadic(sym->sym->storage_class == sc_static ? op_bl : op_bl, 0, MakeDirect(node->p[0]));
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

void starkCodeGenerator::GenerateIndirectJump(ENODE* node, Operand* ap, Function* sym, int flags, int lab)
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

void starkCodeGenerator::GenerateUnlink(int64_t amt)
{
	if (cpu.SupportsEnter)
		currentFn->PatchEnter(currentFn->mask);
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

void starkStatementGenerator::GenerateTabularSwitch(Statement* stmt, int64_t minv, int64_t maxv, Operand* ap, bool HasDefcase, int deflbl, int tablabel)
{
	Operand* ap2;

	stmt->tabular = true;
	ap2 = GetTempRegister();
	if (minv != 0)
		GenerateTriadic(op_sub, 0, ap, ap, MakeImmediate(minv));
	GenerateTriadic(op_bgeu, 0, ap, MakeImmediate(maxv - minv + 1), MakeCodeLabel(HasDefcase ? deflbl : breaklab));
	ReleaseTempRegister(ap2);
	GenerateTriadic(op_asl, 0, ap, ap, MakeImmediate(2));
	//GenerateDiadic(op_ldt, 0, ap, compiler.of.MakeIndexedCodeLabel(tablabel, ap->preg));
	cg.GenerateLoad(ap, compiler.of.MakeIndexedDataLabel(tablabel, ap->preg), 4, 4);
	GenerateMonadic(op_jmp, 0, MakeIndirect(ap->preg));
	ReleaseTempRegister(ap);
	GenerateSwitchStatements(stmt);
}

void starkStatementGenerator::GenerateNakedTabularSwitch(Statement* stmt, int64_t minv, Operand* ap, int tablabel)
{
	if (minv != 0)
		GenerateTriadic(op_sub, 0, ap, ap, MakeImmediate(minv));
	GenerateTriadic(op_sll, 0, ap, ap, MakeImmediate(2));
	//	GenerateDiadic(cpu.ldo_op, 0, ap, compiler.of.MakeIndexedCodeLabel(tablabel, ap->preg));
//	GenerateDiadic(op_ldt, 0, ap, compiler.of.MakeIndexedName((char*)stmt->GenerateSwitchTargetName(tablabel).c_str(), ap->preg)); // MakeIndexedCodeLabel(tablabel, ap->preg));
	cg.GenerateLoad(ap, compiler.of.MakeIndexedDataLabel(tablabel, ap->preg), 4, 4);
	//cg.GenerateLoad(ap, compiler.of.MakeIndexedName((char*)stmt->GenerateSwitchTargetName(tablabel).c_str(), ap->preg), 4, 4);
	GenerateMonadic(op_jmp, 0, MakeIndirect(ap->preg));
	ReleaseTempRegister(ap);
	GenerateSwitchStatements(stmt);
}

//
// Generate a jump to label if the node passed evaluates to
// a true condition.
//
void starkCodeGenerator::GenerateTrueJump(ENODE* node, int64_t label, unsigned int prediction)
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
				GenerateBne(ap1, makereg(regZero), label);
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
void starkCodeGenerator::GenerateFalseJump(ENODE* node, int64_t label, unsigned int prediction)
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
			GenerateBeq(ap, makereg(regZero), label);
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

void starkCodeGenerator::GenerateLoadFloat(Operand* ap3, Operand* ap1, int64_t ssize, int64_t size, Operand* mask)
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

void starkCodeGenerator::GenerateInterruptSave(Function* func)
{
	int nn, kk;
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
			kk++;
		}
		tsm = tsm >> 1;
	}
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

void starkCodeGenerator::GenerateInterruptLoad(Function* func)
{
	int nn, kk;
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
			GenerateLoad(makereg(nn), MakeIndexed(kk * cpu.sizeOfWord, regSP), cpu.sizeOfWord, cpu.sizeOfWord);
			kk++;
		}
		tsm = tsm >> 1;
	}
	// Deallocate stack storage
	GenerateAddOnto(makereg(regSP), MakeImmediate(kk * cpu.sizeOfWord));
}

void starkCodeGenerator::GenerateLoadConst(Operand* ap1, Operand* ap2)
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
					// ToDo handle constant >64 bits
					/*
					ip = GenerateDiadic(cpu.ldi_op, 0, ap2, MakeImmediate(ap1->offset->i128.low & 0xffffLL));
					if (!ap1->offset->i128.IsNBit(16))
						GenerateMonadic(op_pfx0, 0, MakeImmediate(ap1->offset->i128.low & 0xfffffffffLL));
					if (!ap1->offset->i128.IsNBit(32))
						GenerateMonadic(op_pfx1, 0, MakeImmediate(ap1->offset->i128.low >> 32LL));
					if (!ap1->offset->i128.IsNBit(64))
						GenerateMonadic(op_pfx2, 0, MakeImmediate(ap1->offset->i128.high & 0xffffffffLL));
					if (!ap1->offset->i128.IsNBit(96))
						GenerateMonadic(op_pfx3, 0, MakeImmediate(ap1->offset->i128.high >> 32LL));
					*/
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
		regs[ap2->preg].isConst = false;// true;
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

void starkCodeGenerator::GenerateLoadDataPointer()
{
	Operand* ap = GetTempRegister();

	return;
	cg.GenerateLoadAddress(makereg(regGP), MakeStringAsNameConst((char*)"_start_data", dataseg));
	ReleaseTempRegister(ap);
}

// Compiler now uses global pointer two addressing for the rodataseg
void starkCodeGenerator::GenerateLoadRodataPointer()
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

void starkCodeGenerator::GenerateLoadBssPointer()
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

void starkCodeGenerator::GenerateSmallDataRegDecl()
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

void starkCodeGenerator::GenerateSignExtendByte(Operand* tgt, Operand* src)
{
	Generate4adic(op_ext, 0, tgt, src, MakeImmediate(0), MakeImmediate(7));
}

void starkCodeGenerator::GenerateSignExtendWyde(Operand* tgt, Operand* src)
{
	Generate4adic(op_ext, 0, tgt, src, MakeImmediate(0), MakeImmediate(15));
}

void starkCodeGenerator::GenerateSignExtendTetra(Operand* tgt, Operand* src)
{
	Generate4adic(op_ext, 0, tgt, src, MakeImmediate(0), MakeImmediate(31));
}

void starkCodeGenerator::GenerateReturnAndDeallocate(Operand* ap1)
{
	GenerateDiadic(op_retd, 0, ap1, MakeImmediate(0));
}

void starkCodeGenerator::GenerateReturnAndDeallocate(int64_t amt)
{
	GenerateDiadic(op_retd, 0, MakeImmediate(amt), MakeImmediate(0));
}

void starkCodeGenerator::GenerateLoadAddress(Operand* ap3, Operand* ap1)
{
	Operand* ap2, * ap4;

	if (address_bits > 24)
		ap1->lowhigh = 1;
	GenerateDiadic(op_lda, 0, ap3, ap1);
}

// Generate load or store operation taking into consideration the number of
// address bits required. If too many bits are needed turn the load or store
// into a load of the address into a register, then use the register.

void starkCodeGenerator::GenerateLoadStore(e_op opcode, Operand* ap1, Operand* ap2)
{
	// Only two modes that have issues: direct and register indirect with disp.
	if (ap2->mode == am_indx) {
		if (ap2->preg == 0)
			ap2->mode = am_direct;
	}
	if (ap2->mode == am_direct || ap2->mode==am_direct2) {
		if (ap2->offset == nullptr || ap2->offset2 != nullptr || ap2->mode == am_direct2) {// || !ap2->offset->i128.IsNBit(21)) {
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
		if (ap2->offset == nullptr || ap2->offset2!=nullptr) {
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
	else {
		// Simple register indirect, we know displacement is zero.
		if (ap2->mode == am_ind) {
			if (opcode == op_store)
				opcode = op_stos;
			if (opcode == op_load)
				opcode = op_ldos;
		}
		GenerateDiadic(opcode, 0, ap1, ap2);
	}
}

void starkCodeGenerator::GenerateLoad(Operand* ap3, Operand* ap1, int64_t ssize, int64_t size, Operand* mask)
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

void starkCodeGenerator::GenerateStore(Operand* ap1, Operand* ap3, int64_t size, Operand* mask)
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

Operand* starkCodeGenerator::GenerateFloatcon(ENODE* node, int flags, int64_t size)
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

Operand* starkCodeGenerator::GenPositcon(ENODE* node, int flags, int64_t size)
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

Operand* starkCodeGenerator::GenLabelcon(ENODE* node, int flags, int64_t size)
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

Operand* starkCodeGenerator::GenNacon(ENODE* node, int flags, int64_t size)
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

int starkCodeGenerator::GetSegmentIndexReg(e_sg segment)
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
OCODE* starkCodeGenerator::GenerateReturnBlock(Function* fn)
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
			GenerateDiadic(op_enter, 0, MakeImmediate(15LL), MakeImmediate(-fn->tempbot));
			ip = currentFn->pl.tail;
			//			GenerateMonadic(op_link, 0, MakeImmediate(stkspace));
						//spAdjust = pl.tail;
			fn->alstk = true;
		}
		else {
			GenerateDiadic(op_enter, 0, MakeImmediate(15LL), MakeImmediate(8388600LL));
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
			GenerateTriadic(op_subtract, 0, makereg(regSP), makereg(regSP), MakeImmediate(Compiler::GetReturnBlockSize() + fn->stkspace - 8388600LL));
			//GenerateMonadic(op_link, 0, MakeImmediate(SizeofReturnBlock() * cpu.sizeOfWord));
			fn->alstk = true;
		}
	}
	else {
		GenerateTriadic(op_subtract, 0, makereg(regSP), makereg(regSP), MakeImmediate(Compiler::GetReturnBlockSize()));
		cg.GenerateStore(makereg(regFP), MakeIndirect(regSP), cpu.sizeOfWord);
		cg.GenerateMove(makereg(regFP), makereg(regSP));
		if (!currentFn->IsLeaf) {
			ap2 = GetTempRegister();
			GenerateDiadic(op_move, 0, ap2, makereg(regLR));
			cg.GenerateStore(ap2, cg.MakeIndexed(cpu.sizeOfWord * 1, regFP), cpu.sizeOfWord);	// Store link register on stack
			ReleaseTempRegister(ap2);
		}
		GenerateTriadic(op_subtract, 0, makereg(regSP), makereg(regSP), MakeImmediate(fn->stkspace));
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

Operand* starkCodeGenerator::GenerateLand(ENODE* node, int flags, int op, bool safe)
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

/* Generate code for an immediate 'add' operation. Uses the CPU's shifted and
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
Operand* starkCodeGenerator::GenerateAddImmediate(Operand* dst, Operand* src1, Operand* src2)
{
	Operand* ap5;

	// ToDo: Should spit out a compiler warning here.
	if (src2->offset == nullptr)
		return (dst);

	if (src2->offset->i128.IsNBit(cpu.RIimmSize)) {
		GenerateTriadic(op_add, 0, dst, src1, MakeImmediate(src2->offset->i128.low));
		return (dst);
	}
	ap5 = nullptr;

	if (!src2->offset->i128.IsNBit(24LL)) {
		ap5 = GetTempRegister();
		GenerateLoadConst(src2, ap5);
		GenerateTriadic(op_add, 0, dst, src1, ap5);
		ReleaseTempRegister(ap5);
		return (dst);
	}
	GenerateTriadic(op_add, 0, dst, src1, MakeImmediate(src2->offset->i128.low & 0xffffffLL));
	/* Should update this sometime to use adds. But managing the sign extended constant is tricky.
	if (src2->offset->i128.low & 0x800000LL) {
		if (((src2->offset->i128.low >> 24) & 0xffffffLL) == 0xffffffLL)
			GenerateTriadic(op_ands, 0, dst, MakeImmediate(1LL), MakeImmediate(2LL));
		else
			GenerateTriadic(op_ands, 0, dst, MakeImmediate(((src2->offset->i128.low >> 24LL) & 0xffffffLL) + 1), MakeImmediate(1LL));
	}
	else
		GenerateTriadic(op_ands, 0, dst, MakeImmediate((src2->offset->i >> 24LL) & 0xffffffLL), MakeImmediate(1LL));
	*/
	return (dst);
}

/* Generate code for an immediate 'and' operation. Uses the CPU's shifted and
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
Operand* starkCodeGenerator::GenerateAndImmediate(Operand* dst, Operand* src1, Operand* src2)
{
	// ToDo: Should spit out a compiler warning here.
	if (src2->offset == nullptr)
		return (dst);

	GenerateTriadic(op_and, 0, dst, src1, MakeImmediate(src2->offset->i128.low));
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
Operand* starkCodeGenerator::GenerateOrImmediate(Operand* dst, Operand* src1, Operand* src2)
{
	// ToDo: Should spit out a compiler warning here.
	if (src2->offset == nullptr)
		return (dst);

	GenerateTriadic(op_or, 0, dst, src1, MakeImmediate(src2->offset->i128.low));
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
Operand* starkCodeGenerator::GenerateEorImmediate(Operand* dst, Operand* src1, Operand* src2)
{
	// ToDo: Should spit out a compiler warning here.
	if (src2->offset == nullptr)
		return (dst);

	GenerateTriadic(op_eor, 0, dst, src1, MakeImmediate(src2->offset->i128.low));
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
Operand* starkCodeGenerator::GenerateShift(ENODE* node, int flags, int64_t size, int op)
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
	Generate4adic(op, size == cpu.sizeOfWord ? 0 : (int)size, ap3, ap1, makereg(regZero), ap2);
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

Operand* starkCodeGenerator::GenerateAdd(Operand* dst, Operand* src1, Operand* src2)
{
	if (dst->preg < 32 && src1->preg < 32 && src2->preg < 32)
		GenerateTriadic(op_adds, 0, dst, src1, src2);
	else
		GenerateTriadic(op_add, 0, dst, src1, src2);
	return (dst);
}

Operand* starkCodeGenerator::GenerateSubtract(Operand* dst, Operand* src1, Operand* src2)
{
	/*
	if (dst->preg < 32 && src1->preg < 32 && src2->preg < 32)
		GenerateTriadic(op_subs, 0, dst, src1, src2);
	else
	*/
		GenerateTriadic(op_sub, 0, dst, src1, src2);
	return (dst);
}
