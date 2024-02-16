 	.bss
	.align 4
_a:
	.type	_a,@object
	.size	_a,8
	.8byte	1,2,3,4
 
	.text

;{++ _main00093

	.sdreg	29
_main00093:
; if (sizeof(a) != 4*sizeof(int))
  ldi t0,1
  beqz t0,.00014
; return 1;
  ldi a0,1
.00013:
  rts 
.00014:
; return 0;
  mov a0,r0
  bra .00013
	.type	_main00093,@function
	.size	_main00093,$-_main00093


;--}

	.rodata
	.align	6

	.align	8



	.global	_main00093
	.extern	_start_data
	.extern	_start_bss
	.global	_a
	.extern	_start_rodata
