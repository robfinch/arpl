    	.bss
	.align 4
_s:
	.type	_s,@object
	.size	_s,24
	.8byte	1,2,3
 
	.text

;{++ _main00047

	.sdreg	29
_main00047:
; if (s.a != 1)
  lda t1,00047.00000[gp]
  ldi t2,1
  beq t1,t2,.00019
; return 1;
  ldi a0,1
.00018:
  rts 
.00019:
; if (s.b != 2)
  lda t1,00047.00000[gp]
  ldou t1,8[t1]
  ldi t2,2
  beq t1,t2,.00021
; return 2;
  ldi a0,2
  bra .00018
.00021:
; if (s.c != 3)
  lda t1,00047.00000[gp]
  ldou t1,16[t1]
  ldi t2,3
  beq t1,t2,.00023
; return 3;
  ldi a0,3
  bra .00018
.00023:
; return 0;
  mov a0,r0
  bra .00018
	.type	_main00047,@function
	.size	_main00047,$-_main00047


;--}

	.rodata
	.align	6

	.align	8




	.global	_main00047
	.extern	_start_data
	.extern	_start_bss
	.global	_s
	.extern	_start_rodata
