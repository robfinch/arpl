    
	.text

;{++ _main00005

	.sdreg	29
_main00005:
; x = 0;
  sto r0,[t1]
; p = &x;
  sto t1,[t0]
; pp = &p;
  mov t2,t0
; if (*p) then
  ldo t0,[t0]
  beqz t0,.00017
; return 1;
  ldi a0,1
.00016:
  rts 
.00017:
; if (**pp) then
  ldou t0,[t2]
  beqz t0,.00019
; return 1;
  bra .00016
.00019:
; **pp = 1;
  ldi t0,1
.00020:
; if (x) then
  ldo t0,[t1]
  beqz t0,.00021
; return 0;
  mov a0,r0
  bra .00016
.00021:
; return 1;
  ldi a0,1
  bra .00016
  bra .00016
	.type	_main00005,@function
	.size	_main00005,$-_main00005


;--}

	.global	_main00005
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
