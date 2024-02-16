   
	.text
 
;{++ _main00052

	.sdreg	29
_main00052:
; s.x = 0;
  sto r0,0[t0]
; return s.x;
  ldo t0,0[t0]
  ldo a0,0[t0]
.00010:
  rts 
	.type	_main00052,@function
	.size	_main00052,$-_main00052


;--}

	.global	_main00052
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
