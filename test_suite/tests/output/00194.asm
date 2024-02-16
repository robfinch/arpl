                                                                                                                                                    
	.text

;{++ _main00194

	.sdreg	29
_main00194:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,48
  sto s0,[sp]
  sto s1,8[sp]
; a = 0;
  mov s0,r0
; while (a < 2)
  ldi t1,2
  bge s0,t1,.00020
.00019:
; printf("%d", a++);
  sub sp,sp,16
  lda t0,_main00194.00001[gp]
  sto t0,0[sp]
  add s0,s0,1
  sto s0,8[sp]
  bsr _printf
; break;
  bra .00020
.00021:
; printf("%c", b++);
  sub sp,sp,16
  lda t0,_main00194.00002[gp]
  sto t0,0[sp]
  add s1,s1,1
  sto s1,8[sp]
  bsr _printf
  ldi t1,67
  blt s1,t1,.00021
.00022:
; printf("e");
  sub sp,sp,8
  lda t0,_main00194.00003[gp]
  sto t0,0[sp]
  bsr _printf
  ldi t1,2
  blt s0,t1,.00019
.00020:
; printf("\n");
  sub sp,sp,8
  lda t0,_main00194.00004[gp]
  sto t0,0[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00018:
  ldo s0,[sp]
  ldo s1,8[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00194,@function
	.size	_main00194,$-_main00194


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00194.00001,@object
	.size	_main00194.00001,6
_main00194.00001: ; %d

	.2byte	37,100,0

	.type	_main00194.00002,@object
	.size	_main00194.00002,6
_main00194.00002: ; %c

	.2byte	37,99,0

	.type	_main00194.00003,@object
	.size	_main00194.00003,4
_main00194.00003: ; e

	.2byte	101,0

	.type	_main00194.00004,@object
	.size	_main00194.00004,4
_main00194.00004:

	.2byte	10,0
	.extern	__Files
	.global	_getc
	.global	_main00194
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
