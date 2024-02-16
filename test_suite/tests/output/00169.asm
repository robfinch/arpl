                                                                                                                                                      
	.text

;{++ _main00169

	.sdreg	29
_main00169:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,56
  sto s0,[sp]
  sto s1,8[sp]
  sto s2,16[sp]
; for (x = 0; x < 2; x++)
  mov s2,r0
  ldi t1,2
  bge s2,t1,.00022
.00021:
; for (y = 0; y < 3; y++)
  mov s1,r0
  ldi t1,3
  bge s1,t1,.00025
.00024:
; for (z = 0; z < 3; z++)
  mov s0,r0
  bge s0,t1,.00028
.00027:
; printf("%d %d %d\n", x, y, z);
  sub sp,sp,32
  lda t0,_main00169.00001[gp]
  sto t0,0[sp]
  sto s2,8[sp]
  sto s1,16[sp]
  sto s0,24[sp]
  bsr _printf
  add sp,sp,32
.00029:
  iblt s0,t1,.00027
.00028:
.00026:
  iblt s1,t1,.00024
.00025:
.00023:
  ldi t1,2
  iblt s2,t1,.00021
.00022:
; return 0;
  mov a0,r0
.00020:
  ldo s0,[sp]
  ldo s1,8[sp]
  ldo s2,16[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00169,@function
	.size	_main00169,$-_main00169


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00169.00001,@object
	.size	_main00169.00001,20
_main00169.00001: ; %d %d %d

	.2byte	37,100,32,37,100,32,37,100
	.2byte	10,0
	.extern	__Files
	.global	_getc
	.global	_main00169
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
	.extern	_printf
	.global	_perror
	.global	_feof
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
