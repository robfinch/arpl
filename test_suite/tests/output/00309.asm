
	.text
 
;{++ _main00309

	.sdreg	29
_main00309:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  lda t0,1[fp]
  ldw t0,[t0]
  beqz t0,.00018
.00017:
  sub sp,sp,16
  lda t0,_main00309.00002[gp]
  sto t0,0[sp]
  ldw t0,[t0]
  sto t0,8[sp]
  bsr _printf
  add t0,t0,2
  ldw t0,[t0]
  bnez t0,.00017
.00018:
; printf("\n");
  sub sp,sp,8
  lda t0,_main00309.00003[gp]
  sto t0,0[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00016:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00309,@function
	.size	_main00309,$-_main00309


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00309.00002,@object
	.size	_main00309.00002,12
_main00309.00002: ; %04X 

	.2byte	37,48,52,88,32,0

	.type	_main00309.00003,@object
	.size	_main00309.00003,4
_main00309.00003:

	.2byte	10,0

	.align	8

	.align 4
_s:
	.type	_s,@object
	.size	_s,2
	.2byte	0
	.extern	__Files
	.global	_main00309
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
