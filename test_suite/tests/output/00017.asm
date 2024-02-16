    
	.text

;{++ _main00017

	.sdreg	29
_main00017:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,48
  sto s0,[sp]
  sto s1,8[sp]
  lda s0,-16[fp]
; s.x = 3;
  ldi s1,3
; s.y = 5;
  ldi t0,5
  sto t0,128[s0]
; return s.y - s.x - 2;
  ldo t2,128[s0]
  sub t1,t2,3
  sub a0,t1,2
.00010:
  ldo s0,[sp]
  ldo s1,8[sp]
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
