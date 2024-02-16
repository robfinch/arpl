   
	.text

;{++ _main00022

	.sdreg	29
_main00022:
; v = 0;
  mov a0,r0
.00010:
  rts 
	.type	_main00022,@function
	.size	_main00022,$-_main00022


;--}

	.global	_main00022
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
