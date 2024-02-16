  
	.text

;{++ _main00008

	.sdreg	29
_main00008:
; x = 50;
  ldi t0,50
.00013:
; x = x - 1;
  sub t0,t0,1
;while x;
  bnez t0,.00013
; return x;
  mov a0,t0
.00012:
  rts 
	.type	_main00008,@function
	.size	_main00008,$-_main00008


;--}

	.global	_main00008
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
