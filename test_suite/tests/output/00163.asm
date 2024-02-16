                                                                                                                                                     	.bss
	.align 4
_bolshevic:
	.type	_bolshevic,@object
	.size	_bolshevic,24

    
	.text
 
;{++ _main00163

	.sdreg	29
_main00163:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,48
  sto s0,[sp]
  sto s1,8[sp]
; int a;
  lda s1,00163.00116[gp]
; a = 42;
  ldi t0,42
  sto t0,-8[fp]
; b = &a;
  lda s0,-8[fp]
; printf("a = %d\n", *b);
  sub sp,sp,16
  lda t0,_main00163.00001[gp]
  sto t0,0[sp]
  ldo t0,[s0]
  sto t0,8[sp]
  bsr _printf
; bolshevic.b = 34;
  lda t0,00163.00116[gp]
  ldi t1,34
  sto t1,8[t0]
; bolshevic.c = 56;
  lda t0,00163.00116[gp]
  ldi t1,56
  sto t1,16[t0]
; printf("bolshevic.a = %d\n", bolshevic.a);
  sub sp,sp,16
  lda t0,_main00163.00002[gp]
  sto t0,0[sp]
  lda t0,00163.00000[gp]
  sto t0,8[sp]
  bsr _printf
; printf("bolshevic.b = %d\n", bolshevic.b);
  sub sp,sp,16
  lda t0,_main00163.00003[gp]
  sto t0,0[sp]
  lda t0,00163.00116[gp]
  ldou t0,8[t0]
  sto t0,8[sp]
  bsr _printf
; printf("bolshevic.c = %d\n", bolshevic.c);
  sub sp,sp,16
  lda t0,_main00163.00004[gp]
  sto t0,0[sp]
  lda t0,00163.00116[gp]
  ldou t0,16[t0]
  sto t0,8[sp]
  bsr _printf
; printf("tsar->a = %d\n", tsar->a);
  sub sp,sp,8
  lda t0,_main00163.00005[gp]
  sto t0,0[sp]
  ldo t0,[s1]
  lea t1,0[sp]
  sto t1,8[sp]
  sto t0,0[sp]
  ldi t1,24
  sto t1,24
  bsr __aacpy
  ldi t1,0
  sto t1,8[sp]
  bsr _printf
; printf("tsar->b = %d\n", tsar->b);
  sub sp,sp,16
  lda t0,_main00163.00006[gp]
  sto t0,0[sp]
  ldou t0,192[s1]
  sto t0,8[sp]
  bsr _printf
; printf("tsar->c = %d\n", tsar->c);
  sub sp,sp,16
  lda t0,_main00163.00007[gp]
  sto t0,0[sp]
  ldou t0,384[s1]
  sto t0,8[sp]
  bsr _printf
; b = &(bolshevic.b);
  lda t1,00163.00116[gp]
  add s0,t1,8
; printf("bolshevic.b = %d\n", *b);
  sub sp,sp,16
  lda t0,_main00163.00008[gp]
  sto t0,0[sp]
  ldo t0,[s0]
  sto t0,8[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00018:
  ldo s0,[sp]
  ldo s1,8[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00163,@function
	.size	_main00163,$-_main00163


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00163.00001,@object
	.size	_main00163.00001,16
_main00163.00001: ; a = %d

	.2byte	97,32,61,32,37,100,10,0

	.type	_main00163.00002,@object
	.size	_main00163.00002,36
_main00163.00002: ; bolshevic.a = %d

	.2byte	98,111,108,115,104,101,118,105
	.2byte	99,46,97,32,61,32,37,100
	.2byte	10,0

	.type	_main00163.00003,@object
	.size	_main00163.00003,36
_main00163.00003: ; bolshevic.b = %d

	.2byte	98,111,108,115,104,101,118,105
	.2byte	99,46,98,32,61,32,37,100
	.2byte	10,0

	.type	_main00163.00004,@object
	.size	_main00163.00004,36
_main00163.00004: ; bolshevic.c = %d

	.2byte	98,111,108,115,104,101,118,105
	.2byte	99,46,99,32,61,32,37,100
	.2byte	10,0

	.type	_main00163.00005,@object
	.size	_main00163.00005,28
_main00163.00005: ; tsar->a = %d

	.2byte	116,115,97,114,45,62,97,32
	.2byte	61,32,37,100,10,0

	.type	_main00163.00006,@object
	.size	_main00163.00006,28
_main00163.00006: ; tsar->b = %d

	.2byte	116,115,97,114,45,62,98,32
	.2byte	61,32,37,100,10,0

	.type	_main00163.00007,@object
	.size	_main00163.00007,28
_main00163.00007: ; tsar->c = %d

	.2byte	116,115,97,114,45,62,99,32
	.2byte	61,32,37,100,10,0

	.type	_main00163.00008,@object
	.size	_main00163.00008,36
_main00163.00008: ; bolshevic.b = %d

	.2byte	98,111,108,115,104,101,118,105
	.2byte	99,46,98,32,61,32,37,100
	.2byte	10,0
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
	.global	_main00163
	.global	_feof
	.extern	_start_data
	.global	_bolshevic
	.extern	_start_bss
	.extern	_start_rodata
