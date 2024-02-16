                                                                                                                                                    
	.data
	.align	6

	.align	6
  	.data
	.align 4
_cases:
	.type	_cases,@object
	.size	_cases,72
































































































































  
	.text

;{++ _main00205

	.sdreg	29
_main00205:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; for(j=0; j < sizeof(cases)/sizeof(cases[0]); j++) {
  mov t0,r0
  ldi t1,1
  bge t0,t1,.00025
.00024:
; for(i=0; i < sizeof(cases->c)/sizeof(cases->c[0]); i++)
  ldi t1,4
  bge t1,t1,.00028
.00027:
; printf("cases[%d].c[%d]=%ld\n", j, i, cases[j].c[i]);
  sub sp,sp,32
  lda t0,_main00205.00003[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  sto t1,16[sp]
  lda t1,_cases[gp]
  add t0,t1,r0
  ldo t0,0[t0+t1*]
  sto t0,24[sp]
  bsr _printf
  add sp,sp,16
.00029:
  ldi t1,4
  blt t1,t1,.00027
.00028:
; printf("cases[%d].b=%ld\n", j, cases[j].b);
  sub sp,sp,24
  lda t0,_main00205.00004[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  lda t0,_cases[gp]
  ldo t0,32[t0]
  sto t0,16[sp]
  bsr _printf
  add sp,sp,8
; printf("cases[%d].e=%ld\n", j, cases[j].e);
  sub sp,sp,24
  lda t0,_main00205.00005[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  lda t0,_cases[gp]
  ldo t0,40[t0]
  sto t0,16[sp]
  bsr _printf
  add sp,sp,8
; printf("cases[%d].k=%ld\n", j, cases[j].k);
  sub sp,sp,24
  lda t0,_main00205.00006[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  lda t0,_cases[gp]
  ldo t0,48[t0]
  sto t0,16[sp]
  bsr _printf
  add sp,sp,8
; printf("\n");
  sub sp,sp,8
  lda t0,_main00205.00007[gp]
  sto t0,0[sp]
  bsr _printf
.00026:
.00025:
; return 0;
  mov a0,r0
.00023:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00205,@function
	.size	_main00205,$-_main00205


;--}

	.rodata
	.align	6

	.align	8





	.align	6
	.type	_main00205.00003,@object
	.size	_main00205.00003,42
_main00205.00003: ; cases[%d].c[%d]=%ld

	.2byte	99,97,115,101,115,91,37,100
	.2byte	93,46,99,91,37,100,93,61
	.2byte	37,108,100,10,0

	.type	_main00205.00004,@object
	.size	_main00205.00004,34
_main00205.00004: ; cases[%d].b=%ld

	.2byte	99,97,115,101,115,91,37,100
	.2byte	93,46,98,61,37,108,100,10
	.2byte	0

	.type	_main00205.00005,@object
	.size	_main00205.00005,34
_main00205.00005: ; cases[%d].e=%ld

	.2byte	99,97,115,101,115,91,37,100
	.2byte	93,46,101,61,37,108,100,10
	.2byte	0

	.type	_main00205.00006,@object
	.size	_main00205.00006,34
_main00205.00006: ; cases[%d].k=%ld

	.2byte	99,97,115,101,115,91,37,100
	.2byte	93,46,107,61,37,108,100,10
	.2byte	0

	.type	_main00205.00007,@object
	.size	_main00205.00007,4
_main00205.00007:

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
	.global	_main00205
	.global	_feof
	.extern	_start_data
	.extern	_start_bss
	.global	_cases
	.extern	_start_rodata
