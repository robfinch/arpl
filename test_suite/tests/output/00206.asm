                                                                                                                                                  
	.text

;{++ _main00206

	.sdreg	29
_main00206:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; printf("abort = %s\n", "111");
  sub sp,sp,16
  lda t0,_main00206.00001[gp]
  sto t0,0[sp]
  lda t0,_main00206.00002[gp]
  sto t0,8[sp]
  bsr _printf
; printf("abort = %s\n", "222");
  sub sp,sp,16
  lda t0,_main00206.00003[gp]
  sto t0,0[sp]
  lda t0,_main00206.00004[gp]
  sto t0,8[sp]
  bsr _printf
; printf("abort = %s\n", "333");
  sub sp,sp,16
  lda t0,_main00206.00005[gp]
  sto t0,0[sp]
  lda t0,_main00206.00006[gp]
  sto t0,8[sp]
  bsr _printf
  sub sp,sp,16
  lda t0,_main00206.00007[gp]
  sto t0,0[sp]
  lda t0,_main00206.00008[gp]
  sto t0,8[sp]
  bsr _printf
  sub sp,sp,16
  lda t0,_main00206.00009[gp]
  sto t0,0[sp]
  lda t0,_main00206.00010[gp]
  sto t0,8[sp]
  bsr _printf
.00020:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00206,@function
	.size	_main00206,$-_main00206


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00206.00001,@object
	.size	_main00206.00001,24
_main00206.00001: ; abort = %s

	.2byte	97,98,111,114,116,32,61,32
	.2byte	37,115,10,0

	.type	_main00206.00002,@object
	.size	_main00206.00002,8
_main00206.00002: ; 111

	.2byte	49,49,49,0

	.type	_main00206.00003,@object
	.size	_main00206.00003,24
_main00206.00003: ; abort = %s

	.2byte	97,98,111,114,116,32,61,32
	.2byte	37,115,10,0

	.type	_main00206.00004,@object
	.size	_main00206.00004,8
_main00206.00004: ; 222

	.2byte	50,50,50,0

	.type	_main00206.00005,@object
	.size	_main00206.00005,24
_main00206.00005: ; abort = %s

	.2byte	97,98,111,114,116,32,61,32
	.2byte	37,115,10,0

	.type	_main00206.00006,@object
	.size	_main00206.00006,8
_main00206.00006: ; 333

	.2byte	51,51,51,0

	.type	_main00206.00007,@object
	.size	_main00206.00007,24
_main00206.00007: ; abort = %s

	.2byte	97,98,111,114,116,32,61,32
	.2byte	37,115,10,0

	.type	_main00206.00008,@object
	.size	_main00206.00008,8
_main00206.00008: ; 333

	.2byte	51,51,51,0

	.type	_main00206.00009,@object
	.size	_main00206.00009,24
_main00206.00009: ; abort = %s

	.2byte	97,98,111,114,116,32,61,32
	.2byte	37,115,10,0

	.type	_main00206.00010,@object
	.size	_main00206.00010,8
_main00206.00010: ; 333

	.2byte	51,51,51,0
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
	.global	_main00206
	.global	_feof
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
