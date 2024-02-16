   	.bss
	.align 4
__noname_var0:
	.type	__noname_var0,@object
	.size	__noname_var0,16

 	.bss
	.align 4
_s:
	.type	_s,@object
	.size	_s,16
	.8byte	1,2
 
	.text

;{++ _main00146

	.sdreg	29
_main00146:
; if(s.a != 1)
  lda t1,00146.00000[gp]
  ldi t2,1
  beq t1,t2,.00017
; return 1;
  ldi a0,1
.00016:
  rts 
.00017:
; if(s.b != 2)
  lda t1,00146.00016[gp]
  ldou t1,8[t1]
  ldi t2,2
  beq t1,t2,.00019
; return 2;
  ldi a0,2
  bra .00016
.00019:
; return 0;
  mov a0,r0
  bra .00016
	.type	_main00146,@function
	.size	_main00146,$-_main00146


;--}

	.rodata
	.align	6

	.align	8




	.global	_main00146
	.extern	_start_data
	.global	__noname_var0
	.extern	_start_bss
	.global	_s
	.extern	_start_rodata
