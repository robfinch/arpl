
;{++ _main

	.text

	.sdreg	29
_main:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,96
  sto s0,[sp]
  sto s1,8[sp]
  sto s2,16[sp]
  sto s3,24[sp]
  sto s4,32[sp]
  sto s5,40[sp]
  sto s6,48[sp]
  sto s7,56[sp]
  lda s0,00311.00001[gp]
; integer a : cache::none = 10;
  ldi s1,10
  ldi s2,1
  ldi s4,2
  ldi s5,3
  ldi s3,4
  ldi t0,5
  sto t0,-48[fp]
  ldi s6,6
  ldi s7,7
  ldi t0,8
  sto t0,-72[fp]
; *mem = a;
  ldo t0,[s0]
  sto s1,[t0]
; *mem = b;
  ldo t0,[s0]
  sto s2,[t0]
; *mem = c;
  ldo t0,[s0]
  sto s4,[t0]
; *mem = d;
  ldo t0,[s0]
  sto s5,[t0]
; *mem = e;
  ldo t0,[s0]
  sto s3,[t0]
; *mem = f;
  ldo t0,[s0]
  ldo t1,-48[fp]
  sto t1,[t0]
; *mem = g;
  ldo t0,[s0]
  sto s6,[t0]
; *mem = h;
  ldo t0,[s0]
  sto s7,[t0]
; *mem = i;
  ldo t0,[s0]
  ldo t1,-72[fp]
  sto t1,[t0]
; a = *mem;
  ldo t0,[s0]
  ldo t1,[t0]
  ldo s1,[t0]
; b = *mem;
  ldo t0,[s0]
  ldo t1,[t0]
  ldo s2,[t0]
; c = *mem;
  ldo t0,[s0]
  ldo t1,[t0]
  ldo s4,[t0]
; d = *mem;
  ldo t0,[s0]
  ldo t1,[t0]
  ldo s5,[t0]
; e = *mem;
  ldo t0,[s0]
  ldo t1,[t0]
  ldo s3,[t0]
; f = *mem;
  ldo t0,[s0]
  sto t0,-48[fp]
; g = *mem;
  ldo t0,[s0]
  ldo t1,[t0]
  ldo s6,[t0]
; h = *mem;
  ldo t0,[s0]
  ldo t1,[t0]
  ldo s7,[t0]
; i = *mem;
  ldo t0,[s0]
  sto t0,-72[fp]
; return ( a ?? b : c : d : e );
  bnez s1,.00018
  mov t0,s2
  bra .00017
.00018:
  ldi t1,1
  bne s1,t1,.00019
  mov t0,s4
  bra .00017
.00019:
  ldi t1,2
  bne s1,t1,.00020
  mov t0,s5
  bra .00017
.00020:
  mov t0,s3
.00017:
  mov a0,t0
.00016:
  ldo s0,[sp]
  ldo s1,8[sp]
  ldo s2,16[sp]
  ldo s3,24[sp]
  ldo s4,32[sp]
  ldo s5,40[sp]
  ldo s6,48[sp]
  ldo s7,56[sp]
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
