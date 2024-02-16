 
	.text

;{++ _main00001

	.sdreg	29
_main00001:
  mov a0,r0
.00010:
  rts 
	.type	_main00001,@function
	.size	_main00001,$-_main00001


;--}

	.global	_main00001
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
