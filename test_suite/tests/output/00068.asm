 	.bss
	.align 4
_x:
	.type	_x,@object
	.size	_x,8
	.8byte	0
 
	.text

;{++ _main00068

	.sdreg	29
_main00068:
  lda a0,_x[gp]
.00010:
  rts 
	.type	_main00068,@function
	.size	_main00068,$-_main00068


;--}

	.global	_main00068
	.extern	_start_data
	.extern	_start_bss
	.global	_x
	.extern	_start_rodata
