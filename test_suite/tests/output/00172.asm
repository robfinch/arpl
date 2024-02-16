                                                                                                                                                      
	.text

;{++ _main00172

	.sdreg	29
_main00172:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  lda t2,-16[fp]
  lda t3,-8[fp]
  lda t1,-8[fp]
; a = 12;
  ldi t0,12
  sto t0,[t3]
; b = 34;
  ldi t0,34
  sto t0,[t2]
; printf("%d\n", *d);
  sub sp,sp,16
  lda t0,_main00172.00001[gp]
  sto t0,0[sp]
  ldo t0,[t1]
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", *e);
  sub sp,sp,16
  lda t0,_main00172.00002[gp]
  sto t0,0[sp]
  ldo t0,[t0]
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", d == e);
  sub sp,sp,16
  lda t0,_main00172.00003[gp]
  sto t0,0[sp]
  zseq t0,t1,t0,1
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", d != e);
  sub sp,sp,16
  lda t0,_main00172.00004[gp]
  sto t0,0[sp]
  zsne t0,t1,t0,1
  sto t0,8[sp]
  bsr _printf
; d = e;
  mov t1,t0
; printf("%d\n", d == e);
  sub sp,sp,16
  lda t0,_main00172.00005[gp]
  sto t0,0[sp]
  zseq t0,t1,t0,1
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", d != e);
  sub sp,sp,16
  lda t0,_main00172.00006[gp]
  sto t0,0[sp]
  zsne t0,t1,t0,1
  sto t0,8[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00024:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00172,@function
	.size	_main00172,$-_main00172


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00172.00001,@object
	.size	_main00172.00001,8
_main00172.00001: ; %d

	.2byte	37,100,10,0

	.type	_main00172.00002,@object
	.size	_main00172.00002,8
_main00172.00002: ; %d

	.2byte	37,100,10,0

	.type	_main00172.00003,@object
	.size	_main00172.00003,8
_main00172.00003: ; %d

	.2byte	37,100,10,0

	.type	_main00172.00004,@object
	.size	_main00172.00004,8
_main00172.00004: ; %d

	.2byte	37,100,10,0

	.type	_main00172.00005,@object
	.size	_main00172.00005,8
_main00172.00005: ; %d

	.2byte	37,100,10,0

	.type	_main00172.00006,@object
	.size	_main00172.00006,8
_main00172.00006: ; %d

	.2byte	37,100,10,0
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
	.global	_main00172
	.global	_feof
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
