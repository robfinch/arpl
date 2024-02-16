                                                                                                                                                  
	.text

;{++ _main00212

	.sdreg	29
_main00212:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  sub sp,sp,8
  lda t0,_main00212.00001[gp]
  sto t0,0[sp]
  bsr _printf
.00011:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00212,@function
	.size	_main00212,$-_main00212


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00212.00001,@object
	.size	_main00212.00001,50
_main00212.00001: ; KO no __*LP*__ defined.

	.2byte	75,79,32,110,111,32,95,95
	.2byte	42,76,80,42,95,95,32,100
	.2byte	101,102,105,110,101,100,46,10
	.2byte	0
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
	.global	_perror
	.global	_main00212
	.global	_feof
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
