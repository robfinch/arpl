   
	.text

;{++ _main00011

	.sdreg	29
_main00011:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; x = y = 0;
  sto r0,-16[fp]
  ldo s0,-16[fp]
  ldo a0,-16[fp]
.00010:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00011,@function
	.size	_main00011,$-_main00011


;--}

	.global	_main00011
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
