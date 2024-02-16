                                                                                                                                                  
	.text

;{++ _main00177

	.sdreg	29
_main00177:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; printf("%d\n", '\1');
  sub sp,sp,16
  lda t0,_main00177.00001[gp]
  sto t0,0[sp]
  ldi t0,1
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", '\10');
  sub sp,sp,16
  lda t0,_main00177.00002[gp]
  sto t0,0[sp]
  ldi t0,10
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", '\100');
  sub sp,sp,16
  lda t0,_main00177.00003[gp]
  sto t0,0[sp]
  ldi t0,100
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", '\x01');
  sub sp,sp,16
  lda t0,_main00177.00004[gp]
  sto t0,0[sp]
  ldi t0,1
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", '\x0e');
  sub sp,sp,16
  lda t0,_main00177.00005[gp]
  sto t0,0[sp]
  ldi t0,14
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", '\x10');
  sub sp,sp,16
  lda t0,_main00177.00006[gp]
  sto t0,0[sp]
  ldi t0,16
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", '\x40');
  sub sp,sp,16
  lda t0,_main00177.00007[gp]
  sto t0,0[sp]
  ldi t0,64
  sto t0,8[sp]
  bsr _printf
; printf("test \x40\n");
  sub sp,sp,8
  lda t0,_main00177.00008[gp]
  sto t0,0[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00018:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00177,@function
	.size	_main00177,$-_main00177


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00177.00001,@object
	.size	_main00177.00001,8
_main00177.00001: ; %d

	.2byte	37,100,10,0

	.type	_main00177.00002,@object
	.size	_main00177.00002,8
_main00177.00002: ; %d

	.2byte	37,100,10,0

	.type	_main00177.00003,@object
	.size	_main00177.00003,8
_main00177.00003: ; %d

	.2byte	37,100,10,0

	.type	_main00177.00004,@object
	.size	_main00177.00004,8
_main00177.00004: ; %d

	.2byte	37,100,10,0

	.type	_main00177.00005,@object
	.size	_main00177.00005,8
_main00177.00005: ; %d

	.2byte	37,100,10,0

	.type	_main00177.00006,@object
	.size	_main00177.00006,8
_main00177.00006: ; %d

	.2byte	37,100,10,0

	.type	_main00177.00007,@object
	.size	_main00177.00007,8
_main00177.00007: ; %d

	.2byte	37,100,10,0

	.type	_main00177.00008,@object
	.size	_main00177.00008,16
_main00177.00008: ; test @

	.2byte	116,101,115,116,32,64,10,0
	.extern	__Files
	.global	_getc
	.global	_main00177
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
