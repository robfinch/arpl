                                                                                                                                                     
	.text

;{++ _main00171

	.sdreg	29
_main00171:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; a = 42;
  ldi t0,42
  sto t0,-8[fp]
; c = (void *)0;
  mov t1,r0
; printf("%d\n", *b);
  sub sp,sp,16
  lda t0,_main00171.00001[gp]
  sto t0,0[sp]
  ldo t0,[t0]
  sto t0,8[sp]
  bsr _printf
; if (b == (void *)0)
  bnez t0,.00020
; printf("b is NULL\n");
  sub sp,sp,8
  lda t0,_main00171.00002[gp]
  sto t0,0[sp]
  bsr _printf
  bra .00021
.00020:
; printf("b is not NULL\n");
  sub sp,sp,8
  lda t0,_main00171.00003[gp]
  sto t0,0[sp]
  bsr _printf
.00021:
; if (c == (void *)0)
  bnez t1,.00022
; printf("c is NULL\n");
  sub sp,sp,8
  lda t0,_main00171.00004[gp]
  sto t0,0[sp]
  bsr _printf
  bra .00023
.00022:
; printf("c is not NULL\n");
  sub sp,sp,8
  lda t0,_main00171.00005[gp]
  sto t0,0[sp]
  bsr _printf
.00023:
; return 0;
  mov a0,r0
.00019:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00171,@function
	.size	_main00171,$-_main00171


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00171.00001,@object
	.size	_main00171.00001,8
_main00171.00001: ; %d

	.2byte	37,100,10,0

	.type	_main00171.00002,@object
	.size	_main00171.00002,22
_main00171.00002: ; b is NULL

	.2byte	98,32,105,115,32,78,85,76
	.2byte	76,10,0

	.type	_main00171.00003,@object
	.size	_main00171.00003,30
_main00171.00003: ; b is not NULL

	.2byte	98,32,105,115,32,110,111,116
	.2byte	32,78,85,76,76,10,0

	.type	_main00171.00004,@object
	.size	_main00171.00004,22
_main00171.00004: ; c is NULL

	.2byte	99,32,105,115,32,78,85,76
	.2byte	76,10,0

	.type	_main00171.00005,@object
	.size	_main00171.00005,30
_main00171.00005: ; c is not NULL

	.2byte	99,32,105,115,32,110,111,116
	.2byte	32,78,85,76,76,10,0
	.extern	__Files
	.global	_getc
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
	.global	_main00171
	.global	_feof
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
