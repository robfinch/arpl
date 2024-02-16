                                                                                                                                                   
	.text

;{++ _fred

	.sdreg	29
_fred:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  sub sp,sp,8
  lda t0,_fred.00001[gp]
  sto t0,0[sp]
  bsr _printf
.00011:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_fred,@function
	.size	_fred,$-_fred


;--}
 
;{++ _main00190

	.sdreg	29
_main00190:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; fred();
  bsr _fred
; return 0;
  mov a0,r0
.00021:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00190,@function
	.size	_main00190,$-_main00190


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_fred.00001,@object
	.size	_fred.00001,8
_fred.00001: ; yo

	.2byte	121,111,10,0
	.extern	__Files
	.global	_fred
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
	.global	_main00190
	.global	_feof
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
