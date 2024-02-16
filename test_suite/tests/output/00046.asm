             
	.text

;{++ _main00046

	.sdreg	29
_main00046:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
  lda s0,-32[fp]
; v.a = 1;
  ldi t0,1
  sto t0,0[s0]
; v.b1 = 2;
  ldi t0,2
  sto t0,8[s0]
; v.c = 3;
  ldi t0,3
  sto t0,16[s0]
; v.d = 4;
  ldi t0,4
  sto t0,24[s0]
; if (v.a != 1)
  ldo t1,0[s0]
  ldi t2,1
  beq t1,t2,.00019
; return 1;
  ldi a0,1
.00018:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00019:
; if (v.b1 != 2 && v.b2 != 2)
  ldo t1,8[s0]
  ldi t2,2
  beq t1,t2,.00021
  ldo t2,8[s0]
  ldi t3,2
  beq t2,t3,.00021
; return 2;
  ldi a0,2
  bra .00018
.00021:
; if (v.c != 3)
  ldo t1,16[s0]
  ldi t2,3
  beq t1,t2,.00023
; return 3;
  ldi a0,3
  bra .00018
.00023:
; if (v.d != 4)
  ldo t1,24[s0]
  ldi t2,4
  beq t1,t2,.00025
; return 4;
  ldi a0,4
  bra .00018
.00025:
; return 0;
  mov a0,r0
  bra .00018
	.type	_main00046,@function
	.size	_main00046,$-_main00046


;--}

	.global	_main00046
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
