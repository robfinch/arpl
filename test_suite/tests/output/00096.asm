 	.bss
	.align 4
_x:
	.type	_x,@object
	.size	_x,8

	.bss
	.align 4
_x:
	.type	_x,@object
	.size	_x,8
	.8byte	0
 
	.text

;{++ _main00096

	.sdreg	29
_main00096:
; if (x != 3)
  lda t1,_x[gp]
  ldi t2,3
  beq t1,t2,.00013
; return 0;
  mov a0,r0
.00012:
  rts 
.00013:
; x = 0;
  lda t0,_x[gp]
  sto r0,[t0]
; return x;
  lda a0,_x[gp]
  bra .00012
	.type	_main00096,@function
	.size	_main00096,$-_main00096


;--}

	.global	_main00096
	.extern	_start_data
	.extern	_start_bss
	.global	_x
	.extern	_start_rodata
