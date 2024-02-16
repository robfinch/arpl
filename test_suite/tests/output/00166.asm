                                                                                                                                                   
	.text
   
;{++ _main00166

	.sdreg	29
_main00166:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,64
  sto s0,[sp]
  sto s1,8[sp]
  sto s2,16[sp]
  sto s3,24[sp]
; int a = 24680;
  ldi s0,24680
  ldi s2,342391
  ldi s3,2386092
  ldi s1,2386092
; printf("%d\n", a);
  sub sp,sp,16
  lda t0,_main00166.00001[gp]
  sto t0,0[sp]
  sto s0,8[sp]
  bsr _printf
; printf("%d\n", b);
  sub sp,sp,16
  lda t0,_main00166.00002[gp]
  sto t0,0[sp]
  sto s2,8[sp]
  bsr _printf
; printf("%d\n", c);
  sub sp,sp,16
  lda t0,_main00166.00003[gp]
  sto t0,0[sp]
  sto s3,8[sp]
  bsr _printf
; printf("%d\n", d);
  sub sp,sp,16
  lda t0,_main00166.00004[gp]
  sto t0,0[sp]
  sto s1,8[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00014:
  ldo s0,[sp]
  ldo s1,8[sp]
  ldo s2,16[sp]
  ldo s3,24[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00166,@function
	.size	_main00166,$-_main00166


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00166.00001,@object
	.size	_main00166.00001,8
_main00166.00001: ; %d

	.2byte	37,100,10,0

	.type	_main00166.00002,@object
	.size	_main00166.00002,8
_main00166.00002: ; %d

	.2byte	37,100,10,0

	.type	_main00166.00003,@object
	.size	_main00166.00003,8
_main00166.00003: ; %d

	.2byte	37,100,10,0

	.type	_main00166.00004,@object
	.size	_main00166.00004,8
_main00166.00004: ; %d

	.2byte	37,100,10,0
	.extern	__Files
	.global	_main00166
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
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
