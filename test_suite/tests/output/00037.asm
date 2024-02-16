   
	.text

;{++ _main00037

	.sdreg	29
_main00037:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,48
  sto s0,[sp]
  sto s1,8[sp]
  ldo t1,[fp]
  lda s1,-16[fp]
; x[1] = 7;
  ldi t0,7
  sto t0,8[s1]
; p = &x[0];
  add s0,s1,r0
; p = p + 1;
  add s0,s0,8
; if(*p != 7)
  ldo t1,[s0]
  ldi t2,7
  beq t1,t2,.00015
; return 1;
  ldi a0,1
.00014:
  ldo s0,[sp]
  ldo s1,8[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00015:
; if(&x[1] - &x[0] != 1)
  add t3,s1,1
  add t4,s1,r0
  sub t2,t3,t4
  asr t1,t2,3
  ldi t2,1
  beq t1,t2,.00017
; return 1;
  ldi a0,1
  bra .00014
.00017:
; return 0;
  mov a0,r0
  bra .00014
	.type	_main00037,@function
	.size	_main00037,$-_main00037


;--}

	.global	_main00037
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
