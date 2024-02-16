 
	.text

;{++ _main00012

	.sdreg	29
_main00012:
  mov a0,r0
.00010:
  rts 
	.type	_main00012,@function
	.size	_main00012,$-_main00012


;--}

	.global	_main00012
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
