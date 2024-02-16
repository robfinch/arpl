                                                                                                                                                  
	.text

;{++ _main00131

	.sdreg	29
_main00131:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; printf("Hello\n");
  sub sp,sp,8
  lda t0,_main00131.00001[gp]
  sto t0,0[sp]
  bsr _printf
; printf("Hello\n"); /* this is a comment */ printf("Hello\n");
  sub sp,sp,8
  lda t0,_main00131.00002[gp]
  sto t0,0[sp]
  bsr _printf
  sub sp,sp,8
  lda t0,_main00131.00003[gp]
  sto t0,0[sp]
  bsr _printf
; printf("Hello\n");
  sub sp,sp,8
  lda t0,_main00131.00004[gp]
  sto t0,0[sp]
  bsr _printf
  sub sp,sp,8
  lda t0,_main00131.00005[gp]
  sto t0,0[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00015:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00131,@function
	.size	_main00131,$-_main00131


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00131.00001,@object
	.size	_main00131.00001,14
_main00131.00001: ; Hello

	.2byte	72,101,108,108,111,10,0

	.type	_main00131.00002,@object
	.size	_main00131.00002,14
_main00131.00002: ; Hello

	.2byte	72,101,108,108,111,10,0

	.type	_main00131.00003,@object
	.size	_main00131.00003,14
_main00131.00003: ; Hello

	.2byte	72,101,108,108,111,10,0

	.type	_main00131.00004,@object
	.size	_main00131.00004,14
_main00131.00004: ; Hello

	.2byte	72,101,108,108,111,10,0

	.type	_main00131.00005,@object
	.size	_main00131.00005,14
_main00131.00005: ; Hello

	.2byte	72,101,108,108,111,10,0
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
	.global	_main00131
	.global	_feof
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
