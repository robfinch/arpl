 	.bss
	.align 4
_x:
	.type	_x,@object
	.size	_x,8
	.8byte	0
 
	.text

;{++ _main00067

	.sdreg	29
_main00067:
  lda a0,00067.00000[gp]
.00010:
  rts 
	.type	_main00067,@function
	.size	_main00067,$-_main00067


;--}

	.global	_main00067
	.extern	_start_data
	.extern	_start_bss
	.global	_x
	.extern	_start_rodata
