   
	.text

;{++ _main00141

	.sdreg	29
_main00141:
; return 0;
  mov a0,r0
.00010:
  rts 
	.type	_main00141,@function
	.size	_main00141,$-_main00141


;--}

	.global	_main00141
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
