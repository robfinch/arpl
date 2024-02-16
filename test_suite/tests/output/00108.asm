     
	.text

;{++ _main00108

	.sdreg	29
_main00108:
  mov a0,r0
.00010:
  rts 
	.type	_main00108,@function
	.size	_main00108,$-_main00108


;--}

	.global	_foo
	.global	_main00108
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
