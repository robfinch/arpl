  
	.text

;{++ _main00028

	.sdreg	29
_main00028:
; x = 1;
  ldi t0,1
; x = x & 3;
  and t0,t0,3
; return x - 1;
  sub t0,t0,1
; return x - 1;
  sub a0,t0,1
.00010:
  rts 
	.type	_main00028,@function
	.size	_main00028,$-_main00028


;--}

	.global	_main00028
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
