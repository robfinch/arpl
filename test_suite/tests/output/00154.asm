                                                                                                                                                    	.bss
	.align 4
__noname_var0:
	.type	__noname_var0,@object
	.size	__noname_var0,16

  
	.text
 
;{++ _main00154

	.sdreg	29
_main00154:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,48
  sto s0,[sp]
  sto s1,8[sp]
  lda s0,-48[fp]
  lda s1,-16[fp]
; bloggs.boris = 12;
  ldi t0,12
  sto t0,0[s1]
; bloggs.natasha = 34;
  ldi t0,34
  sto t0,8[s1]
; printf("%d\n", bloggs.boris);
  sub sp,sp,16
  lda t0,_main00154.00001[gp]
  sto t0,0[sp]
  ldo t0,0[s1]
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", bloggs.natasha);
  sub sp,sp,16
  lda t0,_main00154.00002[gp]
  sto t0,0[sp]
  ldo t0,8[s1]
  sto t0,8[sp]
  bsr _printf
; jones[0].boris = 12;
  ldi t0,12
  sto t0,0[s0]
; jones[0].natasha = 34;
  ldi t0,34
  sto t0,128[s0]
; jones[1].boris = 56;
  ldi t0,56
  sto t0,0[s0]
; jones[1].natasha = 78;
  ldi t0,78
  sto t0,128[s0]
; printf("%d\n", jones[0].boris);
  sub sp,sp,16
  lda t0,_main00154.00003[gp]
  sto t0,0[sp]
  ldo t0,0[s0]
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", jones[0].natasha);
  sub sp,sp,16
  lda t0,_main00154.00004[gp]
  sto t0,0[sp]
  ldo t0,128[s0]
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", jones[1].boris);
  sub sp,sp,16
  lda t0,_main00154.00005[gp]
  sto t0,0[sp]
  ldo t0,0[s0]
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", jones[1].natasha);
  sub sp,sp,16
  lda t0,_main00154.00006[gp]
  sto t0,0[sp]
  ldo t0,128[s0]
  sto t0,8[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00016:
  ldo s0,[sp]
  ldo s1,8[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00154,@function
	.size	_main00154,$-_main00154


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00154.00001,@object
	.size	_main00154.00001,8
_main00154.00001: ; %d

	.2byte	37,100,10,0

	.type	_main00154.00002,@object
	.size	_main00154.00002,8
_main00154.00002: ; %d

	.2byte	37,100,10,0

	.type	_main00154.00003,@object
	.size	_main00154.00003,8
_main00154.00003: ; %d

	.2byte	37,100,10,0

	.type	_main00154.00004,@object
	.size	_main00154.00004,8
_main00154.00004: ; %d

	.2byte	37,100,10,0

	.type	_main00154.00005,@object
	.size	_main00154.00005,8
_main00154.00005: ; %d

	.2byte	37,100,10,0

	.type	_main00154.00006,@object
	.size	_main00154.00006,8
_main00154.00006: ; %d

	.2byte	37,100,10,0
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
	.global	_main00154
	.global	_feof
	.extern	_start_data
	.global	__noname_var0
	.extern	_start_bss
	.extern	_start_rodata
