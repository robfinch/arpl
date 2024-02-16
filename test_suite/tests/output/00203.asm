                                                                                                                                                   
	.text

;{++ _main00203

	.sdreg	29
_main00203:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
; long long int res = 0;
  mov s0,r0
; if (res < -2147483648LL) {
  or t1,r0,0
  adds t1,16777088,1
  bge s0,t1,.00018
; printf("Error: 0 < -2147483648\n");
  sub sp,sp,8
  lda t0,_main00203.00001[gp]
  sto t0,0[sp]
  bsr _printf
; return 1;
  ldi a0,1
.00017:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00018:
; if (2147483647LL < res) {
  or t1,r0,16777215
  adds t1,127,1
  bge t1,s0,.00020
; printf("Error: 2147483647 < 0\n");
  sub sp,sp,8
  lda t0,_main00203.00002[gp]
  sto t0,0[sp]
  bsr _printf
; return 2;
  ldi a0,2
  bra .00017
.00020:
; printf("long long constant test ok.\n");
  sub sp,sp,8
  lda t0,_main00203.00003[gp]
  sto t0,0[sp]
  bsr _printf
; return 0;
  mov a0,r0
  bra .00017
	.type	_main00203,@function
	.size	_main00203,$-_main00203


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00203.00001,@object
	.size	_main00203.00001,48
_main00203.00001: ; Error: 0 < -2147483648

	.2byte	69,114,114,111,114,58,32,48
	.2byte	32,60,32,45,50,49,52,55
	.2byte	52,56,51,54,52,56,10,0

	.type	_main00203.00002,@object
	.size	_main00203.00002,46
_main00203.00002: ; Error: 2147483647 < 0

	.2byte	69,114,114,111,114,58,32,50
	.2byte	49,52,55,52,56,51,54,52
	.2byte	55,32,60,32,48,10,0

	.type	_main00203.00003,@object
	.size	_main00203.00003,58
_main00203.00003: ; long long constant test ok.

	.2byte	108,111,110,103,32,108,111,110
	.2byte	103,32,99,111,110,115,116,97
	.2byte	110,116,32,116,101,115,116,32
	.2byte	111,107,46,10,0
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
	.global	_main00203
	.global	_feof
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
