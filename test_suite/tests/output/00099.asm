     
	.text

;{++ _vecresize
	.sdreg	29
_vecresize:
.00011:
  rts 
	.type	_vecresize,@function
	.size	_vecresize,$-_vecresize


;--}
 
;{++ _main00099

	.sdreg	29
_main00099:
  mov a0,r0
.00021:
  rts 
	.type	_main00099,@function
	.size	_main00099,$-_main00099


;--}

	.global	_main00099
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
