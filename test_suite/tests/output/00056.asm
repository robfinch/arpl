                                                                                                                                                   
	.text
  
;{++ _main00056

	.sdreg	29
_main00056:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; int a;
  ldi t2,12
  ldi t3,34
; a = 42;
  ldi t1,42
; printf("%d\n", a);
  sub sp,sp,16
  lda t0,_main00056.00001[gp]
  sto t0,0[sp]
  sto t1,8[sp]
  bsr _printf
; printf("%d\n", b);
  sub sp,sp,16
  lda t0,_main00056.00002[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
; printf("%d, %d\n", c, d);
  sub sp,sp,24
  lda t0,_main00056.00003[gp]
  sto t0,0[sp]
  sto t2,8[sp]
  sto t3,16[sp]
  bsr _printf
  add sp,sp,8
; return 0;
  mov a0,r0
.00013:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00056,@function
	.size	_main00056,$-_main00056


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00056.00001,@object
	.size	_main00056.00001,8
_main00056.00001: ; %d

	.2byte	37,100,10,0

	.type	_main00056.00002,@object
	.size	_main00056.00002,8
_main00056.00002: ; %d

	.2byte	37,100,10,0

	.type	_main00056.00003,@object
	.size	_main00056.00003,16
_main00056.00003: ; %d, %d

	.2byte	37,100,44,32,37,100,10,0
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
	.global	_feof
	.global	_main00056
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata