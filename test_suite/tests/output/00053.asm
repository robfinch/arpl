   
	.text
  
;{++ _main00053

	.sdreg	29
_main00053:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,48
  sto s0,[sp]
  sto s1,8[sp]
  lda s0,-16[fp]
  lda s1,-8[fp]
; s1.x = 1;
  ldi t0,1
  sto t0,0[s1]
; s2.y = 1;
  sto t0,8[s0]
; if (s1.x - s2.y != 0)
  ldo t2,0[s1]
  ldo t3,8[s0]
  sub t1,t2,t3
  beq t1,r0,.00013
; return 1;
  ldi a0,1
.00012:
  ldo s0,[sp]
  ldo s1,8[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00013:
; return 0;
  mov a0,r0
  bra .00012
	.type	_main00053,@function
	.size	_main00053,$-_main00053


;--}

	.global	_main00053
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
