                                                                                                                                                   
	.text
   
;{++ _main00187

	.sdreg	29
_main00187:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  lda t3,-22[fp]
; FILE *f = fopen("fred.txt", "w");
  sub sp,sp,16
  lda t0,_main00187.00001[gp]
  sto t0,0[sp]
  lda t0,_main00187.00002[gp]
  sto t0,8[sp]
  bsr _fopen
  sto a0,-8[fp]
; fwrite("hello\nhello\n", 1, 12, f);
  sub sp,sp,32
  lda t0,_main00187.00003[gp]
  sto t0,0[sp]
  ldi t0,1
  sto t0,8[sp]
  ldi t0,12
  sto t0,16[sp]
  sto t0,24[sp]
  bsr _fwrite
; fclose(f);
  sub sp,sp,8
  sto t0,0[sp]
  bsr _fclose
; f = fopen("fred.txt", "r");
  sub sp,sp,16
  lda t0,_main00187.00004[gp]
  sto t0,0[sp]
  lda t0,_main00187.00005[gp]
  sto t0,8[sp]
  bsr _fopen
  mov t0,a0
; if (fread(freddy, 1, 6, f) != 6)
  sto t0,-32[fp]
  sub sp,sp,32
  sto t3,0[sp]
  ldi t1,1
  sto t1,8[sp]
  ldi t1,6
  sto t1,16[sp]
  sto t0,24[sp]
  bsr _fread
  ldo t0,-32[fp]
  beq a0,t1,.00043
; printf("couldn't read fred.txt\n");
  sub sp,sp,8
  lda t0,_main00187.00006[gp]
  sto t0,0[sp]
  bsr _printf
.00043:
; freddy[6] = '\0';
  stw r0,12[t3]
; fclose(f);
  sub sp,sp,8
  sto t0,0[sp]
  bsr _fclose
; printf("%s", freddy);
  sub sp,sp,16
  lda t0,_main00187.00007[gp]
  sto t0,0[sp]
  sto t3,8[sp]
  bsr _printf
; f = fopen("fred.txt", "r");
  sub sp,sp,16
  lda t0,_main00187.00008[gp]
  sto t0,0[sp]
  lda t0,_main00187.00009[gp]
  sto t0,8[sp]
  bsr _fopen
  mov t0,a0
; while ( (InChar = fgetc(f)) != (-1))
  sto t0,-32[fp]
  sub sp,sp,8
  sto t0,0[sp]
  bsr _fgetc
  ldo t0,-32[fp]
  ldi t1,16777215
  beq t1,t1,.00046
.00045:
; ShowChar = InChar;
  mov t2,t1
; if (ShowChar < ' ')
  ldi t1,32
  bge t2,t1,.00047
; ShowChar = '.';
  ldi t2,46
.00047:
; printf("ch: %d '%c'\n", InChar, ShowChar);
  sub sp,sp,24
  lda t0,_main00187.00010[gp]
  sto t0,0[sp]
  sto t1,8[sp]
  sto t2,16[sp]
  bsr _printf
  add sp,sp,8
  sto t0,-32[fp]
  sub sp,sp,8
  sto t0,0[sp]
  bsr _fgetc
  ldo t0,-32[fp]
  ldi t1,16777215
  bne t1,t1,.00045
.00046:
; fclose(f);
  sub sp,sp,8
  sto t0,0[sp]
  bsr _fclose
; f = fopen("fred.txt", "r");
  sub sp,sp,16
  lda t0,_main00187.00011[gp]
  sto t0,0[sp]
  lda t0,_main00187.00012[gp]
  sto t0,8[sp]
  bsr _fopen
  mov t0,a0
; while ( (InChar = ((f)->_Next < (f)->_Rend ? *(f)->_Next++ : (getc)(f))) != (-1))
  ldou t2,2[fp]
  ldou t2,24[t2]
  ldou t3,2[fp]
  ldou t3,32[t3]
  bgeu t2,t3,.00051
  ldou t3,2[fp]
  ldou t3,24[t3]
  ldou t2,24[t3]
  bra .00052
.00051:
  sto t0,-32[fp]
  sto t1,-24[fp]
  sto t2,-16[fp]
  sub sp,sp,8
  ldo t3,2[fp]
  sto t3,0[sp]
  bsr _getc
  ldo t0,-32[fp]
  mov t2,a0
.00052:
  movzxw t2,t2
  movzxw t1,t2
  ldi t2,16777215
  beq t1,t2,.00050
.00049:
; ShowChar = InChar;
  mov t2,t1
; if (ShowChar < ' ')
  ldi t1,32
  bge t2,t1,.00053
; ShowChar = '.';
  ldi t2,46
.00053:
; printf("ch: %d '%c'\n", InChar, ShowChar);
  sub sp,sp,24
  lda t0,_main00187.00013[gp]
  sto t0,0[sp]
  sto t1,8[sp]
  sto t2,16[sp]
  bsr _printf
  add sp,sp,8
  ldou t2,2[fp]
  ldou t2,24[t2]
  ldou t3,2[fp]
  ldou t3,32[t3]
  bgeu t2,t3,.00055
  ldou t3,2[fp]
  ldou t3,24[t3]
  ldou t2,24[t3]
  bra .00056
.00055:
  sto t0,-32[fp]
  sto t1,-24[fp]
  sto t2,-16[fp]
  sub sp,sp,8
  ldo t3,2[fp]
  sto t3,0[sp]
  bsr _getc
  ldo t0,-32[fp]
  mov t2,a0
.00056:
  movzxw t2,t2
  movzxw t1,t2
  ldi t2,16777215
  bne t1,t2,.00049
.00050:
; fclose(f);
  sub sp,sp,8
  sto t0,0[sp]
  bsr _fclose
; f = fopen("fred.txt", "r");
  sub sp,sp,16
  lda t0,_main00187.00014[gp]
  sto t0,0[sp]
  lda t0,_main00187.00015[gp]
  sto t0,8[sp]
  bsr _fopen
  mov t0,a0
; while (fgets(freddy, sizeof(freddy), f) != (void *)0)
  sto t0,-32[fp]
  sub sp,sp,24
  sto t3,0[sp]
  ldi t1,14
  sto t1,8[sp]
  sto t0,16[sp]
  bsr _fgets
  ldo t0,-32[fp]
  beq a0,r0,.00058
.00057:
; printf("x: %s", freddy);
  sub sp,sp,16
  lda t0,_main00187.00016[gp]
  sto t0,0[sp]
  sto t3,8[sp]
  bsr _printf
  sto t0,-32[fp]
  sub sp,sp,24
  sto t3,0[sp]
  sto t1,8[sp]
  sto t0,16[sp]
  bsr _fgets
  ldo t0,-32[fp]
  bnez a0,.00057
.00058:
; fclose(f);
  sub sp,sp,8
  sto t0,0[sp]
  bsr _fclose
; return 0;
  mov a0,r0
.00042:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00187,@function
	.size	_main00187,$-_main00187


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00187.00001,@object
	.size	_main00187.00001,18
_main00187.00001: ; fred.txt

	.2byte	102,114,101,100,46,116,120,116
	.2byte	0

	.type	_main00187.00002,@object
	.size	_main00187.00002,4
_main00187.00002: ; w

	.2byte	119,0

	.type	_main00187.00003,@object
	.size	_main00187.00003,26
_main00187.00003: ; hellohello

	.2byte	104,101,108,108,111,10,104,101
	.2byte	108,108,111,10,0

	.type	_main00187.00004,@object
	.size	_main00187.00004,18
_main00187.00004: ; fred.txt

	.2byte	102,114,101,100,46,116,120,116
	.2byte	0

	.type	_main00187.00005,@object
	.size	_main00187.00005,4
_main00187.00005: ; r

	.2byte	114,0

	.type	_main00187.00006,@object
	.size	_main00187.00006,48
_main00187.00006: ; couldn't read fred.txt

	.2byte	99,111,117,108,100,110,39,116
	.2byte	32,114,101,97,100,32,102,114
	.2byte	101,100,46,116,120,116,10,0

	.type	_main00187.00007,@object
	.size	_main00187.00007,6
_main00187.00007: ; %s

	.2byte	37,115,0

	.type	_main00187.00008,@object
	.size	_main00187.00008,18
_main00187.00008: ; fred.txt

	.2byte	102,114,101,100,46,116,120,116
	.2byte	0

	.type	_main00187.00009,@object
	.size	_main00187.00009,4
_main00187.00009: ; r

	.2byte	114,0

	.type	_main00187.00010,@object
	.size	_main00187.00010,26
_main00187.00010: ; ch: %d '%c'

	.2byte	99,104,58,32,37,100,32,39
	.2byte	37,99,39,10,0

	.type	_main00187.00011,@object
	.size	_main00187.00011,18
_main00187.00011: ; fred.txt

	.2byte	102,114,101,100,46,116,120,116
	.2byte	0

	.type	_main00187.00012,@object
	.size	_main00187.00012,4
_main00187.00012: ; r

	.2byte	114,0

	.type	_main00187.00013,@object
	.size	_main00187.00013,26
_main00187.00013: ; ch: %d '%c'

	.2byte	99,104,58,32,37,100,32,39
	.2byte	37,99,39,10,0

	.type	_main00187.00014,@object
	.size	_main00187.00014,18
_main00187.00014: ; fred.txt

	.2byte	102,114,101,100,46,116,120,116
	.2byte	0

	.type	_main00187.00015,@object
	.size	_main00187.00015,4
_main00187.00015: ; r

	.2byte	114,0

	.type	_main00187.00016,@object
	.size	_main00187.00016,12
_main00187.00016: ; x: %s

	.2byte	120,58,32,37,115,0
	.extern	__Files
	.global	_getc
	.global	_main00187
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
