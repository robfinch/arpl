                                                                                                                                                   
	.text

;{++ _main00192

	.sdreg	29
_main00192:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
; int Count = 0;
  mov s0,r0
; Count++;
  add s0,s0,1
; printf("%d\n", Count);
  sub sp,sp,16
  lda t0,_main00192.00001[gp]
  sto t0,0[sp]
  sto s0,8[sp]
  bsr _printf
; if (Count >= 10)
  ldi t1,10
  blt s0,t1,.00020
.00020:
; return 0;
  mov a0,r0
.00016:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00192,@function
	.size	_main00192,$-_main00192


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00192.00001,@object
	.size	_main00192.00001,8
_main00192.00001: ; %d

	.2byte	37,100,10,0
	.extern	__Files
	.global	_main00192
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
