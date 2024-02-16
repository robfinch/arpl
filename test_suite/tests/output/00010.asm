 
	.text

;{++ _main00010

	.sdreg	29
_main00010:
; goto next;
  bra .00002
.00014:
  rts 
.00003:
; return 0;
  mov a0,r0
  bra .00014
.00002:
; goto success;
  bra .00003
	.type	_main00010,@function
	.size	_main00010,$-_main00010


;--}

	.global	_main00010
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
