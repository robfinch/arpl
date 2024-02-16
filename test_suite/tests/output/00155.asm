  
	.text

;{++ _main00155

	.sdreg	29
_main00155:
; return 0;
  mov a0,r0
.00010:
  rts 
	.type	_main00155,@function
	.size	_main00155,$-_main00155


;--}

	.global	_main00155
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
