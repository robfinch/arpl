                                                                                                                                                                                                                  
	.text

;{++ _main00174

	.sdreg	29
_main00174:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  ldo t1,0x0000[gp]
; printf("%f\n", a);
  sub sp,sp,16
  lda t0,_main00174.00001[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
; printf("%f\n", 12.34 + 56.78);
  sub sp,sp,16
  lda t0,_main00174.00002[gp]
  sto t0,0[sp]
  ldh t0,0x0000[gp]
  sto t0,8[sp]
  bsr _printf
; printf("%f\n", 12.34 - 56.78);
  sub sp,sp,16
  lda t0,_main00174.00003[gp]
  sto t0,0[sp]
  sto t1,8[sp]
  bsr _printf
; printf("%f\n", 12.34 * 56.78);
  sub sp,sp,16
  lda t0,_main00174.00004[gp]
  sto t0,0[sp]
  ldh t0,0x0000[gp]
  sto t0,8[sp]
  bsr _printf
; printf("%f\n", 12.34 / 56.78);
  sub sp,sp,16
  lda t0,_main00174.00005[gp]
  sto t0,0[sp]
  ldh t0,0x0000[gp]
  sto t0,8[sp]
  bsr _printf
; printf("%d %d %d %d %d %d\n", 12.34 < 56.78, 12.34 <= 56.78, 12.34 == 56.78, 12.34 >= 56.78, 12.34 > 56.78, 12.34 != 56.78);
  sub sp,sp,56
  lda t0,_main00174.00006[gp]
  sto t0,0[sp]
  orf t0,r0,0
  sto t0,8[sp]
  orf t0,r0,0
  sto t0,16[sp]
  orf t0,r0,0
  sto t0,24[sp]
  orf t0,r0,0
  sto t0,32[sp]
  orf t0,r0,0
  sto t0,40[sp]
  orf t0,r0,0
  sto t0,48[sp]
  bsr _printf
  add sp,sp,40
; printf("%d %d %d %d %d %d\n", 12.34 < 12.34, 12.34 <= 12.34, 12.34 == 12.34, 12.34 >= 12.34, 12.34 > 12.34, 12.34 != 12.34);
  sub sp,sp,56
  lda t0,_main00174.00007[gp]
  sto t0,0[sp]
  orf t0,r0,0
  sto t0,8[sp]
  orf t0,r0,0
  sto t0,16[sp]
  orf t0,r0,0
  sto t0,24[sp]
  orf t0,r0,0
  sto t0,32[sp]
  orf t0,r0,0
  sto t0,40[sp]
  orf t0,r0,0
  sto t0,48[sp]
  bsr _printf
  add sp,sp,40
; printf("%d %d %d %d %d %d\n", 56.78 < 12.34, 56.78 <= 12.34, 56.78 == 12.34, 56.78 >= 12.34, 56.78 > 12.34, 56.78 != 12.34);
  sub sp,sp,56
  lda t0,_main00174.00008[gp]
  sto t0,0[sp]
  orf t0,r0,0
  sto t0,8[sp]
  orf t0,r0,0
  sto t0,16[sp]
  orf t0,r0,0
  sto t0,24[sp]
  orf t0,r0,0
  sto t0,32[sp]
  orf t0,r0,0
  sto t0,40[sp]
  orf t0,r0,0
  sto t0,48[sp]
  bsr _printf
  add sp,sp,40
; printf("%f\n", a);
  sub sp,sp,16
  lda t0,_main00174.00009[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
; printf("%f\n", a);
  sub sp,sp,16
  lda t0,_main00174.00010[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
; printf("%f\n", a);
  sub sp,sp,16
  lda t0,_main00174.00011[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
; printf("%f\n", a);
  sub sp,sp,16
  lda t0,_main00174.00012[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
; printf("%f\n", +12.34);
  sub sp,sp,16
  lda t0,_main00174.00013[gp]
  sto t0,0[sp]
  sto t1,8[sp]
  bsr _printf
; printf("%f\n", -12.34);
  sub sp,sp,16
  lda t0,_main00174.00014[gp]
  sto t0,0[sp]
  ldh t0,0x0000[gp]
  sto t0,8[sp]
  bsr _printf
; printf("%f\n", a);
  sub sp,sp,16
  lda t0,_main00174.00016[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
; printf("%f\n", _Sin(2, 0));
  sub sp,sp,16
  lda t0,_main00174.00017[gp]
  sto t0,0[sp]
  sub sp,sp,16
  ldi t0,2
  sto t0,0[sp]
  sto r0,8[sp]
  bsr __Sin
  sto a0,8[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00032:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00174,@function
	.size	_main00174,$-_main00174


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00174.00015,@object
	.size	_main00174.00015,8
_main00174.00015:
	.4byte	0x7AE147AE,0xC028AE14

	.type	_main00174.00023,@object
	.size	_main00174.00023,8
_main00174.00023:
	.4byte	0x147AE147,0x405147AE

	.type	_main00174.00024,@object
	.size	_main00174.00024,8
_main00174.00024:
	.4byte	0x7AE147AE,0x4028AE14

	.type	_main00174.00025,@object
	.size	_main00174.00025,8
_main00174.00025:
	.4byte	0x5460AA64,0x4085E552

	.type	_main00174.00026,@object
	.size	_main00174.00026,8
_main00174.00026:
	.4byte	0x8F8E0597,0x3FCBD178

	.type	_main00174.00027,@object
	.size	_main00174.00027,8
_main00174.00027:
	.4byte	0x00000000,0x40000000
	.align	8



	.type	_main00174.00001,@object
	.size	_main00174.00001,8
_main00174.00001: ; %f

	.2byte	37,102,10,0

	.type	_main00174.00002,@object
	.size	_main00174.00002,8
_main00174.00002: ; %f

	.2byte	37,102,10,0

	.type	_main00174.00003,@object
	.size	_main00174.00003,8
_main00174.00003: ; %f

	.2byte	37,102,10,0

	.type	_main00174.00004,@object
	.size	_main00174.00004,8
_main00174.00004: ; %f

	.2byte	37,102,10,0

	.type	_main00174.00005,@object
	.size	_main00174.00005,8
_main00174.00005: ; %f

	.2byte	37,102,10,0

	.type	_main00174.00006,@object
	.size	_main00174.00006,38
_main00174.00006: ; %d %d %d %d %d %d

	.2byte	37,100,32,37,100,32,37,100
	.2byte	32,37,100,32,37,100,32,37
	.2byte	100,10,0

	.type	_main00174.00007,@object
	.size	_main00174.00007,38
_main00174.00007: ; %d %d %d %d %d %d

	.2byte	37,100,32,37,100,32,37,100
	.2byte	32,37,100,32,37,100,32,37
	.2byte	100,10,0

	.type	_main00174.00008,@object
	.size	_main00174.00008,38
_main00174.00008: ; %d %d %d %d %d %d

	.2byte	37,100,32,37,100,32,37,100
	.2byte	32,37,100,32,37,100,32,37
	.2byte	100,10,0

	.type	_main00174.00009,@object
	.size	_main00174.00009,8
_main00174.00009: ; %f

	.2byte	37,102,10,0

	.type	_main00174.00010,@object
	.size	_main00174.00010,8
_main00174.00010: ; %f

	.2byte	37,102,10,0

	.type	_main00174.00011,@object
	.size	_main00174.00011,8
_main00174.00011: ; %f

	.2byte	37,102,10,0

	.type	_main00174.00012,@object
	.size	_main00174.00012,8
_main00174.00012: ; %f

	.2byte	37,102,10,0

	.type	_main00174.00013,@object
	.size	_main00174.00013,8
_main00174.00013: ; %f

	.2byte	37,102,10,0

	.type	_main00174.00014,@object
	.size	_main00174.00014,8
_main00174.00014: ; %f

	.2byte	37,102,10,0

	.type	_main00174.00016,@object
	.size	_main00174.00016,8
_main00174.00016: ; %f

	.2byte	37,102,10,0

	.type	_main00174.00017,@object
	.size	_main00174.00017,8
_main00174.00017: ; %f

	.2byte	37,102,10,0
	.extern	__Hugeval
	.extern	__Files
	.global	_main00174
	.global	_getc
	.global	_log10
	.global	_atan
	.global	_acos
	.global	_fmod
	.global	_modf
	.global	_asin
	.global	_tanh
	.global	_cosh
	.global	_sinh
	.global	_gets
	.global	_putc
	.global	_sqrt
	.global	_puts
	.global	_atan2
	.global	_getchar
	.global	__Asin
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
	.global	_ldexp
	.global	_fputc
	.global	_floor
	.global	_frexp
	.global	_fputs
	.global	_log
	.global	_tan
	.global	_cos
	.global	_sin
	.global	_exp
	.global	_clearerr
	.global	_pow
	.global	__Fgpos
	.global	__Fspos
	.global	_vfprintf
	.global	_fscanf
	.global	_rename
	.global	_fclose
	.global	_vsprintf
	.global	_sscanf
	.global	__Log
	.global	_ungetc
	.global	_fflush
	.global	_rewind
	.global	_setbuf
	.global	__Sin
	.global	_tmpnam
	.global	_remove
	.global	_ferror
	.global	_fwrite
	.global	_setjmp
	.global	_printf
	.global	_perror
	.global	_fabs
	.global	_ceil
	.global	_feof
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
