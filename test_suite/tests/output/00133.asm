    
	.text

;{++ _main00133

	.sdreg	29
_main00133:
; return 0;
  mov a0,r0
.00010:
  rts 
	.type	_main00133,@function
	.size	_main00133,$-_main00133


;--}

	.global	_main00133
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
