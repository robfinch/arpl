 	.bss
	.align 4
_g:
	.type	_g,@object
	.size	_g,8

 
	.text

;{++ _effect

	.sdreg	29
_effect:
; return 1;
  ldi a0,1
.00010:
  rts 
	.type	_effect,@function
	.size	_effect,$-_effect


;--}
  
;{++ _main00033

	.sdreg	29
_main00033:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
; g = 0;
  lda t0,_g[gp]
  sto r0,[t0]
; x = 0;
  mov s0,r0
; if(x && effect())
  beqz s0,.00039
  bsr _effect
  beqz a0,.00039
; return 1;
  ldi a0,1
.00038:
  ldo s0,[sp]
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00039:
; if(g)
  lda t0,_g[gp]
  beqz t0,.00041
; return 2;
  ldi a0,2
  bra .00038
.00041:
; x = 1;
  ldi s0,1
; if(x && effect()) {
  beqz s0,.00043
  bsr _effect
  beqz a0,.00043
; if(g != 1)
  lda t1,_g[gp]
  ldi t2,1
  beq t1,t2,.00045
; return 3;
  ldi a0,3
  bra .00038
.00045:
  bra .00044
.00043:
; return 4;
  ldi a0,4
  bra .00038
.00044:
; g = 0;
  lda t0,_g[gp]
  sto r0,[t0]
; x = 1;
  ldi s0,1
; if(x || effect()) {
  bnez s0,.00049
  bsr _effect
  beqz a0,.00047
.00049:
; if(g)
  lda t0,_g[gp]
  beqz t0,.00050
; return 5;
  ldi a0,5
  bra .00038
.00050:
  bra .00048
.00047:
; return 6;
  ldi a0,6
  bra .00038
.00048:
; x = 0;
  mov s0,r0
; if(x || effect()) {
  bnez s0,.00054
  bsr _effect
  beqz a0,.00052
.00054:
; if(g != 1)
  lda t1,_g[gp]
  ldi t2,1
  beq t1,t2,.00055
; return 7;
  ldi a0,7
  bra .00038
.00055:
  bra .00053
.00052:
; return 8;
  ldi a0,8
  bra .00038
.00053:
; return 0;
  mov a0,r0
  bra .00038
	.type	_main00033,@function
	.size	_main00033,$-_main00033


;--}

	.global	_effect
	.global	_main00033
	.extern	_start_data
	.extern	_start_bss
	.global	_g
	.extern	_start_rodata
