  
	.text

;{++ _main00101

	.sdreg	29
_main00101:
.00013:
;while (0);
  ldi t0,0
  bnez t0,.00013
; return c;
  mov a0,t0
.00012:
  rts 
	.type	_main00101,@function
	.size	_main00101,$-_main00101


;--}

	.global	_main00101
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
