    
	.text

;{++ _main00042

	.sdreg	29
_main00042:
; u.a = 1;
  ldi t0,1
  sto t0,0[t0]
; u.b = 3;
  ldi t0,3
  sto t0,0[t0]
; if (u.a != 3 || u.b != 3)
  ldo t0,0[t0]
  ldi t1,3
  bne t0,t1,.00016
  ldo t0,0[t0]
  beq t0,t1,.00014
.00016:
; return 1;
  ldi a0,1
.00013:
  rts 
.00014:
; return 0;
  mov a0,r0
  bra .00013
	.type	_main00042,@function
	.size	_main00042,$-_main00042


;--}

	.global	_main00042
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
