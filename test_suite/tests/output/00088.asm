   
	.text

;{++ _main00088

	.sdreg	29
_main00088:
; if (fptr)
  lda t0,_fptr[gp]
  beqz t0,.00013
; return 1;
  ldi a0,1
.00012:
  rts 
.00013:
; return 0;
  mov a0,r0
  bra .00012
	.type	_main00088,@function
	.size	_main00088,$-_main00088


;--}

	.global	_main00088
	.global	_fptr
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
