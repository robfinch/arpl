  	.bss
	.align 4
_x:
	.type	_x,@object
	.size	_x,8

 
	.text

;{++ _main00110

	.sdreg	29
_main00110:
  lda a0,_x[gp]
.00010:
  rts 
	.type	_main00110,@function
	.size	_main00110,$-_main00110


;--}

	.global	_main00110
	.extern	_start_data
	.extern	_start_bss
	.global	_x
	.extern	_start_rodata
