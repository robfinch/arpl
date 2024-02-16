                                                                                                                                                    
	.text

;{++ _f1

	.sdreg	29
_f1:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
  ldo s0,32[fp]
; if(0)
  ldi t0,0
  beqz t0,.00017
.00001:
; printf("boom!\n");
  sub sp,sp,8
  lda t0,_f1.00002[gp]
  sto t0,0[sp]
  bsr _printf
.00017:
; if(argc-- == 0)
  bnez s0,.00019
.00016:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 40,0
.00019:
; goto label;
  bra .00001
	.type	_f1,@function
	.size	_f1,$-_f1


;--}
     
;{++ _f2

	.sdreg	29
_f2:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
; a[0] = 0;
  lda t0,-8[fp]
  sto r0,0[t0]
; b[0] = 0;
  lda t0,-16[fp]
  sto r0,0[t0]
; c[0] = 0;
  lda t0,-24[fp]
  sto r0,0[t0]
.00031:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_f2,@function
	.size	_f2,$-_f2


;--}
  
;{++ _f3

	.sdreg	29
_f3:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; printf("%d\n", 0 ? printf("x1\n") : 11);
  sub sp,sp,16
  lda t0,_f3.00032[gp]
  sto t0,0[sp]
  ldi t0,0
  beqz t0,.00056
  sto t0,-32[fp]
  sub sp,sp,8
  lda t1,_f3.00033[gp]
  sto t1,0[sp]
  bsr _printf
  mov t0,a0
  bra .00057
.00056:
  ldi t1,11
  ldi t0,11
.00057:
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", 1 ? 12 : printf("x2\n"));
  sub sp,sp,16
  lda t0,_f3.00034[gp]
  sto t0,0[sp]
  ldi t0,1
  beqz t0,.00058
  ldi t1,12
  ldi t0,12
  bra .00059
.00058:
  sto t0,-32[fp]
  sub sp,sp,8
  lda t1,_f3.00035[gp]
  sto t1,0[sp]
  bsr _printf
  mov t0,a0
.00059:
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", 0 && printf("x3\n"));
  sub sp,sp,16
  lda t0,_f3.00036[gp]
  sto t0,0[sp]
  sub sp,sp,8
  lda t0,_f3.00037[gp]
  sto t0,0[sp]
  bsr _printf
  sto a0,8[sp]
  bsr _printf
; printf("%d\n", 1 || printf("x4\n"));
  sub sp,sp,16
  lda t0,_f3.00038[gp]
  sto t0,0[sp]
  sub sp,sp,8
  lda t0,_f3.00039[gp]
  sto t0,0[sp]
  bsr _printf
  sto a0,8[sp]
  bsr _printf
.00055:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_f3,@function
	.size	_f3,$-_f3


;--}
 
;{++ _main00207

	.sdreg	29
_main00207:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; f1(2);
  sub sp,sp,8
  ldi t0,2
  sto t0,0[sp]
  bsr _f1
; f2();
  bsr _f2
; f3();
  bsr _f3
; return 0;
  mov a0,r0
.00071:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00207,@function
	.size	_main00207,$-_main00207


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_f1.00002,@object
	.size	_f1.00002,14
_f1.00002: ; boom!

	.2byte	98,111,111,109,33,10,0

	.type	_f3.00032,@object
	.size	_f3.00032,8
_f3.00032: ; %d

	.2byte	37,100,10,0

	.type	_f3.00033,@object
	.size	_f3.00033,8
_f3.00033: ; x1

	.2byte	120,49,10,0

	.type	_f3.00034,@object
	.size	_f3.00034,8
_f3.00034: ; %d

	.2byte	37,100,10,0

	.type	_f3.00035,@object
	.size	_f3.00035,8
_f3.00035: ; x2

	.2byte	120,50,10,0

	.type	_f3.00036,@object
	.size	_f3.00036,8
_f3.00036: ; %d

	.2byte	37,100,10,0

	.type	_f3.00037,@object
	.size	_f3.00037,8
_f3.00037: ; x3

	.2byte	120,51,10,0

	.type	_f3.00038,@object
	.size	_f3.00038,8
_f3.00038: ; %d

	.2byte	37,100,10,0

	.type	_f3.00039,@object
	.size	_f3.00039,8
_f3.00039: ; x4

	.2byte	120,52,10,0
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
	.global	_f1
	.global	_f2
	.global	_f3
	.global	_perror
	.global	_main00207
	.global	_feof
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
