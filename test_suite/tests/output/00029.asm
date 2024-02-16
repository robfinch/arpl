  
	.text

;{++ _main00029

	.sdreg	29
_main00029:
; x = 1;
  ldi t0,1
; x = x ^ 3;
  xor t0,t0,3
; return x - 2;
  sub t0,t0,2
; return x - 2;
  sub a0,t0,2
.00010:
  rts 
	.type	_main00029,@function
	.size	_main00029,$-_main00029


;--}

	.global	_main00029
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
