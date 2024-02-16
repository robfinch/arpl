                                                                                                                                                    
	.text

;{++ _main00201

	.sdreg	29
_main00201:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; int xy = 42;
  ldi t0,42
  sto t0,-8[fp]
; printf("%d\n", A#"B"(x));
  sub sp,sp,16
  lda t0,_main00201.00001[gp]
  sto t0,0[sp]
  lda t0,_main00201.09999[r56]
  sto t0,8[sp]
  bsr _printf
  sub sp,sp,8
  lda t1,_main00201.09999[r56]
  sto t1,0[sp]
; return 0;
  mov a0,r0
.00012:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00201,@function
	.size	_main00201,$-_main00201


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00201.00001,@object
	.size	_main00201.00001,8
_main00201.00001: ; %d

	.2byte	37,100,10,0

	.type	_main00201.00002,@object
	.size	_main00201.00002,4
_main00201.00002: ; B

	.2byte	66,0
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
	.global	_main00201
	.global	_perror
	.global	_feof
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
