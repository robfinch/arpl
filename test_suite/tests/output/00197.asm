
	.text

;{++ _henry

	.sdreg	29
_henry:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; printf("%d\n", fred);
  sub sp,sp,16
  lda t0,_henry.00004[gp]
  sto t0,0[sp]
  ldo t0,[t0]
  sto t0,8[sp]
  bsr _printf
.00014:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_henry,@function
	.size	_henry,$-_henry


;--}
 
;{++ _main00197

	.sdreg	29
_main00197:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; printf("%d\n", fred);
  sub sp,sp,16
  lda t0,_main00197.00015[gp]
  sto t0,0[sp]
  ldo t0,[t0]
  sto t0,8[sp]
  bsr _printf
; henry();
  bsr _henry
  bsr _henry
  bsr _henry
  bsr _henry
; printf("%d\n", fred);
  sub sp,sp,16
  lda t0,_main00197.00016[gp]
  sto t0,0[sp]
  ldo t0,[t0]
  sto t0,8[sp]
  bsr _printf
; fred = 8901;
  ldi t0,8901
  sto t0,[t0]
; joe = 2345;
  ldi t0,2345
  sto t0,00197.00002[gp]
; printf("%d\n", fred);
  sub sp,sp,16
  lda t0,_main00197.00017[gp]
  sto t0,0[sp]
  ldo t0,[t0]
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", joe);
  sub sp,sp,16
  lda t0,_main00197.00018[gp]
  sto t0,0[sp]
  ldo t0,00197.00002[gp]
  sto t0,8[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00028:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00197,@function
	.size	_main00197,$-_main00197


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_henry.00004,@object
	.size	_henry.00004,8
_henry.00004: ; %d

	.2byte	37,100,10,0

	.type	_main00197.00015,@object
	.size	_main00197.00015,8
_main00197.00015: ; %d

	.2byte	37,100,10,0

	.type	_main00197.00016,@object
	.size	_main00197.00016,8
_main00197.00016: ; %d

	.2byte	37,100,10,0

	.type	_main00197.00017,@object
	.size	_main00197.00017,8
_main00197.00017: ; %d

	.2byte	37,100,10,0

	.type	_main00197.00018,@object
	.size	_main00197.00018,8
_main00197.00018: ; %d

	.2byte	37,100,10,0

	.align	2


	.bss
	.align	6
	.type	00197.00001,@object
	.size	00197.00001,8
00197.00001: ; _fred
	.byte	0,0,0,0,0,0,0,0

	.8byte	1234



	.bss
	.type	00197.00002,@object
	.size	00197.00002,8
00197.00002: ; _joe
	.byte	0,0,0,0,0,0,0,0





	.bss
	.align	6
	.type	00197.00003,@object
	.size	00197.00003,8
00197.00003: ; _fred
	.byte	0,0,0,0,0,0,0,0

	.8byte	4567
	.extern	__Files
	.global	_getc
	.global	_main00197
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
	.global	_henry
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
