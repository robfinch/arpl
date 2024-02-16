                                                                                                                                                  
	.text

;{++ _main00188

	.sdreg	29
_main00188:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; printf("#include test\n");
  sub sp,sp,8
  lda t0,_main00188.00001[gp]
  sto t0,0[sp]
  bsr _printf
; printf("b\n");
  sub sp,sp,8
  lda t0,_main00188.00002[gp]
  sto t0,0[sp]
  bsr _printf
; printf("g\n");
  sub sp,sp,8
  lda t0,_main00188.00003[gp]
  sto t0,0[sp]
  bsr _printf
; printf("i\n");
  sub sp,sp,8
  lda t0,_main00188.00004[gp]
  sto t0,0[sp]
  bsr _printf
; printf("p\n");
  sub sp,sp,8
  lda t0,_main00188.00005[gp]
  sto t0,0[sp]
  bsr _printf
; printf("r\n");
  sub sp,sp,8
  lda t0,_main00188.00006[gp]
  sto t0,0[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00016:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00188,@function
	.size	_main00188,$-_main00188


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00188.00001,@object
	.size	_main00188.00001,30
_main00188.00001: ; #include test

	.2byte	35,105,110,99,108,117,100,101
	.2byte	32,116,101,115,116,10,0

	.type	_main00188.00002,@object
	.size	_main00188.00002,6
_main00188.00002: ; b

	.2byte	98,10,0

	.type	_main00188.00003,@object
	.size	_main00188.00003,6
_main00188.00003: ; g

	.2byte	103,10,0

	.type	_main00188.00004,@object
	.size	_main00188.00004,6
_main00188.00004: ; i

	.2byte	105,10,0

	.type	_main00188.00005,@object
	.size	_main00188.00005,6
_main00188.00005: ; p

	.2byte	112,10,0

	.type	_main00188.00006,@object
	.size	_main00188.00006,6
_main00188.00006: ; r

	.2byte	114,10,0
	.extern	__Files
	.global	_getc
	.global	_main00188
	.global	_gets
	.global	_putc
	.global	_puts
	.global	_getchar
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
	.global	_feof
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
