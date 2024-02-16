 
	.text

;{++ _main00112

	.sdreg	29
_main00112:
  lda t1,_main00112.00001[gp]
  zseq a0,t1,0,1
.00013:
  rts 
	.type	_main00112,@function
	.size	_main00112,$-_main00112


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00112.00001,@object
	.size	_main00112.00001,8
_main00112.00001: ; abc

	.2byte	97,98,99,0
	.global	_main00112
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
