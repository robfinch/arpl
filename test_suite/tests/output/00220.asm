                                                                                                                                                  
	.text

;{++ _main00220

	.sdreg	29
_main00220:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,48
  sto s0,[sp]
  sto s1,8[sp]
  lda s0,_main00220.09999[r56]
  lda s1,_main00220.09999[r56]
; for (p = s; *p; p++) printf("%04X ", (unsigned) *p);
  lda t0,_main00220.09999[r56]
  ldo t0,_main00220.09999[r56]
  beqz t0,.00018
.00017:
  sub sp,sp,16
  lda t0,_main00220.00002[gp]
  sto t0,0[sp]
  ldo t0,_main00220.09999[r56]
  sto t0,8[sp]
  bsr _printf
  lda t0,_main00220.09999[r56]
  ldo t0,_main00220.09999[r56]
  bnez t0,.00017
.00018:
; printf("\n");
  sub sp,sp,8
  lda t0,_main00220.00003[gp]
  sto t0,0[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00016:
  ldo s0,[sp]
  ldo s1,8[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00220,@function
	.size	_main00220,$-_main00220


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00220.00001,@object
	.size	_main00220.00001,0
_main00220.00001: ; hello$$你好¢¢世界€€world


	.type	_main00220.00002,@object
	.size	_main00220.00002,12
_main00220.00002: ; %04X 

	.2byte	37,48,52,88,32,0

	.type	_main00220.00003,@object
	.size	_main00220.00003,4
_main00220.00003:

	.2byte	10,0
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
	.global	_main00220
	.global	_perror
	.global	_feof
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
