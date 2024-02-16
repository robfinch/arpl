                                                                                                                                                  	.bss
	.align 4
__noname_var0:
	.type	__noname_var0,@object
	.size	__noname_var0,2

 
	.text
   
;{++ _main00198

	.sdreg	29
_main00198:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; printf("a=%d\n", a);
  sub sp,sp,16
  lda t0,_main00198.00001[gp]
  sto t0,0[sp]
  sto r0,8[sp]
  bsr _printf
; printf("b=%d\n", b);
  sub sp,sp,16
  lda t0,_main00198.00002[gp]
  sto t0,0[sp]
  ldi t0,1
  sto t0,8[sp]
  bsr _printf
; printf("c=%d\n", c);
  sub sp,sp,16
  lda t0,_main00198.00003[gp]
  sto t0,0[sp]
  ldi t0,2
  sto t0,8[sp]
  bsr _printf
; printf("e=%d\n", e);
  sub sp,sp,16
  lda t0,_main00198.00004[gp]
  sto t0,0[sp]
  sto r0,8[sp]
  bsr _printf
; printf("f=%d\n", f);
  sub sp,sp,16
  lda t0,_main00198.00005[gp]
  sto t0,0[sp]
  ldi t0,1
  sto t0,8[sp]
  bsr _printf
; printf("g=%d\n", g);
  sub sp,sp,16
  lda t0,_main00198.00006[gp]
  sto t0,0[sp]
  ldi t0,2
  sto t0,8[sp]
  bsr _printf
; printf("i=%d\n", i);
  sub sp,sp,16
  lda t0,_main00198.00007[gp]
  sto t0,0[sp]
  sto r0,8[sp]
  bsr _printf
; printf("j=%d\n", j);
  sub sp,sp,16
  lda t0,_main00198.00008[gp]
  sto t0,0[sp]
  ldi t0,1
  sto t0,8[sp]
  bsr _printf
; printf("k=%d\n", k);
  sub sp,sp,16
  lda t0,_main00198.00009[gp]
  sto t0,0[sp]
  ldi t0,2
  sto t0,8[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00019:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00198,@function
	.size	_main00198,$-_main00198


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00198.00001,@object
	.size	_main00198.00001,12
_main00198.00001: ; a=%d

	.2byte	97,61,37,100,10,0

	.type	_main00198.00002,@object
	.size	_main00198.00002,12
_main00198.00002: ; b=%d

	.2byte	98,61,37,100,10,0

	.type	_main00198.00003,@object
	.size	_main00198.00003,12
_main00198.00003: ; c=%d

	.2byte	99,61,37,100,10,0

	.type	_main00198.00004,@object
	.size	_main00198.00004,12
_main00198.00004: ; e=%d

	.2byte	101,61,37,100,10,0

	.type	_main00198.00005,@object
	.size	_main00198.00005,12
_main00198.00005: ; f=%d

	.2byte	102,61,37,100,10,0

	.type	_main00198.00006,@object
	.size	_main00198.00006,12
_main00198.00006: ; g=%d

	.2byte	103,61,37,100,10,0

	.type	_main00198.00007,@object
	.size	_main00198.00007,12
_main00198.00007: ; i=%d

	.2byte	105,61,37,100,10,0

	.type	_main00198.00008,@object
	.size	_main00198.00008,12
_main00198.00008: ; j=%d

	.2byte	106,61,37,100,10,0

	.type	_main00198.00009,@object
	.size	_main00198.00009,12
_main00198.00009: ; k=%d

	.2byte	107,61,37,100,10,0
	.extern	__Files
	.global	_getc
	.global	_main00198
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
	.global	__noname_var0
	.extern	_start_bss
	.extern	_start_rodata
