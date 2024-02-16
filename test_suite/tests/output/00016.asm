   
	.text

;{++ _main00016

	.sdreg	29
_main00016:
; p = &arr[1];
  add t0,t0,1
; p = &arr[1];
  add t1,t0,1
; *p = 0;
  sto r0,[t1]
; return arr[1];
  ldo t0,8[t0]
  ldo a0,8[t0]
.00010:
  rts 
	.type	_main00016,@function
	.size	_main00016,$-_main00016


;--}

	.global	_main00016
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
