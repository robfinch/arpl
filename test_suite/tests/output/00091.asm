    	.bss
	.align 4
_a:
	.type	_a,@object
	.size	_a,24
	.8byte	1,2,3
 
	.text

;{++ _main00091

	.sdreg	29
_main00091:
; if (a[0].v != 1)
  lda t1,00091.00024[gp]
  ldo t1,0[t1]
  ldi t2,1
  beq t1,t2,.00022
; return 1;
  ldi a0,1
.00021:
  rts 
.00022:
; if (a[0].sub[0] != 2)
  lda t3,00091.00024[gp]
  add t2,t3,r0
  add t1,t2,8
  ldo t1,0[t1]
  ldi t2,2
  beq t1,t2,.00024
; return 2;
  ldi a0,2
  bra .00021
.00024:
; if (a[0].sub[1] != 3)
  lda t3,00091.00024[gp]
  add t2,t3,r0
  add t1,t2,8
  ldo t1,1[t1]
  ldi t2,3
  beq t1,t2,.00026
; return 3;
  ldi a0,3
  bra .00021
.00026:
; return 0;
  mov a0,r0
  bra .00021
	.type	_main00091,@function
	.size	_main00091,$-_main00091


;--}

	.rodata
	.align	6

	.align	8







	.global	_main00091
	.extern	_start_data
	.extern	_start_bss
	.global	_a
	.extern	_start_rodata
