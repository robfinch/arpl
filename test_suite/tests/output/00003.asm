  
	.text

;{++ _main00003

	.sdreg	29
_main00003:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; x = 4;
  ldi s0,4
; return x - 4;
  sub a0,s0,4
.00010:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00003,@function
	.size	_main00003,$-_main00003


;--}

	.global	_main00003
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
