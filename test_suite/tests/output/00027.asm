  
	.text

;{++ _main00027

	.sdreg	29
_main00027:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; x = 1;
  ldi s0,1
; x = x | 4;
  or s0,s0,4
; return x - 5;
  sub a0,s0,5
.00010:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00027,@function
	.size	_main00027,$-_main00027


;--}

	.global	_main00027
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
