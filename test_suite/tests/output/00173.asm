                                                                                                                                                   
	.text
       
;{++ _main00173

	.sdreg	29
_main00173:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,80
  sto s0,[sp]
  sto s1,8[sp]
  sto s2,16[sp]
  sto s3,24[sp]
  sto s4,32[sp]
  sto s5,40[sp]
  ldo s1,1[fp]
  ldo t1,[fp]
  lda s4,-54[fp]
; int x = 'a';
  ldi s5,97
  stw s5,-10[fp]
  lda t0,_main00173.00001[gp]
  sto t0,-18[fp]
  add s3,s4,r0
  mov s2,s1
; printf("%s\n", a);
  sub sp,sp,16
  lda t0,_main00173.00002[gp]
  sto t0,0[sp]
  sto s1,8[sp]
  bsr _printf
; c = *a;
  ldo t0,[s1]
  sto t0,-26[fp]
; for (b = a; *b != 0; b++)
  mov s0,s1
  ldw t1,[s0]
  beq t1,r0,.00021
.00020:
; printf("%c: %d\n", *b, *b);
  sub sp,sp,24
  lda t0,_main00173.00003[gp]
  sto t0,0[sp]
  ldw t0,[s0]
  sto t0,8[sp]
  ldw t0,[s0]
  sto t0,16[sp]
  bsr _printf
  add sp,sp,8
.00022:
  add s0,s0,2
  ldw t1,[s0]
  bnez t1,.00020
.00021:
; while (*src != 0)
  ldw t1,[s2]
  beq t1,r0,.00024
.00023:
; *dest++ = *src++;
  ldw t0,[s2]
  stw t0,[s3]
  add s3,s3,2
  add s2,s2,2
  ldw t1,[s2]
  bnez t1,.00023
.00024:
; *dest = 0;
  stw r0,[s3]
; printf("copied string is %s\n", destarray);
  sub sp,sp,16
  lda t0,_main00173.00004[gp]
  sto t0,0[sp]
  sto s4,8[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00019:
  ldo s0,[sp]
  ldo s1,8[sp]
  ldo s2,16[sp]
  ldo s3,24[sp]
  ldo s4,32[sp]
  ldo s5,40[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00173,@function
	.size	_main00173,$-_main00173


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00173.00001,@object
	.size	_main00173.00001,12
_main00173.00001: ; hello

	.2byte	104,101,108,108,111,0

	.type	_main00173.00002,@object
	.size	_main00173.00002,8
_main00173.00002: ; %s

	.2byte	37,115,10,0

	.type	_main00173.00003,@object
	.size	_main00173.00003,16
_main00173.00003: ; %c: %d

	.2byte	37,99,58,32,37,100,10,0

	.type	_main00173.00004,@object
	.size	_main00173.00004,42
_main00173.00004: ; copied string is %s

	.2byte	99,111,112,105,101,100,32,115
	.2byte	116,114,105,110,103,32,105,115
	.2byte	32,37,115,10,0
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
	.global	_main00173
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
