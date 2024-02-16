                                                                                                                                                    
	.text

;{++ _main00157

	.sdreg	29
_main00157:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; for (Count = 1; Count <= 10; Count++)
  ldi t0,1
  ldi t1,10
  bgt t0,t1,.00019
; Array[Count-1] = Count * Count;
  sub t0,t0,1
  mul t0,t0,t0
  sto t0,0[t1+t0*]
.00020:
.00019:
; for (Count = 0; Count < 10; Count++)
  mov t0,r0
  bge t0,t1,.00022
; printf("%d\n", Array[Count]);
  sub sp,sp,16
  lda t0,_main00157.00001[gp]
  sto t0,0[sp]
  ldo t0,0[t1+t0*]
  sto t0,8[sp]
  bsr _printf
.00023:
.00022:
; return 0;
  mov a0,r0
.00017:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00157,@function
	.size	_main00157,$-_main00157


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00157.00001,@object
	.size	_main00157.00001,8
_main00157.00001: ; %d

	.2byte	37,100,10,0
	.extern	__Files
	.global	_main00157
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
