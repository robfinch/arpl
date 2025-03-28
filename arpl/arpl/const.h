#ifndef _CONST_H
#define _CONST_H

// ============================================================================
//        __
//   \\__/ o\    (C) 2012-2025  Robert Finch, Waterloo
//    \  __ /    All rights reserved.
//     \/_//     robfinch<remove>@finitron.ca
//       ||
//
// Arpl - 'C' derived language compiler
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
enum e_cpu { thor, qupls, riscv, bigfoot, i386 };
//#define THOR	1
//#define LB650	1
#define STARK	1
//#define QUPLS	1
//#define QUPLS40 1
//#define RISCV 5
//#define I386 386
//#define BIGFOOT 999

enum e_bt {
	bt_none,
	bt_byte, bt_ubyte, bt_bit, bt_bool, bt_set,
	bt_char, bt_short, bt_int, bt_long, 
	bt_half, bt_single, bt_float, bt_double, bt_triple, bt_quad, bt_decimal, bt_posit, bt_pointer,
	bt_ichar, bt_iuchar, bt_i128,
	bt_uchar, bt_ushort, bt_uint, bt_ulong,
  bt_unsigned, bt_vector, bt_vector_mask,
  bt_aggregate, bt_array, bt_struct, bt_union, bt_class, bt_enum, bt_void,
  bt_func, bt_ifunc, bt_label,
	bt_interrupt, bt_oscall, bt_pascal, bt_kernel, bt_bitfield, bt_ubitfield,
	bt_exception, bt_ellipsis,
  bt_last};

enum e_node {
		en_unknown,
        en_void,        /* used for parameter lists */
				en_cast,				/* typecast */
				en_nop,

		en_list, en_aggregate, en_end_aggregate,

		en_ubyt2wyde,
		en_ubyt2tetra, en_uwyde2tetra,
		en_byt2wyde,
		en_byt2octa, en_wyde2octa, en_tetra2octa,
		en_ubyt2octa, en_uwyde2octa, en_utetra2octa,
		en_cucwp, en_ccwp,
		
		en_byt2hexi, en_wyde2hexi, en_tetra2hexi,
		en_ubyt2hexi, en_uwyde2hexi, en_utetra2hexi,
		en_ubyt2ptr, en_byt2ptr,
		en_uwyde2ptr, en_wyde2ptr,
		en_utetra2ptr, en_tetra2ptr,
		en_uocta2ptr, en_octa2ptr,
		en_uocta2hexi,

    en_byt2tetra,
		en_wyde2tetra,
		en_octa2hexi,

		en_cld, en_cfd,
		en_sxb, en_sxc, en_sxh,
		en_zxb, en_zxc, en_zxh,
		en_i2p, en_p2i,
        en_icon, en_bicon, en_fcon, en_fqcon, en_dcon, en_tcon, en_scon, en_labcon, en_nacon,
				en_autocon, en_autofcon, en_autopcon, en_classcon,
		en_clabcon, en_cnacon,
		en_dlabcon, en_dnacon, // 30<-
		
		     en_fcall, en_ifcall,
         en_type, en_regvar, en_fpregvar, en_pregvar, en_tempfpref, en_temppref,
		en_add, en_sub, en_mul, en_mod, en_scndx,
		en_ftadd, en_ftsub, en_ftmul, en_ftdiv,
		en_fdadd, en_fdsub, en_fdmul, en_fddiv,
		en_fsadd, en_fssub, en_fsmul, en_fsdiv,
		en_fadd, en_fsub, en_fmul, en_fdiv,
		en_padd, en_psub, en_pmul, en_pdiv, en_ptoi, en_itop, en_peq, en_pne, en_plt, en_ple, en_pcon, en_pgt, en_pge,
		en_d2t, en_d2q, en_t2q, en_p2d,
		en_i2d, en_i2q, en_d2i, en_q2i, en_s2q, en_t2i, // 63<-
        en_div, en_asl, en_shl, en_shlu, en_shr, en_shru, en_asr, en_rol, en_ror, en_ext, en_extu,
		en_cond, en_safe_cond, en_assign, 
        en_asadd, en_assub, en_asmul, en_asdiv, en_asdivu, en_asmod, en_asmodu,
				en_asfmul,
		en_asrsh, en_asrshu, en_asmulu, //81
        en_aslsh, en_asand, en_asor, en_asxor, en_uminus, en_not, en_compl,
        en_eq, en_ne, en_lt, en_le, en_gt, en_ge,
        en_feq, en_fne, en_flt, en_fle, en_fgt, en_fge,
        en_veq, en_vne, en_vlt, en_vle, en_vgt, en_vge,
		en_and, en_or, en_land, en_lor, en_land_safe, en_lor_safe, //104
        en_xor, en_mulu, en_udiv, en_umod, en_ugt,
        en_uge, en_ule, en_ult,
		en_and_and, en_or_and, en_and_or, en_or_or,
		en_ref, en_fieldref, en_ursh,
		en_bchk, en_chk, en_bytendx, en_bitoffset,
		en_abs, en_max, en_min, en_addrof, en_ptrdif, en_wydendx,
		en_switch, en_case, en_default, en_save_context,
		en_bmap,
		// Vector
		en_autovcon, en_autovmcon, en_vector_ref, en_vex, en_veins,
		en_vadd, en_vsub, en_vmul, en_vdiv,
		en_vadds, en_vsubs, en_vmuls, en_vdivs,
		en_mulf, en_sync, en_isnullptr, en_pfx0, en_pfx1, en_pfx2, en_pfx3,
		en_object_list
		};

