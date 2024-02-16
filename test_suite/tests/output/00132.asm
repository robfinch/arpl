                                                                                                                                                  
	.text
 
;{++ _main00132

	.sdreg	29
_main00132:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; printf("Hello world\n");
  sub sp,sp,8
  lda t0,_main00132.00001[gp]
  sto t0,0[sp]
  bsr _printf
; for (Count = -5; Count <= 5; Count++)
  ldi t0,16777211
  ldi t1,5
  bgt t0,t1,.00022
; printf("Count = %d\n", Count);
  sub sp,sp,16
  lda t0,_main00132.00002[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
.00023:
.00022:
; printf("String 'hello', 'there' is '%s', '%s'\n", "hello", "there");
  sub sp,sp,24
  lda t0,_main00132.00003[gp]
  sto t0,0[sp]
  lda t0,_main00132.00004[gp]
  sto t0,8[sp]
  lda t0,_main00132.00005[gp]
  sto t0,16[sp]
  bsr _printf
  add sp,sp,8
; printf("Character 'A' is '%c'\n", 65);
  sub sp,sp,16
  lda t0,_main00132.00006[gp]
  sto t0,0[sp]
  ldi t0,65
  sto t0,8[sp]
  bsr _printf
; printf("Character 'a' is '%c'\n", 'a');
  sub sp,sp,16
  lda t0,_main00132.00007[gp]
  sto t0,0[sp]
  ldi t0,97
  sto t0,8[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00020:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00132,@function
	.size	_main00132,$-_main00132


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00132.00001,@object
	.size	_main00132.00001,26
_main00132.00001: ; Hello world

	.2byte	72,101,108,108,111,32,119,111
	.2byte	114,108,100,10,0

	.type	_main00132.00002,@object
	.size	_main00132.00002,24
_main00132.00002: ; Count = %d

	.2byte	67,111,117,110,116,32,61,32
	.2byte	37,100,10,0

	.type	_main00132.00003,@object
	.size	_main00132.00003,78
_main00132.00003: ; String 'hello', 'there' is '%s', '%s'

	.2byte	83,116,114,105,110,103,32,39
	.2byte	104,101,108,108,111,39,44,32
	.2byte	39,116,104,101,114,101,39,32
	.2byte	105,115,32,39,37,115,39,44
	.2byte	32,39,37,115,39,10,0

	.type	_main00132.00004,@object
	.size	_main00132.00004,12
_main00132.00004: ; hello

	.2byte	104,101,108,108,111,0

	.type	_main00132.00005,@object
	.size	_main00132.00005,12
_main00132.00005: ; there

	.2byte	116,104,101,114,101,0

	.type	_main00132.00006,@object
	.size	_main00132.00006,46
_main00132.00006: ; Character 'A' is '%c'

	.2byte	67,104,97,114,97,99,116,101
	.2byte	114,32,39,65,39,32,105,115
	.2byte	32,39,37,99,39,10,0

	.type	_main00132.00007,@object
	.size	_main00132.00007,46
_main00132.00007: ; Character 'a' is '%c'

	.2byte	67,104,97,114,97,99,116,101
	.2byte	114,32,39,97,39,32,105,115
	.2byte	32,39,37,99,39,10,0
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
	.global	_main00132
	.global	_feof
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
