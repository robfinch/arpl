             
	.text

;{++ _main00046

	.sdreg	29
_main00046:
; v.a = 1;
  ldi t0,1
  sto t0,0[t0]
; v.b1 = 2;
  ldi t0,2
  sto t0,8[t0]
; v.c = 3;
  ldi t0,3
  sto t0,16[t0]
; v.d = 4;
  ldi t0,4
  sto t0,24[t0]
; if (v.a != 1)
  ldo t0,0[t0]
  ldi t1,1
  beq t0,t1,.00019
; return 1;
  ldi a0,1
.00018:
  rts 
.00019:
; if (v.b1 != 2 && v.b2 != 2)
  ldo t0,8[t0]
  ldi t1,2
  beq t0,t1,.00021
  ldo t0,8[t0]
  beq t0,t1,.00021
; return 2;
  ldi a0,2
  bra .00018
.00021:
; if (v.c != 3)
  ldo t0,16[t0]
  ldi t1,3
  beq t0,t1,.00023
; return 3;
  ldi a0,3
  bra .00018
.00023:
; if (v.d != 4)
  ldo t0,24[t0]
  ldi t1,4
  beq t0,t1,.00025
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
