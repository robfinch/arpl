                                                                                                                                                  
	.text

;{++ _fred

	.sdreg	29
_fred:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; printf("fred\n");
  sub sp,sp,8
  lda t0,_fred.00001[gp]
  sto t0,0[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00011:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_fred,@function
	.size	_fred,$-_fred


;--}
 
;{++ _joe

	.sdreg	29
_joe:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; printf("joe\n");
  sub sp,sp,8
  lda t0,_joe.00012[gp]
  sto t0,0[sp]
  bsr _printf
; return 1;
  ldi a0,1
.00022:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_joe,@function
	.size	_joe,$-_joe


;--}
 
;{++ _main00196

	.sdreg	29
_main00196:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; printf("%d\n", fred() && joe());
  sub sp,sp,16
  lda t0,_main00196.00023[gp]
  sto t0,0[sp]
  bsr _fred
  bsr _joe
  sto a0,8[sp]
  bsr _printf
; printf("%d\n", fred() || joe());
  sub sp,sp,16
  lda t0,_main00196.00024[gp]
  sto t0,0[sp]
  bsr _fred
  bsr _joe
  sto a0,8[sp]
  bsr _printf
; printf("%d\n", joe() && fred());
  sub sp,sp,16
  lda t0,_main00196.00025[gp]
  sto t0,0[sp]
  bsr _joe
  bsr _fred
  sto a0,8[sp]
  bsr _printf
; printf("%d\n", joe() || fred());
  sub sp,sp,16
  lda t0,_main00196.00026[gp]
  sto t0,0[sp]
  bsr _joe
  bsr _fred
  sto a0,8[sp]
  bsr _printf
; printf("%d\n", fred() && (1 + joe()));
  sub sp,sp,16
  lda t0,_main00196.00027[gp]
  sto t0,0[sp]
  bsr _fred
  sto t0,-32[fp]
  bsr _joe
  sto a0,8[sp]
  bsr _printf
; printf("%d\n", fred() || (0 + joe()));
  sub sp,sp,16
  lda t0,_main00196.00028[gp]
  sto t0,0[sp]
  bsr _fred
  bsr _joe
  sto a0,8[sp]
  bsr _printf
; printf("%d\n", joe() && (0 + fred()));
  sub sp,sp,16
  lda t0,_main00196.00029[gp]
  sto t0,0[sp]
  bsr _joe
  bsr _fred
  sto a0,8[sp]
  bsr _printf
; printf("%d\n", joe() || (1 + fred()));
  sub sp,sp,16
  lda t0,_main00196.00030[gp]
  sto t0,0[sp]
  bsr _joe
  sto t0,-32[fp]
  bsr _fred
  sto a0,8[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00048:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00196,@function
	.size	_main00196,$-_main00196


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_fred.00001,@object
	.size	_fred.00001,12
_fred.00001: ; fred

	.2byte	102,114,101,100,10,0

	.type	_joe.00012,@object
	.size	_joe.00012,10
_joe.00012: ; joe

	.2byte	106,111,101,10,0

	.type	_main00196.00023,@object
	.size	_main00196.00023,8
_main00196.00023: ; %d

	.2byte	37,100,10,0

	.type	_main00196.00024,@object
	.size	_main00196.00024,8
_main00196.00024: ; %d

	.2byte	37,100,10,0

	.type	_main00196.00025,@object
	.size	_main00196.00025,8
_main00196.00025: ; %d

	.2byte	37,100,10,0

	.type	_main00196.00026,@object
	.size	_main00196.00026,8
_main00196.00026: ; %d

	.2byte	37,100,10,0

	.type	_main00196.00027,@object
	.size	_main00196.00027,8
_main00196.00027: ; %d

	.2byte	37,100,10,0

	.type	_main00196.00028,@object
	.size	_main00196.00028,8
_main00196.00028: ; %d

	.2byte	37,100,10,0

	.type	_main00196.00029,@object
	.size	_main00196.00029,8
_main00196.00029: ; %d

	.2byte	37,100,10,0

	.type	_main00196.00030,@object
	.size	_main00196.00030,8
_main00196.00030: ; %d

	.2byte	37,100,10,0
	.extern	__Files
	.global	_fred
	.global	_getc
	.global	_main00196
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
	.global	_joe
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
