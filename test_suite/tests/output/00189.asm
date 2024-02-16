                                                                                                                                                   
	.text

;{++ _fred

	.sdreg	29
_fred:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; printf("yo %d\n", p);
  sub sp,sp,16
  lda t0,_fred.00001[gp]
  sto t0,0[sp]
  ldo t0,32[fp]
  sto t0,8[sp]
  bsr _printf
; return 42;
  ldi a0,42
.00011:
  mov sp,fp
  ldo fp,[sp]
  rtd 40,0
	.type	_fred,@function
	.size	_fred,$-_fred


;--}
         
;{++ _main00189

	.sdreg	29
_main00189:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; fprintfptr(_Files[1], "%d\n", (*f)(24));
  sub sp,sp,24
  lda t1,00189.00000[gp]
  ldo t1,1[t1]
  sto t1,0[sp]
  lda t1,_main00189.00012[gp]
  sto t1,8[sp]
  sub sp,sp,8
  ldi t2,24
  sto t2,0[sp]
  sto a0,16[sp]
; return 0;
  mov a0,r0
.00022:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00189,@function
	.size	_main00189,$-_main00189


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_fred.00001,@object
	.size	_fred.00001,14
_fred.00001: ; yo %d

	.2byte	121,111,32,37,100,10,0

	.type	_main00189.00012,@object
	.size	_main00189.00012,8
_main00189.00012: ; %d

	.2byte	37,100,10,0
	.extern	__Files
	.global	_fred
	.global	_getc
	.global	_main00189
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
	.global	_fprintfptr
	.global	_clearerr
	.global	__Fgpos
	.global	_f
	.global	_fprintfptr
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
