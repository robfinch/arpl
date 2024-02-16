  
	.text
 
;{++ _main00113

	.sdreg	29
_main00113:
; int a = 0;
  mov t0,r0
  add t1,t0,1
  itof t0,t1
; return f == a;
  itof t1,t0
  ;empty t0,t1,t1,1
  mov a0,t0
.00012:
  rts 
	.type	_main00113,@function
	.size	_main00113,$-_main00113


;--}

	.global	_main00113
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
