  
	.text

;{++ _main00057

	.sdreg	29
_main00057:
; if(sizeof(a) != sizeof(b))
  ldi t0,0
  beqz t0,.00013
; return 1;
  ldi a0,1
.00012:
  rts 
.00013:
; return 0;
  mov a0,r0
  bra .00012
	.type	_main00057,@function
	.size	_main00057,$-_main00057


;--}

	.global	_main00057
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
