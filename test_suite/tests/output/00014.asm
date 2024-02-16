   
	.text

;{++ _main00014

	.sdreg	29
_main00014:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,48
  sto s0,[sp]
  sto s1,8[sp]
  lda s0,-8[fp]
; x = 1;
  ldi t0,1
  sto t0,[s0]
; p = &x;
  mov s1,s0
; p[0] = 0;
  sto r0,0[s1]
; return x;
  ldo a0,[s0]
.00010:
  ldo s0,[sp]
  ldo s1,8[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00014,@function
	.size	_main00014,$-_main00014


;--}

	.global	_main00014
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