enum e_stmt {
		st_empty, st_funcbody,
        st_expr, st_compound, st_while, 
		st_until, st_forever, st_firstcall, st_asm,
		st_dountil, st_doloop, st_dowhile, st_doonce,
		st_try, st_catch, st_throw, st_critical, st_spinlock, st_spinunlock,
		st_for,
		st_do, st_if, st_else, st_elsif, st_switch, st_default,
        st_case, st_goto, st_break, st_continue, st_label,
        st_return, st_vortex, st_intoff, st_inton, st_stop, st_check, st_yield };

enum e_sym {
	tk_nop,
	id, cconst, iconst, uiconst, lconst, sconst, isconst, asconst, rconst, pconst, plus, minus,
	star, divide, lshift, rshift, lrot, rrot,
	modop, eq, neq, lt, leq, gt,
	geq, assign, asplus, asminus, astimes, asdivide, asmodop,
	aslshift, asrshift, aslrot, asrrot,
	asand, asor, asxor, autoinc, autodec, hook, mux_hook, cmpl,
	comma, colon, semicolon, double_colon, uparrow, openbr, closebr, begin, end,
	openpa, closepa, pointsto, dot, lor, land, nott, bitorr, bitandd, lor_safe, land_safe,
	ellipsis,
	// functions
	kw_abs, kw_max, kw_min, kw_bit,
	kw_coroutine, kw_yield,
	kw_vector, kw_vector_mask,
	kw_int, kw_byte, kw_int8, kw_int16, kw_int32, kw_int40, kw_int64, kw_int80,
	kw_float128, kw_posit, 
	kw_icache, kw_dcache, kw_thread,
	kw_void, kw_char, kw_float, kw_double, kw_triple,
	kw_struct, kw_union, kw_class,
	kw_bool, kw_long, kw_short, kw_unsigned, kw_auto, kw_extern,
	kw_register, kw_typedef, kw_static, kw_goto, kw_return,
	kw_sizeof, kw_break, kw_continue, kw_if, kw_else, kw_elsif,
	kw_for, kw_forever, kw_signed,
	kw_firstcall, kw_asm, kw_fallthru, kw_until, kw_loop,
	kw_try, kw_catch, kw_throw, kw_typenum, kw_const, kw_volatile,
	kw_do, kw_while, kw_switch, kw_case, kw_default, kw_enum,
	kw_interrupt, kw_vortex, kw_pascal, kw_oscall, kw_nocall, kw_naked,
	kw_intoff, kw_inton, kw_then,
	kw_private, kw_public, kw_stop, kw_critical, kw_spinlock, kw_spinunlock, kw_lockfail,
	kw_far, kw_cdecl, kw_align, kw_prolog, kw_epilog, kw_check, kw_exception, kw_task,
	kw_unordered, kw_inline, kw_kernel, kw_inout, kw_leaf, kw_leafs,
	kw_unique, kw_virtual, kw_this,
	kw_new, kw_delete, kw_using, kw_namespace, kw_not, kw_attribute,
	kw_no_temps, kw_no_parms, kw_floatmax, kw_mulf, kw_bytendx, kw_is_nullptr,
	kw_compound, kw_expr, kw_label, kw_restrict,
	kw_nullptr, kw_generic, kw_set, kw_single, kw_quad, kw_half, kw_real, kw_decimal,
	kw_alignof, kw_declspec,
	kw_vm0, kw_vm1, kw_vm2, kw_vm3, kw_vm4, kw_vm5, kw_vm6, kw_vm7,
	kw_user, kw_supervisor, kw_hypervisor, kw_machine,
	// Intrinsics
	kw_bmap, kw_wydendx, kw_save_context, kw_sync,
	my_eof
};

