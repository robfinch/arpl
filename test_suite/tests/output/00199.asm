                                                                                                                                                  
	.text

;{++ _fred

	.sdreg	29
_fred:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; printf("In fred()\n");
  sub sp,sp,8
  lda t0,_fred.00001[gp]
  sto t0,0[sp]
  bsr _printf
; printf("At end\n");
  sub sp,sp,8
  lda t0,_fred.00004[gp]
  sto t0,0[sp]
  bsr _printf
.00014:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_fred,@function
	.size	_fred,$-_fred


;--}
   
;{++ _joe

	.sdreg	29
_joe:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
; int b = 5678;
  ldi t0,5678
  sto t0,-8[fp]
; printf("In joe()\n");
  sub sp,sp,8
  lda t0,_joe.00015[gp]
  sto t0,0[sp]
  bsr _printf
; int c = 1234;
  ldi s0,1234
; printf("c = %d\n", c);
  sub sp,sp,16
  lda t0,_joe.00016[gp]
  sto t0,0[sp]
  sto s0,8[sp]
  bsr _printf
; printf("done\n");
  sub sp,sp,8
  lda t0,_joe.00019[gp]
  sto t0,0[sp]
  bsr _printf
.00029:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_joe,@function
	.size	_joe,$-_joe


;--}
   
;{++ _henry

	.sdreg	29
_henry:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; printf("In henry()\n");
  sub sp,sp,8
  lda t0,_henry.00030[gp]
  sto t0,0[sp]
  bsr _printf
.00031:
; b = 1234;
  ldi t0,1234
  sto t0,-16[fp]
; printf("b = %d\n", b);
  sub sp,sp,16
  lda t0,_henry.00032[gp]
  sto t0,0[sp]
  ldo t0,-16[fp]
  sto t0,8[sp]
  bsr _printf
; printf("done\n");
  sub sp,sp,8
  lda t0,_henry.00033[gp]
  sto t0,0[sp]
  bsr _printf
.00043:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_henry,@function
	.size	_henry,$-_henry


;--}
 
;{++ _main00199

	.sdreg	29
_main00199:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; fred();
  bsr _fred
; joe();
  bsr _joe
; henry();
  bsr _henry
; return 0;
  mov a0,r0
.00053:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00199,@function
	.size	_main00199,$-_main00199


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_fred.00001,@object
	.size	_fred.00001,22
_fred.00001: ; In fred()

	.2byte	73,110,32,102,114,101,100,40
	.2byte	41,10,0

	.type	_fred.00003,@object
	.size	_fred.00003,22
_fred.00003: ; In middle

	.2byte	73,110,32,109,105,100,100,108
	.2byte	101,10,0

	.type	_fred.00004,@object
	.size	_fred.00004,16
_fred.00004: ; At end

	.2byte	65,116,32,101,110,100,10,0

	.type	_joe.00015,@object
	.size	_joe.00015,20
_joe.00015: ; In joe()

	.2byte	73,110,32,106,111,101,40,41
	.2byte	10,0

	.type	_joe.00016,@object
	.size	_joe.00016,16
_joe.00016: ; c = %d

	.2byte	99,32,61,32,37,100,10,0

	.type	_joe.00018,@object
	.size	_joe.00018,14
_joe.00018: ; uh-oh

	.2byte	117,104,45,111,104,10,0

	.type	_joe.00019,@object
	.size	_joe.00019,12
_joe.00019: ; done

	.2byte	100,111,110,101,10,0

	.type	_henry.00030,@object
	.size	_henry.00030,24
_henry.00030: ; In henry()

	.2byte	73,110,32,104,101,110,114,121
	.2byte	40,41,10,0

	.type	_henry.00032,@object
	.size	_henry.00032,16
_henry.00032: ; b = %d

	.2byte	98,32,61,32,37,100,10,0

	.type	_henry.00033,@object
	.size	_henry.00033,12
_henry.00033: ; done

	.2byte	100,111,110,101,10,0
	.extern	__Files
	.global	_fred
	.global	_getc
	.global	_main00199
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
	.global	_henry
	.global	_fputs
	.global	_joe
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
