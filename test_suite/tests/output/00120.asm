  	.bss
	.align 4
_s:
	.type	_s,@object
	.size	_s,2

 
	.text

;{++ _main00120

	.sdreg	29
_main00120:
  mov a0,r0
.00010:
  rts 
	.type	_main00120,@function
	.size	_main00120,$-_main00120


;--}

	.global	_main00120
	.extern	_start_data
	.extern	_start_bss
	.global	_s
	.extern	_start_rodata
