
;{++ _main00185

	.text

	.sdreg	29
_main00185:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
; for (Count = 0; Count < 10; Count++)
  mov s0,r0
  ldi t1,10
  bge s0,t1,.00023
.00022:
; printf("%d: %d\n", Count, Array[Count]);
  sub sp,sp,24
  lda t0,_main00185.00002[gp]
  sto t0,0[sp]
  sto s0,8[sp]
  lda t0,-88[fp]
  ldo t0,0[t0+s0*]
  sto t0,16[sp]
  bsr _printf
  add sp,sp,8
.00024:
.00023:
; for (Count = 0; Count < 10; Count++)
  mov s0,r0
  bge s0,t1,.00026
.00025:
; printf("%d: %d\n", Count, Array2[Count]);
  sub sp,sp,24
  lda t0,_main00185.00005[gp]
  sto t0,0[sp]
  sto s0,8[sp]
  lda t0,-168[fp]
  ldo t0,0[t0+s0*]
  sto t0,16[sp]
  bsr _printf
  add sp,sp,8
.00027:
  iblt s0,t1,.00025
.00026:
; return 0;
  mov a0,r0
.00021:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00185,@function
	.size	_main00185,$-_main00185


;--}

	.rodata
	.align	6

	.align	8




	.align	6
	.type	_main00185.00002,@object
	.size	_main00185.00002,16
_main00185.00002: ; %d: %d

	.2byte	37,100,58,32,37,100,10,0



	.type	_main00185.00005,@object
	.size	_main00185.00005,16
_main00185.00005: ; %d: %d

	.2byte	37,100,58,32,37,100,10,0

	.align	8

	.align 4
_Array:
	.type	_Array,@object
	.size	_Array,80
	.8byte	12,34,56,78,90
	.8byte	123,456,789,8642,9753


	.align 4
_Array2:
	.type	_Array2,@object
	.size	_Array2,80
	.8byte	12,34,56,78,90
	.8byte	123,456,789,8642,9753
	.8byte	0
	.extern	__Files
	.global	_getc
	.global	_main00185
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
