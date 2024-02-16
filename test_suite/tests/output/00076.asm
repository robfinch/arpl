 
	.text

;{++ _main00076

	.sdreg	29
_main00076:
; if(0 ? 1 : 0)
  ldi t0,0
  beqz t0,.00015
; return 1;
  ldi a0,1
.00014:
  rts 
.00015:
; if(1 ? 0 : 1)
  ldi t0,0
  beqz t0,.00017
; return 2;
  ldi a0,2
  bra .00014
.00017:
; return 0;
  mov a0,r0
  bra .00014
	.type	_main00076,@function
	.size	_main00076,$-_main00076


;--}

	.global	_main00076
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
