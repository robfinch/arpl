 	.bss
	.align 4
_x:
	.type	_x,@object
	.size	_x,8
	.8byte	0
 
	.text

;{++ _main00070

	.sdreg	29
_main00070:
  lda a0,_x[gp]
.00010:
  rts 
	.type	_main00070,@function
	.size	_main00070,$-_main00070


;--}

	.global	_main00070
	.extern	_start_data
	.extern	_start_bss
	.global	_x
	.extern	_start_rodata
