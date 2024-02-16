  
	.text
    
;{++ _main

	.sdreg	29
_main:
; integer a = 10;
  ldi t0,10
  ldi t2,1
  ldi t3,2
  ldi t1,3
  ldi t4,4
; return ( a ?? b : c : d : e );
  bnez t0,.00017
  mov t0,t2
  bra .00016
.00017:
  ldi t1,1
  bne t0,t1,.00018
  mov t0,t3
  bra .00016
.00018:
  ldi t1,2
  bne t0,t1,.00019
  mov t0,t1
  bra .00016
.00019:
  mov t0,t4
.00020:
.00016:
  mov a0,t0
.00015:
  rts 
	.type	_main,@function
	.size	_main,$-_main


;--}

	.global	_main
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
