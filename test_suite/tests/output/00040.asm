                                                                                                                         	.bss
	.align 4
_N:
	.type	_N,@object
	.size	_N,8

 	.bss
	.align 4
_t:
	.type	_t,@object
	.size	_t,8

     
	.text

;{++ _chk

	.sdreg	29
_chk:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
  ldo t2,32[fp]
  ldo t3,40[fp]
; for (r=i=0; i<8; i++) {
  mov t0,r0
  ldi t1,8
  bge t0,t1,.00039
; r = r + t[x + 8*i];
  lda t1,_t[gp]
  asl t3,t0,3
  add t2,t2,t3
  ldo t0,0[t1+t2*]
  add t0,t1,t0
; r = r + t[i + 8*y];
  lda t1,_t[gp]
  asl t3,t3,3
  add t2,t0,t3
  ldo t0,0[t1+t2*]
  add t0,t1,t0
; if (x+i < 8 & y+i < 8)
  add t2,t2,t0
  zslt t1,t2,8,1
  sto t1,-32[fp]
  add t2,t3,t0
  zslt t1,t2,8,1
  and t0,t1,t1
  beqz t0,.00041
; r = r + t[x+i + 8*(y+i)];
  lda t1,_t[gp]
  add t3,t2,t0
  sto t3,-32[fp]
  add t3,t3,t0
  asl t2,t3,3
  add t2,t3,t2
  ldo t1,0[t1+t2*]
  add t0,t1,t1
.00041:
; if (x+i < 8 & y-i >= 0)
  add t2,t2,t0
  zslt t1,t2,8,1
  sto t1,-32[fp]
  sub t2,t3,t0
  zsge t1,t2,0,1
  and t0,t1,t1
  beqz t0,.00047
; r = r + t[x+i + 8*(y-i)];
  lda t1,_t[gp]
  add t3,t2,t0
  sto t3,-32[fp]
  sub t3,t3,t0
  asl t2,t3,3
  add t2,t3,t2
  ldo t1,0[t1+t2*]
  add t0,t1,t1
.00047:
; if (x-i >= 0 & y+i < 8)
  sub t2,t2,t0
  zsge t1,t2,0,1
  sto t1,-32[fp]
  add t2,t3,t0
  zslt t1,t2,8,1
  and t0,t1,t1
  beqz t0,.00053
; r = r + t[x-i + 8*(y+i)];
  lda t1,_t[gp]
  sub t3,t2,t0
  sto t3,-32[fp]
  add t3,t3,t0
  asl t2,t3,3
  add t2,t3,t2
  ldo t1,0[t1+t2*]
  add t0,t1,t1
.00053:
; if (x-i >= 0 & y-i >= 0)
  sub t2,t2,t0
  zsge t1,t2,0,1
  sto t1,-32[fp]
  sub t2,t3,t0
  zsge t1,t2,0,1
  and t0,t1,t1
  beqz t0,.00059
; r = r + t[x-i + 8*(y-i)];
  sub t3,t2,t0
  sto t3,-32[fp]
.00059:
  ldi t1,8
.00039:
; return r;
  mov a0,t1
.00037:
  mov sp,fp
  ldo fp,[sp]
  rtd 48,0
	.type	_chk,@function
	.size	_chk,$-_chk


;--}
    
;{++ _go

	.sdreg	29
_go:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  ldo t0,40[fp]
  ldo t2,32[fp]
; if (n == 8) {
  ldi t1,8
  bne t2,t1,.00085
; N++;
  lda t0,_N[gp]
; return 0;
  mov a0,r0
.00084:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 56,0
.00085:
; for (; y<8; y++) {
  bge t1,t1,.00088
.00087:
; for (; x<8; x++)
  bge t0,t1,.00091
; if (chk(x, y) == 0) {
  sto t0,-32[fp]
  sub sp,sp,16
  sto t0,0[sp]
  sto t1,8[sp]
  bsr _chk
  ldo t0,-32[fp]
  bnez a0,.00093
; t[x + 8*y]++;
  lda t0,_t[gp]
  asl t2,t1,3
  add t1,t0,t2
; go(n+1, x, y);
  sub sp,sp,24
  add t0,t2,1
  sto t0,0[sp]
  sto t0,8[sp]
  sto t1,16[sp]
  bsr _go
.00093:
  ldi t1,8
.00091:
; x = 0;
  blt t1,t1,.00087
.00088:
; return 0;
  mov a0,r0
  bra .00084
	.type	_go,@function
	.size	_go,$-_go


;--}
 
;{++ _main00040

	.sdreg	29
_main00040:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; t = calloc(64, sizeof(int));
  lda t0,_t[gp]
  sto t0,-32[fp]
  sub sp,sp,16
  ldi t1,64
  sto t1,0[sp]
  ldi t1,8
  sto t1,8[sp]
  bsr _calloc
  ldo t0,-32[fp]
  sto a0,[t0]
; go(0, 0, 0);
  sub sp,sp,24
  sto r0,0[sp]
  sto r0,8[sp]
  sto r0,16[sp]
  bsr _go
; if(N != 92)
  lda t1,_N[gp]
  ldi t2,92
  beq t1,t2,.00107
; return 1;
  ldi a0,1
.00106:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00107:
; return 0;
  mov a0,r0
  bra .00106
	.type	_main00040,@function
	.size	_main00040,$-_main00040


;--}

	.extern	__Mbxlen
	.extern	__Wcxtomb
	.extern	__Mbxtowc
	.extern	__Randseed
	.extern	__Mbcurmax
	.global	_free
	.global	_labs
	.global	_rand
	.global	_system
	.global	_strtol
	.global	_atof
	.global	_atoi
	.global	_ldiv
	.global	_atol
	.global	_exit
	.global	__Mbtowc
	.global	__Wctomb
	.global	_go
	.global	_bsearch
	.global	_realloc
	.global	__Stod
	.global	_mblen
	.global	_abort
	.global	_srand
	.global	_strtoul
	.global	_abs
	.global	_chk
	.global	_qsort
	.global	_div
	.global	_calloc
	.global	_mbstowcs
	.global	_wcstombs
	.global	__Stoul
	.global	_malloc
	.global	_getenv
	.global	_mbtowc
	.global	_wctomb
	.global	_atexit
	.global	_setjmp
	.global	_main00040
	.global	_strtod
	.extern	_start_data
	.extern	_start_bss
	.global	_N
	.global	_t
	.extern	_start_rodata
