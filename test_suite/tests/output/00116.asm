  
	.text

;{++ _f

	.sdreg	29
_f:
  ldi a0,_f
.00010:
  rts 
	.type	_f,@function
	.size	_f,$-_f


;--}
 
;{++ _main00116

	.sdreg	29
_main00116:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  sub sp,sp,8
  sto r0,0[sp]
  bsr _f
.00020:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00116,@function
	.size	_main00116,$-_main00116


;--}

	.global	_f
	.global	_main00116
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
