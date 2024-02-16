 	.bss
	.align 4
_x:
	.type	_x,@object
	.size	_x,8
	.8byte	0
 
	.text

;{++ _main00074

	.sdreg	29
_main00074:
  mov a0,r0
.00010:
  rts 
	.type	_main00074,@function
	.size	_main00074,$-_main00074


;--}

	.global	_main00074
	.extern	_start_data
	.extern	_start_bss
	.global	_x
	.extern	_start_rodata
