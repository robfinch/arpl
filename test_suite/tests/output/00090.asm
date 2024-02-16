 	.bss
	.align 4
_a:
	.type	_a,@object
	.size	_a,24
	.8byte	0,1,2
 
	.text

;{++ _main00090

	.sdreg	29
_main00090:
; if (a[0] != 0)
  lda t1,_a[gp]
  ldo t1,0[t1]
  beq t1,r0,.00018
; return 1;
  ldi a0,1
.00017:
  rts 
.00018:
; if (a[1] != 1)
  lda t1,_a[gp]
  ldo t1,1[t1]
  ldi t2,1
  beq t1,t2,.00020
; return 2;
  ldi a0,2
  bra .00017
.00020:
; if (a[2] != 2)
  lda t1,_a[gp]
  ldo t1,2[t1]
  ldi t2,2
  beq t1,t2,.00022
; return 3;
  ldi a0,3
  bra .00017
.00022:
; return 0;
  mov a0,r0
  bra .00017
	.type	_main00090,@function
	.size	_main00090,$-_main00090


;--}

	.rodata
	.align	6

	.align	8



	.global	_main00090
	.extern	_start_data
	.extern	_start_bss
	.global	_a
	.extern	_start_rodata
