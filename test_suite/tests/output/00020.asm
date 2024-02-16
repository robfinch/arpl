  
	.text

;{++ _main00020

	.sdreg	29
_main00020:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; x = 0;
  sto r0,-8[fp]
; p = &x;
  lda t0,-8[fp]
  sto t0,-16[fp]
; pp = &p;
  lda s0,-16[fp]
; return **pp;
  ldou a0,[s0]
.00010:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00020,@function
	.size	_main00020,$-_main00020


;--}

	.global	_main00020
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
