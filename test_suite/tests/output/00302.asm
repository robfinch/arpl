 	.bss
	.align 4
_x:
	.type	_x,@object
	.size	_x,2

 	.bss
	.align 4
_Y:
	.type	_Y,@object
	.size	_Y,2

 	.bss
	.align 4
_bit_array:
	.type	_bit_array,@object
	.size	_bit_array,128

 
	.text

;{++ _foo

	.sdreg	29
_foo:
  ldi a0,4
.00010:
  rts 
	.type	_foo,@function
	.size	_foo,$-_foo


;--}
  
;{++ _main00302

	.sdreg	29

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00302.00011,@object
	.size	_main00302.00011,24
_main00302.00011: ; hello world

	.2byte	104,101,108,108,111,32,119,111
	.2byte	114,108,100,0
	.extern	_printf
	.global	_foo
	.global	_main00302
	.extern	_start_data
	.global	_bit_array
	.extern	_start_bss
	.global	_Y
	.global	_x
	.extern	_start_rodata
