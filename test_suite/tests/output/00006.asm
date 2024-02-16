  
	.text

;{++ _main00006

	.sdreg	29
_main00006:
; x = 50;
  ldi t0,50
; while x do
  beqz t0,.00014
.00013:
; x = x - 1;
  sub t0,t0,1
  bnez t0,.00013
.00014:
; return x;
  mov a0,t0
.00012:
  rts 
	.type	_main00006,@function
	.size	_main00006,$-_main00006


;--}

	.global	_main00006
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
