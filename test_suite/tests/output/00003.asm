  
	.text

;{++ _main00003

	.sdreg	29
_main00003:
; x = 4;
  ldi t0,4
; return x - 4;
  sub t0,t0,4
; return x - 4;
  sub a0,t0,4
.00010:
  rts 
	.type	_main00003,@function
	.size	_main00003,$-_main00003


;--}

	.global	_main00003
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
