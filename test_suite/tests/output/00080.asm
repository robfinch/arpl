 
	.text

;{++ _voidfn

	.sdreg	29
_voidfn:
.00010:
  rts 
	.type	_voidfn,@function
	.size	_voidfn,$-_voidfn


;--}
 
;{++ _main00080

	.sdreg	29
_main00080:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; voidfn();
  bsr _voidfn
; return 0;
  mov a0,r0
.00020:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00080,@function
	.size	_main00080,$-_main00080


;--}

	.global	_voidfn
	.global	_main00080
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
