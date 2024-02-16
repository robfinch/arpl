                                                                                                                                                   
	.text

;{++ _fred

	.sdreg	29
_fred:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; switch (x)
  ldo t0,32[fp]
; case 1: printf("1\n"); return;
  bne t0,1,.00029
  sub sp,sp,8
  lda t0,_fred.00001[gp]
  sto t0,0[sp]
  bsr _printf
.00029:
  bne t0,2,.00031
.00031:
  bne t0,3,.00026
.00026:
; printf("out\n");
  sub sp,sp,8
  lda t0,_fred.00007[gp]
  sto t0,0[sp]
  bsr _printf
.00025:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 40,0
	.type	_fred,@function
	.size	_fred,$-_fred


;--}
 
;{++ _main00193

	.sdreg	29
_main00193:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; fred(1);
  sub sp,sp,8
  ldi t0,1
  sto t0,0[sp]
  bsr _fred
; fred(2);
  sub sp,sp,8
  ldi t0,2
  sto t0,0[sp]
  bsr _fred
; fred(3);
  sub sp,sp,8
  ldi t0,3
  sto t0,0[sp]
  bsr _fred
; return 0;
  mov a0,r0
.00043:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00193,@function
	.size	_main00193,$-_main00193


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_fred.00001,@object
	.size	_fred.00001,6
_fred.00001: ; 1

	.2byte	49,10,0

	.type	_fred.00003,@object
	.size	_fred.00003,6
_fred.00003: ; 2

	.2byte	50,10,0

	.type	_fred.00005,@object
	.size	_fred.00005,6
_fred.00005: ; 3

	.2byte	51,10,0

	.type	_fred.00007,@object
	.size	_fred.00007,10
_fred.00007: ; out

	.2byte	111,117,116,10,0
	.extern	__Files
	.global	_fred
	.global	_main00193
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