enum e_sc {
	sc_none,
	sc_static, sc_auto, sc_global, sc_thread, sc_external, sc_type, sc_const,
	sc_member, sc_label, sc_ulabel, sc_typedef, sc_register
};

enum e_rc {
	rc_none, rc_int, rc_float, rc_posit, rc_cmp
};

enum e_sg { noseg, codeseg, dataseg, stackseg, bssseg, idataseg, tlsseg, rodataseg, lastseg };

enum e_op {
	op_none,
	op_aslx, op_reglist, op_stm, op_ldm,
	op_move, op_add, op_addu, op_addi, op_asfmul,
	op_bf, op_bt, op_gcsub, op_bal,
	op_sub, op_subi, op_mov, op_mtspr, op_mfspr, op_loadi, op_ld,
	op_mul, op_muli, op_mulu, op_divi, op_modi, op_modui,
	op_div, op_divs, op_divsi, op_divu, op_and, op_andi, op_eor, op_eori,
	op_bit,
	op_or, op_ori, op_xor, op_xori, op_redor,
	op_nand, op_nor, op_xnor,
	op_asr, op_asri, op_stpl, op_stpr, op_stpru, op_ror, op_rol,
	op_stpli, op_stpri, op_stprui, op_stplu, op_stplui, op_rori, op_roli,
	op_lslor,
	op_bfclr, op_bfext, op_bfextu, op_bfins, op_bfset, op_dep,
	op_jmp, op_jsr, op_mului, op_mod, op_modu,
	op_bmi, op_subu, op_lddr, op_stdc, op_loop, op_iret,
	op_sext32, op_sext16, op_sext8, op_sxb, op_sxc, op_sxh, op_sxw, op_sxt, op_sxp, op_sxo,
	op_zxb, op_zxw, op_zxt, op_extr,
	op_dw, op_cache,
	op_subui, op_addui, op_sei,
	op_std, op_sth, op_sto, op_stos, op_stp, op_stt, op_stw, op_stb, op_outb, op_inb, op_inbu,
	op_stfd, op_ldfd,
	op_call, op_jal, op_beqi, op_bnei, op_tst,

	op_beq, op_bne, op_blt, op_ble, op_bgt, op_bge, op_band, op_bor, op_bnand, op_bnor,
	op_bltu, op_bleu, op_bgtu, op_bgeu,
	op_bltui, op_bleui, op_blti, op_blei, op_bgti, op_bgtui, op_bgei, op_bgeui,
	op_bbs, op_bbc, op_beqz, op_bnez,
	op_dbra,

