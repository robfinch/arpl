 	.bss
	.align 4
_x:
	.type	_x,@object
	.size	_x,8
	.8byte	5
 	.bss
	.align 4
_y:
	.type	_y,@object
	.size	_y,8,6,0
 	.bss
	.align 4
_p:
	.type	_p,@object
	.size	_p,8
	.8byte	_x
 
	.text

;{++ _main00045

	.sdreg	29
_main00045:
; if (x != 5)
  lda t1,_x[gp]
  ldi t2,5
  beq t1,t2,.00017
; return 1;
  ldi a0,1
.00016:
  rts 
.00017:
; if (y != 6)
  lda t1,_y[gp]
  ldi t2,6
  beq t1,t2,.00019
; return 2;
  ldi a0,2
  bra .00016
.00019:
; if (*p != 5)
  lda t1,_p[gp]
  ldo t1,[t1]
  ldi t2,5
  beq t1,t2,.00021
; return 3;
  ldi a0,3
  bra .00016
.00021:
; return 0;
  mov a0,r0
  bra .00016
	.type	_main00045,@function
	.size	_main00045,$-_main00045


;--}

	.global	_main00045
	.extern	_start_data
	.extern	_start_bss
	.global	_p
	.global	_x
	.global	_y
	.extern	_start_rodata
