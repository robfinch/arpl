  
	.text
 
;{++ _main00111

	.sdreg	29
_main00111:
; short s = 1;
  ldi t0,1
  ldi t1,1
; s -= l;
  sub t0,t0,1
; s -= l;
  sub a0,t0,1
.00010:
  rts 
	.type	_main00111,@function
	.size	_main00111,$-_main00111


;--}

	.global	_main00111
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
