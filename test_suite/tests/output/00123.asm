 	.bss
	.align 4
_x:
	.type	_x,@object
	.size	_x,8
	.4byte	0x00000000,0x40590000
 
	.text

;{++ _main00123

	.sdreg	29
_main00123:
  lda t1,_x[gp]
  ldo t2,0x3c00[gp]
  ;empty t0,t1,t2,1
  mov a0,t0
.00013:
  rts 
	.type	_main00123,@function
	.size	_main00123,$-_main00123


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00123.00006,@object
	.size	_main00123.00006,8
_main00123.00006:
	.4byte	0x00000000,0x3FF00000
	.global	_main00123
	.extern	_start_data
	.extern	_start_bss
	.global	_x
	.extern	_start_rodata
