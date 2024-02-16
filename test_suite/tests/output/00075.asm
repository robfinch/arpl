 
	.text

;{++ _main00075

	.sdreg	29
_main00075:
  mov a0,r0
.00010:
  rts 
	.type	_main00075,@function
	.size	_main00075,$-_main00075


;--}

	.global	_main00075
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
