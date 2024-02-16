                                                                                                                                                   
	.text

;{++ _myfunc

	.sdreg	29
_myfunc:
  mul t0,t0,t0
  mul a0,t0,t0
.00010:
  rts 
	.type	_myfunc,@function
	.size	_myfunc,$-_myfunc


;--}
  
;{++ _vfunc

	.sdreg	29
_vfunc:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  sub sp,sp,16
  lda t0,_vfunc.00011[gp]
  sto t0,0[sp]
  ldo t0,32[fp]
  sto t0,8[sp]
  bsr _printf
.00021:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 40,0
	.type	_vfunc,@function
	.size	_vfunc,$-_vfunc


;--}
 
;{++ _qfunc

	.sdreg	29
_qfunc:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  sub sp,sp,8
  lda t0,_qfunc.00022[gp]
  sto t0,0[sp]
  bsr _printf
.00032:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_qfunc,@function
	.size	_qfunc,$-_qfunc


;--}
   
;{++ _zfunc

	.sdreg	29
_zfunc:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
.00042:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_zfunc,@function
	.size	_zfunc,$-_zfunc


;--}
 
;{++ _main00159

	.sdreg	29
_main00159:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; printf("%d\n", myfunc(3));
  sub sp,sp,16
  lda t0,_main00159.00043[gp]
  sto t0,0[sp]
  sub sp,sp,8
  ldi t0,3
  sto t0,0[sp]
  bsr _myfunc
  sto a0,8[sp]
  bsr _printf
; printf("%d\n", myfunc(4));
  sub sp,sp,16
  lda t0,_main00159.00044[gp]
  sto t0,0[sp]
  sub sp,sp,8
  ldi t0,4
  sto t0,0[sp]
  bsr _myfunc
  sto a0,8[sp]
  bsr _printf
; vfunc(1234);
  sub sp,sp,8
  ldi t0,1234
  sto t0,0[sp]
  bsr _vfunc
; qfunc();
  bsr _qfunc
; return 0;
  mov a0,r0
.00054:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00159,@function
	.size	_main00159,$-_main00159


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_vfunc.00011,@object
	.size	_vfunc.00011,12
_vfunc.00011: ; a=%d

	.2byte	97,61,37,100,10,0

	.type	_qfunc.00022,@object
	.size	_qfunc.00022,18
_qfunc.00022: ; qfunc()

	.2byte	113,102,117,110,99,40,41,10
	.2byte	0

	.type	_main00159.00043,@object
	.size	_main00159.00043,8
_main00159.00043: ; %d

	.2byte	37,100,10,0

	.type	_main00159.00044,@object
	.size	_main00159.00044,8
_main00159.00044: ; %d

	.2byte	37,100,10,0
	.extern	__Files
	.global	_getc
	.global	_main00159
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
	.global	_qfunc
	.global	_fputc
	.global	_vfunc
	.global	_zfunc
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
	.global	_myfunc
	.global	_setjmp
	.global	_printf
	.global	_perror
	.global	_feof
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
