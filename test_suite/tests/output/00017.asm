     
	.text

;{++ _main00017

	.sdreg	29
_main00017:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
  lda s0,-24[fp]
; aa = 10;
  ldi t0,10
  sto t0,-8[fp]
; s.x = 3;
  ldi t0,3
  sto t0,0[s0]
; s.y = 5;
  ldi t0,5
  sto t0,8[s0]
; return s.y - s.x - 2;
  ldo t2,8[s0]
  ldo t3,0[s0]
  sub t1,t2,t3
  sub a0,t1,2
.00010:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00017,@function
	.size	_main00017,$-_main00017


;--}

	.global	_main00017
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
