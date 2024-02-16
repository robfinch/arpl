 	.bss
	.align 4
_x:
	.type	_x,@object
	.size	_x,8
	.8byte	0
 
	.text

;{++ _main00063

	.sdreg	29
_main00063:
  mov a0,r0
.00010:
  rts 
	.type	_main00063,@function
	.size	_main00063,$-_main00063


;--}

	.global	_main00063
	.extern	_start_data
	.extern	_start_bss
	.global	_x
	.extern	_start_rodata
