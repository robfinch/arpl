   
	.text
 
;{++ _main00052

	.sdreg	29
_main00052:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; s.x = 0;
  mov s0,r0
  mov a0,r0
.00010:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00052,@function
	.size	_main00052,$-_main00052


;--}

	.global	_main00052
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
