   
	.text

;{++ _main00014

	.sdreg	29
_main00014:
; x = 1;
  ldi t0,1
  sto t0,[t0]
; p = &x;
  mov t1,t0
; p[0] = 0;
  sto r0,0[t1]
; return x;
  ldo t0,[t0]
; return x;
  ldo a0,[t0]
.00010:
  rts 
	.type	_main00014,@function
	.size	_main00014,$-_main00014


;--}

	.global	_main00014
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
