                                                                                                                                                   
	.text
 
;{++ _main00167

	.sdreg	29
_main00167:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; int a = 1;
  ldi t1,1
  mov t0,r0
; if (a)
  beqz t1,.00019
; printf("a is true\n");
  sub sp,sp,8
  lda t0,_main00167.00001[gp]
  sto t0,0[sp]
  bsr _printf
  bra .00020
.00019:
; printf("a is false\n");
  sub sp,sp,8
  lda t0,_main00167.00002[gp]
  sto t0,0[sp]
  bsr _printf
.00020:
; if (b)
  beqz t0,.00021
; printf("b is true\n");
  sub sp,sp,8
  lda t0,_main00167.00003[gp]
  sto t0,0[sp]
  bsr _printf
  bra .00022
.00021:
; printf("b is false\n");
  sub sp,sp,8
  lda t0,_main00167.00004[gp]
  sto t0,0[sp]
  bsr _printf
.00022:
; return 0;
  mov a0,r0
.00018:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00167,@function
	.size	_main00167,$-_main00167


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00167.00001,@object
	.size	_main00167.00001,22
_main00167.00001: ; a is true

	.2byte	97,32,105,115,32,116,114,117
	.2byte	101,10,0

	.type	_main00167.00002,@object
	.size	_main00167.00002,24
_main00167.00002: ; a is false

	.2byte	97,32,105,115,32,102,97,108
	.2byte	115,101,10,0

	.type	_main00167.00003,@object
	.size	_main00167.00003,22
_main00167.00003: ; b is true

	.2byte	98,32,105,115,32,116,114,117
	.2byte	101,10,0

	.type	_main00167.00004,@object
	.size	_main00167.00004,24
_main00167.00004: ; b is false

	.2byte	98,32,105,115,32,102,97,108
	.2byte	115,101,10,0
	.extern	__Files
	.global	_getc
	.global	_main00167
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
