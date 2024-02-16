                                                                                                                                                   
	.text

;{++ _charfunc

	.sdreg	29
_charfunc:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  sub sp,sp,16
  lda t0,_charfunc.00001[gp]
  sto t0,0[sp]
  ldw t0,32[fp]
  sto t0,8[sp]
  bsr _printf
.00011:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 40,0
	.type	_charfunc,@function
	.size	_charfunc,$-_charfunc


;--}
  
;{++ _intfunc

	.sdreg	29
_intfunc:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  sub sp,sp,16
  lda t0,_intfunc.00012[gp]
  sto t0,0[sp]
  ldo t0,32[fp]
  sto t0,8[sp]
  bsr _printf
.00022:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 40,0
	.type	_intfunc,@function
	.size	_intfunc,$-_intfunc


;--}
  
;{++ _floatfunc

	.sdreg	29
_floatfunc:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  sub sp,sp,16
  lda t0,_floatfunc.00023[gp]
  sto t0,0[sp]
  ldh t0,32[fp]
  sto t0,8[sp]
  bsr _printf
.00033:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 96,0
	.type	_floatfunc,@function
	.size	_floatfunc,$-_floatfunc


;--}
       
;{++ _main00175

	.sdreg	29
_main00175:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  ldo t5,0x5630[gp]
  ldo t6,0x5610[gp]
; charfunc('a');
  ldi t4,97
  ldi t2,97
  mov t1,t6
  mov t3,t6
  sub sp,sp,8
  ldi t0,97
  sto t0,0[sp]
  bsr _charfunc
; charfunc(98);
  sub sp,sp,8
  ldi t0,98
  sto t0,0[sp]
  bsr _charfunc
; charfunc(99.0);
  sub sp,sp,8
  sto t5,0[sp]
  bsr _charfunc
; intfunc('a');
  sub sp,sp,8
  ldi t0,97
  sto t0,0[sp]
  bsr _intfunc
; intfunc(98);
  sub sp,sp,8
  ldi t0,98
  sto t0,0[sp]
  bsr _intfunc
; intfunc(99.0);
  sub sp,sp,8
  sto t5,0[sp]
  bsr _intfunc
; floatfunc('a');
  sub sp,sp,8
  ldi t0,97
  sto t0,0[sp]
  bsr _floatfunc
; floatfunc(98);
  sub sp,sp,8
  ldi t0,98
  sto t0,0[sp]
  bsr _floatfunc
; floatfunc(99.0);
  sub sp,sp,8
  sto t5,0[sp]
  bsr _floatfunc
; printf("%d %d\n", b, c);
  sub sp,sp,24
  lda t0,_main00175.00034[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  ldw t0,-4[fp]
  sto t0,16[sp]
  bsr _printf
  add sp,sp,8
; printf("%d %d\n", d, e);
  sub sp,sp,24
  lda t0,_main00175.00035[gp]
  sto t0,0[sp]
  sto t4,8[sp]
  sto t2,16[sp]
  bsr _printf
  add sp,sp,8
; printf("%f %f\n", f, g);
  sub sp,sp,24
  lda t0,_main00175.00036[gp]
  sto t0,0[sp]
  sto t1,8[sp]
  sto t3,16[sp]
  bsr _printf
  add sp,sp,8
; return 0;
  mov a0,r0
.00047:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00175,@function
	.size	_main00175,$-_main00175


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00175.00042,@object
	.size	_main00175.00042,8
_main00175.00042:
	.4byte	0x00000000,0x40584000
	.align	8



	.type	_charfunc.00001,@object
	.size	_charfunc.00001,20
_charfunc.00001: ; char: %c

	.2byte	99,104,97,114,58,32,37,99
	.2byte	10,0

	.type	_intfunc.00012,@object
	.size	_intfunc.00012,18
_intfunc.00012: ; int: %d

	.2byte	105,110,116,58,32,37,100,10
	.2byte	0

	.type	_floatfunc.00023,@object
	.size	_floatfunc.00023,22
_floatfunc.00023: ; float: %f

	.2byte	102,108,111,97,116,58,32,37
	.2byte	102,10,0

	.type	_main00175.00034,@object
	.size	_main00175.00034,14
_main00175.00034: ; %d %d

	.2byte	37,100,32,37,100,10,0

	.type	_main00175.00035,@object
	.size	_main00175.00035,14
_main00175.00035: ; %d %d

	.2byte	37,100,32,37,100,10,0

	.type	_main00175.00036,@object
	.size	_main00175.00036,14
_main00175.00036: ; %f %f

	.2byte	37,102,32,37,102,10,0
	.extern	__Files
	.global	_main00175
	.global	_getc
	.global	_gets
	.global	_putc
	.global	_floatfunc
	.global	_puts
	.global	_getchar
	.global	_freopen
	.global	_tmpfile
	.global	_putchar
	.global	_intfunc
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
	.global	_charfunc
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
