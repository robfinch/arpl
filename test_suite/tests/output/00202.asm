                                                                                                                                                    
	.text

;{++ _main00202

	.sdreg	29
_main00202:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,48
  sto s0,[sp]
  sto s1,8[sp]
; int bob, jim = 21;
  ldi s0,21
; int bob, jim = 21;
  ldi s1,21
  mul s1,s1,2
; printf("jim: %d, bob: %d\n", jim, bob);
  sub sp,sp,24
  lda t0,_main00202.00001[gp]
  sto t0,0[sp]
  sto s0,8[sp]
  sto s1,16[sp]
  bsr _printf
  add sp,sp,8
; jim = 60 ++3;
  ldi s0,60
; printf("jim: %d\n", jim);
  sub sp,sp,16
  lda t0,_main00202.00002[gp]
  sto t0,0[sp]
  sto s0,8[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00012:
  ldo s0,[sp]
  ldo s1,8[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00202,@function
	.size	_main00202,$-_main00202


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00202.00001,@object
	.size	_main00202.00001,36
_main00202.00001: ; jim: %d, bob: %d

	.2byte	106,105,109,58,32,37,100,44
	.2byte	32,98,111,98,58,32,37,100
	.2byte	10,0

	.type	_main00202.00002,@object
	.size	_main00202.00002,18
_main00202.00002: ; jim: %d

	.2byte	106,105,109,58,32,37,100,10
	.2byte	0
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
	.global	_main00202
	.global	_perror
	.global	_feof
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
