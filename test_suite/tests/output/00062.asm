 	.bss
	.align 4
_x:
	.type	_x,@object
	.size	_x,8
	.8byte	0
 
	.text

;{++ _main00062

	.sdreg	29
_main00062:
  lda a0,00062.00000[gp]
.00010:
  rts 
	.type	_main00062,@function
	.size	_main00062,$-_main00062


;--}

	.global	_main00062
	.extern	_start_data
	.extern	_start_bss
	.global	_x
	.extern	_start_rodata
