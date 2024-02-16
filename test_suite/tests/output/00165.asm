                                                                                                                                                  
	.text

;{++ _main00165

	.sdreg	29
_main00165:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; printf("%d\n", 12);
  sub sp,sp,16
  lda t0,_main00165.00001[gp]
  sto t0,0[sp]
  ldi t0,12
  sto t0,8[sp]
  bsr _printf
; printf("%d, %d, %d\n", (12*(1)), (12*(2)), (12*(3)));
  sub sp,sp,32
  lda t0,_main00165.00002[gp]
  sto t0,0[sp]
  ldi t0,12
  sto t0,8[sp]
  ldi t0,24
  sto t0,16[sp]
  ldi t0,36
  sto t0,24[sp]
  bsr _printf
  add sp,sp,16
; return 0;
  mov a0,r0
.00012:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00165,@function
	.size	_main00165,$-_main00165


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00165.00001,@object
	.size	_main00165.00001,8
_main00165.00001: ; %d

	.2byte	37,100,10,0

	.type	_main00165.00002,@object
	.size	_main00165.00002,24
_main00165.00002: ; %d, %d, %d

	.2byte	37,100,44,32,37,100,44,32
	.2byte	37,100,10,0
	.extern	__Files
	.global	_main00165
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
