  
	.text

;{++ _main00009

	.sdreg	29
_main00009:
; x = 1;
  ldi t0,1
; x = x * 10;
  mul t0,t0,10
; x = x / 2;
  asr t0,t0,1
; x = x % 3;
  div t1,t0,3
  mul t1,t1,3
  sub t0,t0,t1
; return x - 2;
  sub t0,t0,2
; return x - 2;
  sub a0,t0,2
.00010:
  rts 
	.type	_main00009,@function
	.size	_main00009,$-_main00009


;--}

	.global	_main00009
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
