    	.bss
	.align 4
_v:
	.type	_v,@object
	.size	_v,16

 
	.text

;{++ _main00024

	.sdreg	29
_main00024:
; v.x = 1;
  lda t0,_v[gp]
  ldi t1,1
  sto t1,0[t0]
; v.y = 2;
  lda t0,_v[gp]
  ldi t1,2
  sto t1,8[t0]
; return 3 - v.x - v.y;
  ldi t2,3
  lda t3,_v[gp]
  ldo t3,0[t3]
  sub t1,t2,t3
  lda t2,_v[gp]
  ldo t2,8[t2]
  sub a0,t1,t2
.00010:
  rts 
	.type	_main00024,@function
	.size	_main00024,$-_main00024


;--}

	.global	_main00024
	.extern	_start_data
	.extern	_start_bss
	.global	_v
	.extern	_start_rodata
