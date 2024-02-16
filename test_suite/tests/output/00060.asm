 
	.text

;{++ _main00060

	.sdreg	29
_main00060:
  mov a0,r0
.00010:
  rts 
	.type	_main00060,@function
	.size	_main00060,$-_main00060


;--}

	.global	_main00060
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
