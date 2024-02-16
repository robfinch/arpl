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
	.8byte	3
 	.bss
	.align 4
_x:
	.type	_x,@object
	.size	_x,8

  
	.text

;{++ _foo

	.sdreg	29
_foo:
  ldi a0,_main00095
.00010:
  rts 
	.type	_foo,@function
	.size	_foo,$-_foo


;--}
 
;{++ _main00095

	.sdreg	29
_main00095:
; if (x != 3)
  lda t1,00095.00016[gp]
  ldi t2,3
  beq t1,t2,.00023
; return 0;
  mov a0,r0
.00022:
  rts 
.00023:
; x = 0;
  lda t0,00095.00016[gp]
  sto r0,[t0]
; return x;
  lda a0,00095.00016[gp]
  bra .00022
	.type	_main00095,@function
	.size	_main00095,$-_main00095


;--}

	.global	_main00095
	.global	_foo
	.extern	_start_data
	.extern	_start_bss
	.global	_x
	.extern	_start_rodata
