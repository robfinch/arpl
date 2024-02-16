 	.bss
	.align 4
_arr:
	.type	_arr,@object
	.size	_arr,320
	.8byte	0,0,3,5,1
	.8byte	0,7,6



	.8byte	1,2,0,0,0
	.8byte	0,0,0





  
	.text

;{++ _main00151

	.sdreg	29
_main00151:
  lda t4,00151.00000[gp]
  add t3,t4,r0
  add t2,t3,r0
  ldo t2,4[t2]
  lda t5,00151.00000[gp]
  add t4,t5,40
  add t3,t4,r0
  ldo t3,4[t3]
  zseq t1,t2,t3,1
  not a0,t1
.00025:
  rts 
	.type	_main00151,@function
	.size	_main00151,$-_main00151


;--}

	.rodata
	.align	6

	.align	8















	.global	_main00151
	.extern	_start_data
	.extern	_start_bss
	.global	_arr
	.extern	_start_rodata
