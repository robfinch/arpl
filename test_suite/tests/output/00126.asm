  
	.text

;{++ _main00126

	.sdreg	29
_main00126:
; x = 3;
  ldi t0,3
; x = !x;
  not t0,t0
  not t0,t0
; x = ~x;
  com t0,t0
; x = -x;
  neg t0,t0
; if(x != 2)
  ldi t1,2
  beq t0,t1,.00013
; return 1;
  ldi a0,1
.00012:
  rts 
.00013:
; return 0;
  mov a0,r0
  bra .00012
	.type	_main00126,@function
	.size	_main00126,$-_main00126


;--}

	.global	_main00126
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
