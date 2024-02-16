  
	.text

;{++ _main00081

	.sdreg	29
_main00081:
; x = 0;
  mov t0,r0
; x = x + 1;
  add t0,t0,1
; if (x != 1)
  ldi t1,1
  beq t0,t1,.00013
; return 1;
  ldi a0,1
.00012:
  rts 
.00013:
; return 0;
  mov a0,r0
  bra .00012
	.type	_main00081,@function
	.size	_main00081,$-_main00081


;--}

	.global	_main00081
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
