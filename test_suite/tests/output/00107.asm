  	.bss
	.align 4
_x:
	.type	_x,@object
	.size	_x,8
	.8byte	1
  
	.text

;{++ _main00107

	.sdreg	29
_main00107:
  lda t1,_x[gp]
  sub a0,t1,1
.00010:
  rts 
	.type	_main00107,@function
	.size	_main00107,$-_main00107


;--}

	.global	_main00107
	.extern	_start_data
	.extern	_start_bss
	.global	_x
	.extern	_start_rodata
