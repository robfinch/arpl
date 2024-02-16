    
	.text

;{++ _main00135

	.sdreg	29
_main00135:
; return 0;
  mov a0,r0
.00010:
  rts 
	.type	_main00135,@function
	.size	_main00135,$-_main00135


;--}

	.global	_main00135
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
