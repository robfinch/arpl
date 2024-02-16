 
	.text

;{++ _main00122

	.sdreg	29
_main00122:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
  sub sp,sp,8
  ldi t0,1
  sto t0,0[sp]
  bsr _F
  add sp,sp,8
.00010:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00122,@function
	.size	_main00122,$-_main00122


;--}

	.extern	_F
	.global	_main00122
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
