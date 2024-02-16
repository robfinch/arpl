  
	.text

;{++ _foo

	.sdreg	29
_foo:
  mov a0,r0
.00010:
  rts 
	.type	_foo,@function
	.size	_foo,$-_foo


;--}
 
;{++ _main00100

	.sdreg	29
_main00100:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  bsr _foo
.00020:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00100,@function
	.size	_main00100,$-_main00100


;--}

	.global	_foo
	.global	_main00100
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