	op_brz, op_brnz, op_br,
	op_ldft, op_stft,
	op_ldd, op_ldh, op_ldo, op_ldos, op_ldp, op_ldw, op_ldb, op_ret, op_sm, op_lm, op_ldis, op_ldds, op_stds,
	op_lvb, op_lvc, op_lvh, op_lvw,
	op_inc, op_dec,
	op_ldbu, op_ldwu, op_ldpu, op_ldou, op_sti,
	op_ldf, op_stf,
	op_rts, op_retd,
	op_push, op_pop, op_movs,
	op_seq, op_sne, op_slt, op_sle, op_sgt, op_sge, op_sltu, op_sleu, op_sgtu, op_sgeu,
	op_sand, op_sor, op_andcm, op_orcm,
	op_branch, op_eq, op_ne, op_lt, op_le, op_gt, op_ge,
	op_feq, op_fne, op_flt, op_fle, op_fgt, op_fge,
	op_gtu, op_geu, op_ltu, op_leu, op_nr,
	op_bhi, op_bhs, op_blo, op_bls, op_ext, op_extu, op_lea, op_stdap,
	op_neg, op_not, op_com, op_cmp, op_clr, op_link, op_unlk, op_label,
	op_pea, op_cmpi, op_dc, op_asm, op_stop, op_fnname,
	op_pfx0, op_pfx1, op_pfx2, op_pfx3,
	// FISA64
	op_ldw0i, op_ldw1i, op_ldw2i, op_ldw3i, op_chk, op_chki,
	op_cmpu, op_bsr, op_bun,
	op_sll, op_sllh, op_slli, op_srl, op_srli, op_sra, op_srai, op_asl, op_lsr, op_asli, op_lsri, op_remark,
	op_sllp,
	// floating point
	op_fbeq, op_fbne, op_fbor, op_fbun, op_fblt, op_fble, op_fbgt, op_fbge,
	op_fcvtsq, op_fcvtdq, op_fcvttq,
	op_fadd, op_fsub, op_fmul, op_fdiv, op_fcmp, op_fneg,
	op_ftmul, op_ftsub, op_ftdiv, op_ftadd, op_ftneg, op_ftcmp,
	op_fdmul, op_fdsub, op_fddiv, op_fdadd, op_fdneg, op_fdcmp,
	op_fsmul, op_fssub, op_fsdiv, op_fsadd, op_fsneg, op_fscmp,
	op_fs2d, op_i2d, op_i2t, op_ftoi, op_itof, op_qtoi,
	op_fmov,
	op_fdmov, op_fix2flt, op_mtfp, op_mffp, op_flt2fix, op_mv2flt, op_mv2fix,
	op_fldo, op_fsto,
	op_padd, op_psub, op_pmul, op_pdiv, op_ptoi, op_itop,
	op_pldw, op_pldt, op_pldo, op_pstw, op_pstt, op_psto,
	op_peq, op_plt, op_ple,
	// Vector
	op_lv, op_sv,
	op_vadd, op_vsub, op_vmul, op_vdiv,
	op_vadds, op_vsubs, op_vmuls, op_vdivs,
	op_vfmul, op_vfmuls,
	op_vseq, op_vsne,
	op_vslt, op_vsge, op_vsle, op_vsgt,
	op_vex, op_veins,
	// RISCV
	op_lb, op_lbu, op_lh, op_lhu, op_lw, op_lwu,
	op_sb, op_sh, op_sw, op_sd,
	op_l, op_la, op_mv, op_lui,
	op_hret, op_mret, op_uret, op_sret,
	op_fcvtqdd,
	op_fld, op_flq, op_flw,
	// DSD9
	op_ldt, op_ldtu,
	op_calltgt,
	op_csrrd, op_csrrw, op_nop,
	op_hint, op_hint2, op_rem2, op_remu,
	// FT64
	op_fslt, op_fsle, op_fsgt, op_fsge, op_fseq, op_fsne, op_fsun,
	op_divmod,
	op_pushf, op_popf, op_mov2, op_sort, op_demux,
	op_lvbu, op_lvcu, op_lvhu,
	op_cmovenz,
	op_sptr, op_isptr, op_setwb, op_spt,
	op_bchk, op_ibne, op_dbnz,
	op_brk, op_rti, op_rte, op_bex,
	op_addq1, op_addq2, op_addq3,
	op_andq1, op_andq2, op_andq3,
	op_orq1, op_orq2, op_orq3,
	op_stdp,
	op_string,
	op_rem,
	op_rtl, op_rtx, op_enter, op_leave, op_base, op_enter_far, op_leave_far,
	op_mfbase, op_mtbase, op_di, op_defcat,
	op_ptrdif, op_isnullptr,
	op_mtlc, op_mflk, op_mtlk, op_sbx,
	op_exi56, op_exim, op_ldhs, op_sths,
	// Thor2023
	op_orf, op_load, op_loadz, op_loadg, op_store, op_storeg, op_vmask, op_loadv, op_fload,
	op_vfadd, op_vfadds, op_vfsub,
	op_lda,
	// Q+
	op_addmo,
	op_addm, op_addh, op_eorm, op_eorh, op_eors, op_andm, op_andh, op_ands,
	op_adds, op_ors, op_orh, op_orm, op_movsxb, op_movsxw, op_movsxt, op_subs,
	op_or_and, op_or_or, op_and_and, op_and_or,
	op_ibeq, op_iblt, op_ibltu, op_ible, op_ibleu,
	op_storev, op_subtract,
	// Bigfoot
	op_bra,
	op_loadm, op_storem, op_pushm, op_popm,
	op_stib, op_stiw, op_stit, op_stio, op_addq,
	op_zseq, op_zsne, op_zslt, op_zsle, op_zsgt, op_zsge, op_zsltu, op_zsleu, op_zsgtu, op_zsgeu,
	op_crext, op_crdep,
	op_crand, op_cror, op_creor, op_reg2crg, op_crg2reg, op_cr2cr,
	// i386
	op_movb, op_movw, op_movl,
	op_shl, op_shr, op_jae, op_jeq, op_jne, op_jl, op_jge, op_jle, op_jg, op_jb, op_jbe, op_ja,
	op_seteq, op_setne, op_setl, op_setle, op_setg, op_setge, op_setb, op_setbe, op_seta, op_setae,
	// stark
	op_b, op_bl, op_cmpa,
	// Built in functions
	op_abs, op_mulf, op_bytendx, op_zxo, op_bmap,
	op_movzxb, op_movzxw, op_movzxt,
	op_wydendx, op_sync,
	op_phi, op_pfi,
	op_verbatium,
    op_empty,
		op_last,
		op_dot = 32768
};

