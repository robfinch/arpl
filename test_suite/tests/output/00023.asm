 	.bss
	.align 4
_x:
	.type	_x,@object
	.size	_x,8

 
	.text

;{++ _main00023

	.sdreg	29
_main00023:
; x = 0;
  lda t0,00023.00000[gp]
  sto r0,[t0]
; return x;
  lda a0,00023.00000[gp]
.00010:
  rts 
	.type	_main00023,@function
	.size	_main00023,$-_main00023


;--}

	.global	_main00023
	.extern	_start_data
	.extern	_start_bss
	.global	_x
	.extern	_start_rodata
