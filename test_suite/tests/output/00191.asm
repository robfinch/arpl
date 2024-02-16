                                                                                                                                                   
	.text
 
;{++ _main00191

	.sdreg	29
_main00191:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; for (a = 0; a < 2; a++)
  mov t0,r0
  ldi t1,2
  bge t0,t1,.00016
; int b = a;
  sto t0,-16[fp]
.00016:
; printf("it's all good\n");
  sub sp,sp,8
  lda t0,_main00191.00001[gp]
  sto t0,0[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00014:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00191,@function
	.size	_main00191,$-_main00191


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00191.00001,@object
	.size	_main00191.00001,30
_main00191.00001: ; it's all good

	.2byte	105,116,39,115,32,97,108,108
	.2byte	32,103,111,111,100,10,0
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
	.global	_feof
	.global	_main00191
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
