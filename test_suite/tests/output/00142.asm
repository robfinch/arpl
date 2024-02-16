 	.bss
	.align 4
_c:
	.type	_c,@object
	.size	_c,8

  
	.text

;{++ _main00142

	.sdreg	29
_main00142:
  lda a0,_c[gp]
.00010:
  rts 
	.type	_main00142,@function
	.size	_main00142,$-_main00142


;--}

	.global	_main00142
	.extern	_start_data
	.extern	_start_bss
	.global	_c
	.extern	_start_rodata
