
;{++ _main00208

	.text

	.sdreg	29
_main00208:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; if (f1char() || f1int())
  bsr _f1char
  bnez a0,.00050
  bsr _f1int
  beqz a0,.00048
.00050:
; printf ("bla\n");
  sub sp,sp,8
  lda t0,_main00208.00034[gp]
  sto t0,0[sp]
  bsr _printf
.00048:
; return !s[0];
  lda t1,27[fp]
  ldw t1,0[t1]
  not a0,t1
.00047:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00208,@function
	.size	_main00208,$-_main00208


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_f1char.00002,@object
	.size	_f1char.00002,10
_f1char.00002: ; bugs

	.2byte	98,117,103,115,0


	.type	_f1int.00015,@object
	.size	_f1int.00015,10
_f1int.00015: ; bugs

	.2byte	98,117,103,115,0


	.type	_main00208.00029,@object
	.size	_main00208.00029,10
_main00208.00029: ; bugs

	.2byte	98,117,103,115,0





	.type	_main00208.00034,@object
	.size	_main00208.00034,10
_main00208.00034: ; bla

	.2byte	98,108,97,10,0

	.align	8

	.align 4
_s:
	.type	_s,@object
	.size	_s,18
	.2byte	0,0,0,0,0,0,0


	.align	8

	.align 4
_s:
	.type	_s,@object
	.size	_s,18
	.2byte	0,0,0,0,0,0,0


	.align	8

	.align 4
_s:
	.type	_s,@object
	.size	_s,18
	.2byte	0,0,0,0,0,0,0




	.bss
	.align	6
	.type	00208.00028,@object
	.size	00208.00028,10
00208.00028: ; _q
	.byte	0,0,0,0,0,0,0,0,0,0

	.4byte	_f1char.00000.00000
	.2byte	99

	.extern	__Files
	.global	_getc
	.global	_gets
	.global	_putc
	.global	_puts
	.global	_getchar
	.global	_f1int
	.global	_freopen
	.global	_tmpfile
	.global	_putchar
	.global	_fgetpos
	.global	_fprintf
	.global	_setvbuf
	.global	_fread
	.global	_fsetpos
	.global	_sprintf
	.global	_fgetc
	.global	_vprintf
	.global	_scanf
	.global	_fseek
	.global	_ftell
	.global	_fopen
	.global	_fgets
	.global	_fputc
	.global	_fputs
	.global	_f1char
	.global	_clearerr
	.global	__Fgpos
	.global	__Fspos
	.global	_vfprintf
	.global	_fscanf
	.global	_rename
	.global	_fclose
	.global	_vsprintf
	.global	_sscanf
	.global	_ungetc
	.global	_fflush
	.global	_rewind
	.global	_setbuf
	.global	_tmpnam
	.global	_remove
	.global	_ferror
	.global	_fwrite
	.global	_setjmp
	.global	_printf
	.global	_perror
	.global	_main00208
	.global	_feof
	.extern	_start_data
	.global	__noname_var0
	.global	__noname_var1
	.extern	_start_bss
	.extern	_start_rodata