enum e_regtype {
	rt_invert = 4096,
	rt_vector = 8192,
	rt_group = 16384,
	rt_float = 32768,
	rt_cr = 65536,
	rt_crg = 1L << 17L
};

enum e_regclass {
	rc_gpr = 0,
	rc_flt = 1,
	rc_ccr = 2,
	rc_ccrg = 3
};

enum e_am {
	am_none = 0,
	am_reg = 1,
	am_fpreg = 2,
	am_vreg = 4,
	am_vmreg = 8,
	am_ind = 16,
	am_indx = 32,
	am_indx2 = 64,
	am_direct = 128,
	am_mem = 240,
	am_imm = 256,
	am_i5 = 512,
	am_ui6 = 1024,
	am_i8 = 2048,	// BEQI
	am_i26 = 4096,
	am_mask = 8192,
	am_ainc = 16384,
	am_adec = 32768,
	am_brind = 65536,
	am_breg = 131072,
	am_sreg = 262144,
	am_direct2 = 524288,
	am_volatile = 1 << 20,
	am_bf_assign = 1 << 21,
	am_imm0 = 1 << 22,
	am_novalue = 1 << 23,
	am_creg = 1 << 24,
	am_preg = 1 << 25,
	am_mem_indirect = 1 << 26,
	am_jmp = 1 << 27,
	am_iindx2 = 1 << 28,
	am_i16 = 1 << 29,
	amCrReg = 1 << 30,
	amCrgReg = 1 << 31,
	am_all = 0x1FB,	// exclude fp reg for Thor
};

//#define am_reg   1       /* register direct mode allowed */
//#define F_BREG	2		/* branch register */
//#define am_mem   4       /* memory alterable modes allowed */
//#define am_imm 8       /* immediate mode allowed */
//#define F_ALT   7       /* alterable modes */
//#define F_DALT  5       /* data alterable modes */
//#define am_volatile   16      /* need volitile operand */
//#define am_imm18	64	// 18-bit immediate constant
//#define am_imm0	128		/* immediate value 0 */
//#define am_i8	256
//#define am_imm13  512
//#define am_fpreg 1024
//#define am_iu6  2048
//#define am_bf_assign	4096
//#define am_vreg	8192
//#define am_vmreg	16384
//#define am_all   (15|1024|am_vreg|am_vmreg)      /* all modes allowed */
//#define am_novalue 32768		/* dont need result value */

enum e_seg {
	op_ns = 0,
	op_ds = 1 << 8,
	op_ts = 2 << 8,
	op_bs = 3 << 8,
	op_rs = 4 << 8,
	op_es = 5 << 8,
	op_seg6 = 6 << 8,
	op_seg7 = 7 << 8,
	op_seg8 = 8 << 8,
	op_seg9 = 9 << 8,
	op_seg10 = 10 << 8,
	op_seg11 = 11 << 8,
	op_seg12 = 12 << 8,
	op_seg13 = 13 << 8,
	op_ss = 14 << 8,
	op_cs = 15 << 8
};

enum e_hint {
	start_funcbody = 20,
	end_funcbody,
	begin_return_block,
	end_return_block,
	begin_stack_unlink,
	end_stack_unlink,
	begin_save_regvars,
	end_save_regvars,
	begin_restore_regvars,
	end_restore_regvars,
	begin_regvar_init,
	end_regvar_init,
	begin_func_call,
	end_func_call,
	begin_index,
	end_index
};

enum e_gt { nogen, bytegen, chargen, halfgen, wordgen, longgen, floatgen };

enum e_decltype { dt_global, dt_auto, dt_parameter };

#define LR		1
#define CLR		11

