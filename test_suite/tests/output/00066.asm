 
	.text

;{++ _main00066

	.sdreg	29
_main00066:
; if(1 + 2 + 3 != 6)
  ldi t0,0
  beqz t0,.00013
; return 1 ;
  ldi a0,1
.00012:
  rts 
.00013:
; return 0 + 0 + 0;
  mov a0,r0
  bra .00012
	.type	_main00066,@function
	.size	_main00066,$-_main00066


;--}

	.global	_main00066
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
