                                                                                                                                                    
	.text

;{++ _main00186

	.sdreg	29
_main00186:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; for (Count = 1; Count <= 20; Count++)
  ldi t0,1
  ldi t1,20
  bgt t0,t1,.00017
.00016:
; sprintf(Buf, "->%02d<-\n", Count);
  sub sp,sp,24
  sto t1,0[sp]
  lda t0,_main00186.00001[gp]
  sto t0,8[sp]
  sto t0,16[sp]
  bsr _sprintf
; printf("%s", Buf);
  sub sp,sp,16
  lda t0,_main00186.00002[gp]
  sto t0,0[sp]
  sto t1,8[sp]
  bsr _printf
.00017:
; return 0;
  mov a0,r0
.00015:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00186,@function
	.size	_main00186,$-_main00186


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00186.00001,@object
	.size	_main00186.00001,20
_main00186.00001: ; ->%02d<-

	.2byte	45,62,37,48,50,100,60,45
	.2byte	10,0

	.type	_main00186.00002,@object
	.size	_main00186.00002,6
_main00186.00002: ; %s

	.2byte	37,115,0
	.extern	__Files
	.global	_getc
	.global	_main00186
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
