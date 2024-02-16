   
	.text

;{++ _main00072

	.sdreg	29
_main00072:
; *p = 123;
  ldi t0,123
  sto t0,[t0]
; if(arr[1] != 123)
  ldo t1,8[t1]
  ldi t2,123
  beq t1,t2,.00013
; return 1;
  ldi a0,1
.00012:
  rts 
.00013:
; return 0;
  mov a0,r0
  bra .00012
	.type	_main00072,@function
	.size	_main00072,$-_main00072


;--}

	.global	_main00072
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
