  
	.text

;{++ _main00036

	.sdreg	29
_main00036:
; x = 0;
  mov t0,r0
; x += 2;
  add t0,t0,2
  add t0,t0,2
; if (x != 4)
  ldi t1,4
  beq t0,t1,.00017
; return 1;
  ldi a0,1
.00016:
  rts 
.00017:
; x -= 1;
  sub t0,t0,1
; if (x != 3)
  ldi t1,3
  beq t0,t1,.00019
; return 2;
  ldi a0,2
  bra .00016
.00019:
; x *= 2;
  mul t0,t0,2
; if (x != 6)
  ldi t1,6
  beq t0,t1,.00021
; return 3;
  ldi a0,3
  bra .00016
.00021:
; return 0;
  mov a0,r0
  bra .00016
	.type	_main00036,@function
	.size	_main00036,$-_main00036


;--}

	.global	_main00036
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
