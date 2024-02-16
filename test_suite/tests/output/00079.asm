   
	.text

;{++ _main00079

	.sdreg	29
_main00079:
; y = 0;
  mov t1,r0
; x = ((y) + 1);
  add t0,1,t1
; if(x != 1)
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
	.type	_main00079,@function
	.size	_main00079,$-_main00079


;--}

	.global	_main00079
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
