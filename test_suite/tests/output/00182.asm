
	.text

;{++ _print_led

	.sdreg	29
_print_led:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  ldo t2,32[fp]
  lda t0,00182.00221[gp]
  lda t4,00182.00221[gp]
; int i=0,n;
  mov t1,r0
; n = ( x == 0L ? 1 : 0 );  /* 0 is a digit, hence a special case */
  bnez t2,.00247
  ldi t2,1
  ldi t1,1
  bra .00248
.00247:
  ldi t2,0
  ldi t1,0
.00248:
  mov t3,t1
; while(x){
  beqz t2,.00250
.00249:
; d[n++] = (int)(x%10L);
  add t3,t3,1
  div t1,t2,10
  mul t1,t1,10
  sub t0,t2,t1
  sto t0,0[t4+t3*]
; if(n >= 32)break;
  ldi t1,32
  blt t3,t1,.00251
  bra .00250
.00251:
; x = x/10L;
  div t0,t2,10
; x = x/10L;
  div t2,t2,10
  bnez t2,.00249
.00250:
; for(i=n-1;i>=0;i--){
  sub t0,t3,1
; for(i=n-1;i>=0;i--){
  sub t1,t3,1
  blt t1,r0,.00254
.00253:
; topline(d[i],buf);
  sub sp,sp,16
  ldo t0,0[t4+t1*]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _topline
; *buf++=' ';
  ldi t0,32
  stw t0,[t0]
  add t0,t0,2
  sub t1,t1,1
  bge t1,r0,.00253
.00254:
; *buf++='\n'; /* move teletype to next line */
  ldi t0,10
  stw t0,[t0]
; for(i=n-1;i>=0;i--){
  sub t0,t3,1
; for(i=n-1;i>=0;i--){
  sub t1,t3,1
  blt t1,r0,.00257
.00256:
; midline(d[i],buf);
  sub sp,sp,16
  ldo t0,0[t4+t1*]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _midline
; *buf++=' ';
  ldi t0,32
  stw t0,[t0]
  add t0,t0,2
  sub t1,t1,1
  bge t1,r0,.00256
.00257:
; *buf++='\n';
  ldi t0,10
  stw t0,[t0]
; for(i=n-1;i>=0;i--){
  sub t0,t3,1
; for(i=n-1;i>=0;i--){
  sub t1,t3,1
  blt t1,r0,.00260
.00259:
; botline(d[i],buf);
  sub sp,sp,16
  ldo t0,0[t4+t1*]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _botline
; *buf++=' ';
  ldi t0,32
  stw t0,[t0]
  add t0,t0,2
  sub t1,t1,1
  bge t1,r0,.00259
.00260:
; *buf++='\n';
  ldi t0,10
  stw t0,[t0]
  add t0,t0,2
; *buf='\0';
  stw r0,[t0]
.00246:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 48,0
	.type	_print_led,@function
	.size	_print_led,$-_print_led


;--}
  
;{++ _main00182

	.sdreg	29
_main00182:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; print_led(1234567, buf);
  sub sp,sp,16
  ldi t0,1234567
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _print_led
; printf("%s\n",buf);
  sub sp,sp,16
  lda t0,_main00182.00262[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00272:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00182,@function
	.size	_main00182,$-_main00182


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00182.00262,@object
	.size	_main00182.00262,8
_main00182.00262: ; %s

	.2byte	37,115,10,0

	.align	8


	.bss
	.align	6
	.type	00182.00221,@object
	.size	00182.00221,256
00182.00221: ; _d
	.byte	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte	0,,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte	0,,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte	0,,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte	0,,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte	0,,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte	0,,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte	0,,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte	0,


	.extern	__Mbxlen
	.extern	__Wcxtomb
	.extern	__Mbxtowc
	.extern	__Files
	.extern	__Randseed
	.extern	__Mbcurmax
	.global	_free
	.global	_labs
	.global	_getc
	.global	_rand
	.global	_system
	.global	_strtol
	.global	_atof
	.global	_atoi
	.global	_ldiv
	.global	_atol
	.global	_gets
	.global	_exit
	.global	_putc
	.global	_print_led
	.global	__Mbtowc
	.global	__Wctomb
	.global	_puts
	.global	_bsearch
	.global	_getchar
	.global	_realloc
	.global	_midline
	.global	_botline
	.global	_freopen
	.global	_tmpfile
	.global	_putchar
	.global	_fgetpos
	.global	__Stod
	.global	_fprintf
	.global	_topline
	.global	_setvbuf
	.global	_fread
	.global	_fsetpos
	.global	_sprintf
	.global	_fgetc
	.global	_vprintf
	.global	_scanf
	.global	_mblen
	.global	_fseek
	.global	_ftell
	.global	_abort
	.global	_srand
	.global	_fopen
	.global	_fgets
	.global	_strtoul
	.global	_fputc
	.global	_fputs
	.global	_abs
	.global	_qsort
	.global	_div
	.global	_clearerr
	.global	__Fgpos
	.global	__Fspos
	.global	_calloc
	.global	_vfprintf
	.global	_fscanf
	.global	_mbstowcs
	.global	_wcstombs
	.global	__Stoul
	.global	_malloc
	.global	_rename
	.global	_fclose
	.global	_vsprintf
	.global	_sscanf
	.global	_ungetc
	.global	_fflush
	.global	_getenv
	.global	_rewind
	.global	_setbuf
	.global	_mbtowc
	.global	_wctomb
	.global	_tmpnam
	.global	_remove
	.global	_atexit
	.global	_ferror
	.global	_fwrite
	.global	_setjmp
	.global	_printf
	.global	_perror
	.global	_strtod
	.global	_feof
	.global	_main00182
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
