  
	.text

;{++ _main00027

	.sdreg	29
_main00027:
; x = 1;
  ldi t0,1
; x = x | 4;
  or t0,t0,4
; return x - 5;
  sub t0,t0,5
; return x - 5;
  sub a0,t0,5
.00010:
  rts 
	.type	_main00027,@function
	.size	_main00027,$-_main00027


;--}

	.global	_main00027
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