#define BP		26
#define SP		27

#define DS		0x21
#define BSS		0x23
#define LS		0x29
#define XLS		0x2A
#define SS		0x2E
#define CS		0x2F

#define MAX_STRLEN      120
#define MAX_STLP1       121
#define ERR_SYNTAX      0
#define ERR_ILLCHAR     1
#define ERR_FPCON       2
#define ERR_ILLTYPE     3
#define ERR_UNDEFINED   4
#define ERR_DUPSYM      5
#define ERR_PUNCT       6
#define ERR_IDEXPECT    7
#define ERR_NOINIT      8
#define ERR_INCOMPLETE  9
#define ERR_ILLINIT     10
#define ERR_INITSIZE    11
#define ERR_ILLCLASS    12
#define ERR_BLOCK       13
#define ERR_NOPOINTER   14
#define ERR_NOFUNC      15
#define ERR_NOMEMBER    16
#define ERR_LVALUE      17
#define ERR_DEREF       18
#define ERR_MISMATCH    19
#define ERR_EXPREXPECT  20
#define ERR_WHILEXPECT  21
#define ERR_NOCASE      22
#define ERR_DUPCASE     23
#define ERR_LABEL       24
#define ERR_PREPROC     25
#define ERR_INCLFILE    26
#define ERR_CANTOPEN    27
#define ERR_DEFINE      28
#define ERR_CATCHEXPECT	29
#define ERR_BITFIELD_WIDTH	30
#define ERR_EXPRTOOCOMPLEX	31
#define ERR_ASMTOOLONG	32
#define ERR_TOOMANYCASECONSTANTS	33
#define ERR_CATCHSTRUCT		34
#define ERR_SEMA_INCR	35
#define ERR_SEMA_ADDR	36
#define ERR_UNDEF_OP	37
#define ERR_INT_CONST	38
#define ERR_BAD_SWITCH_EXPR	39
#define ERR_NOT_IN_LOOP	40
#define ERR_CHECK       41
#define ERR_BADARRAYNDX	42
#define ERR_TOOMANYDIMEN	43
#define ERR_OUTOFPREDS  44 
#define ERR_PARMLIST_MISMATCH	45
#define ERR_PRIVATE		46
#define ERR_CALLSIG2	47
#define ERR_METHOD_NOTFOUND	48
#define ERR_OUT_OF_MEMORY   49
#define ERR_TOOMANY_SYMBOLS 50
#define ERR_TOOMANY_PARAMS  51
#define ERR_THIS            52
#define ERR_BADARG			53
#define ERR_CSETABLE		54
#define ERR_UBLTZ			55
#define ERR_UBGEQ			56
#define ERR_INFINITELOOP	57
#define ERR_TOOMANYELEMENTS	58
#define ERR_CONST			59
#define ERR_INIT_UNION      60
#define ERR_LOST_PREC		61
#define ERR_TOOMANY_TREES	62
#define ERR_STACKFULL		63
#define ERR_STACKEMPTY		64
#define ERR_IGNODES			65
#define ERR_CASTAGGR		66
#define ERR_PRECISION		67
#define ERR_TOOMANY_DESIGNATORS	68
#define ERR_NOT_STRUCT	69
#define ERR_INT0255			70
#define ERR_CLOSEPA			71
#define ERR_EOF_REACHED	72
#define ERR_BAD_PRECISION 73
#define ERR_UNKNOWN_FN	74
#define ERR_MISSING_PARM	75
#define ERR_SHIFT_TOOMANYBITS	76
#define ERR_NULLPOINTER		1000
#define ERR_CIRCULAR_LIST 1001
#define ERR_MISSING_HIDDEN_STRUCTPTR	1002
#define ERR_CODEGEN 1003
#define ERR_MISSING_MUX 1004

/*      alignment sizes         */

#define AL_BYTE			1
#define AL_CHAR         2
#define AL_SHORT        4
#define AL_INT					8
#define AL_LONG         16
#define AL_POINTER      16
#define AL_FLOAT        8
#define AL_DOUBLE       8
#define AL_POSIT				8
#define AL_QUAD			16
#define AL_STRUCT       2
#define AL_TRIPLE       12

#define TRUE	1
#define FALSE	0
//#define NULL	((void *)0)

// Register classes for instructions
#define RC_GP	1
#define RC_FP	2
#define RC_VEC	4
#define RC_ALL	7

#define MAX_PARMS		20

#define CR0		80
#define BR7		79
#endif
