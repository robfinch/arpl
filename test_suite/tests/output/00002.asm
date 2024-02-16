 
	.text

;{++ _main00002

	.sdreg	29
_main00002:
  mov a0,r0
.00010:
  rts 
	.type	_main00002,@function
	.size	_main00002,$-_main00002


;--}

	.global	_main00002
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
