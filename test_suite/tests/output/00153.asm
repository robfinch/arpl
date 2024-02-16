    
	.text

;{++ _main00153

	.sdreg	29
_main00153:
; s.f = 0;
  sto r0,0[t0]
; return s.f;
  ldo t0,0[t0]
  ldo a0,0[t0]
.00010:
  rts 
	.type	_main00153,@function
	.size	_main00153,$-_main00153


;--}

	.global	_main00153
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
