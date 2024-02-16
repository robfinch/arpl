                                                                                                                                                   
	.text

;{++ _main00158

	.sdreg	29
_main00158:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
; for (Count = 0; Count < 4; Count++)
  mov s0,r0
  ldi t1,4
  bge s0,t1,.00029
.00028:
; printf("%d\n", Count);
  sub sp,sp,16
  lda t0,_main00158.00001[gp]
  sto t0,0[sp]
  sto s0,8[sp]
  bsr _printf
.00032:
  bne s0,1,.00034
; printf("%d\n", 1);
  sub sp,sp,16
  lda t0,_main00158.00002[gp]
  sto t0,0[sp]
  ldi t0,1
  sto t0,8[sp]
  bsr _printf
.00034:
  bne s0,2,.00007
.00007:
.00031:
.00030:
  ldi t1,4
  iblt s0,t1,.00028
.00029:
; return 0;
  mov a0,r0
.00027:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00158,@function
	.size	_main00158,$-_main00158


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00158.00001,@object
	.size	_main00158.00001,8
_main00158.00001: ; %d

	.2byte	37,100,10,0

	.type	_main00158.00002,@object
	.size	_main00158.00002,8
_main00158.00002: ; %d

	.2byte	37,100,10,0

	.type	_main00158.00004,@object
	.size	_main00158.00004,8
_main00158.00004: ; %d

	.2byte	37,100,10,0

	.type	_main00158.00006,@object
	.size	_main00158.00006,8
_main00158.00006: ; %d

	.2byte	37,100,10,0
	.extern	__Files
	.global	_getc
	.global	_main00158
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
