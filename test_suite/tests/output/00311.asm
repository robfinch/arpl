
;{++ _main

	.text

	.sdreg	29
_main:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
  lda t0,00311.00001[gp]
; integer a : cache::none = 10;
  ldi t1,10
  ldi t2,1
  ldi t4,2
  ldi t5,3
  ldi t3,4
  ldi s0,5
  ldi t6,6
  ldi t7,7
  ldi t8,8
; *mem = a;
  ldo t0,[t0]
  sto t1,[t0]
; *mem = b;
  ldo t0,[t0]
  sto t2,[t0]
; *mem = c;
  ldo t0,[t0]
  sto t4,[t0]
; *mem = d;
  ldo t0,[t0]
  sto t5,[t0]
; *mem = e;
  ldo t0,[t0]
  sto t3,[t0]
; *mem = f;
  ldo t0,[t0]
  sto s0,[t0]
; *mem = g;
  ldo t0,[t0]
  sto t6,[t0]
; *mem = h;
  ldo t0,[t0]
  sto t7,[t0]
; *mem = i;
  ldo t0,[t0]
  sto t8,[t0]
; a = *mem;
  ldo t0,[t0]
  ldo t1,[t0]
; b = *mem;
  ldo t0,[t0]
  ldo t1,[t0]
  ldo t2,[t0]
; c = *mem;
  ldo t0,[t0]
  ldo t1,[t0]
  ldo t4,[t0]
; d = *mem;
  ldo t0,[t0]
  ldo t1,[t0]
  ldo t5,[t0]
; e = *mem;
  ldo t0,[t0]
  ldo t1,[t0]
  ldo t3,[t0]
; f = *mem;
  ldo t0,[t0]
  ldo t1,[t0]
  ldo s0,[t0]
; g = *mem;
  ldo t0,[t0]
  ldo t1,[t0]
; h = *mem;
  ldo t0,[t0]
  ldo t1,[t0]
; i = *mem;
  ldo t0,[t0]
  ldo t1,[t0]
; return ( a ?? b : c : d : e );
  bnez t1,.00018
  mov t0,t2
  bra .00017
.00018:
  ldi t1,1
  bne t1,t1,.00019
  mov t0,t4
  bra .00017
.00019:
  ldi t1,2
  bne t1,t1,.00020
  mov t0,t5
  bra .00017
.00020:
  mov t0,t3
.00017:
  mov a0,t0
.00016:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main,@function
	.size	_main,$-_main


;--}


	.align	8


	.bss
	.align	6
	.type	00311.00001,@object
	.size	00311.00001,8
00311.00001: ; _mem
	.byte	0,0,0,0,0,0,0,0


	.global	_main
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
