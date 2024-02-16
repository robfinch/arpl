  	.bss
	.align 4
__noname_var0:
	.type	__noname_var0,@object
	.size	__noname_var0,8

  
	.text
  
;{++ _main00044

	.sdreg	29
_main00044:
; v.x = 2;
  ldi t0,2
  sto t0,0[t0]
; if(v.x != 2)
  ldo t0,0[t0]
  ldi t1,2
  beq t0,t1,.00013
; return 1;
  ldi a0,1
.00012:
  rts 
.00013:
; return 0;
  mov a0,r0
  bra .00012
	.type	_main00044,@function
	.size	_main00044,$-_main00044


;--}

	.global	_main00044
	.extern	_start_data
	.global	__noname_var0
	.extern	_start_bss
	.extern	_start_rodata
