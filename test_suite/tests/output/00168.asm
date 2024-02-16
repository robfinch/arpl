                                                                                                                                                   
	.text

;{++ _factorial

	.sdreg	29
_factorial:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; if (i < 2)
  ldi t1,2
  bge t1,t1,.00013
; return i;
  mov a0,t1
.00012:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 40,0
.00013:
; return i * factorial(i - 1);
  sto t0,-32[fp]
  store v9,-32[fp]
  sub sp,sp,8
  sub t1,t1,1
  sto t1,0[sp]
  bsr _factorial
  mul v9,t1,a0
  ldw a0,v9
  bra .00012
	.type	_factorial,@function
	.size	_factorial,$-_factorial


;--}
  
;{++ _main00168

	.sdreg	29
_main00168:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; for (Count = 1; Count <= 10; Count++)
  ldi t0,1
  ldi t1,10
  bgt t0,t1,.00030
.00029:
; printf("%d\n", factorial(Count));
  sub sp,sp,16
  lda t0,_main00168.00015[gp]
  sto t0,0[sp]
  sub sp,sp,8
  sto t0,0[sp]
  bsr _factorial
  sto a0,8[sp]
  bsr _printf
.00031:
.00030:
; return 0;
  mov a0,r0
.00028:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00168,@function
	.size	_main00168,$-_main00168


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00168.00015,@object
	.size	_main00168.00015,8
_main00168.00015: ; %d

	.2byte	37,100,10,0
	.extern	__Files
	.global	_getc
	.global	_main00168
	.global	_gets
	.global	_factorial
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
