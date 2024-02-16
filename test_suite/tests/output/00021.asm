   
	.text

;{++ _foo

	.sdreg	29
_foo:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
  ldo t2,32[fp]
  add t1,t2,2
  ldo t2,40[fp]
  sub a0,t1,t2
.00010:
  mov sp,fp
  ldo fp,[sp]
  rtd 48,0
	.type	_foo,@function
	.size	_foo,$-_foo


;--}
 
;{++ _main00021

	.sdreg	29
_main00021:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  sub sp,sp,16
  ldi t0,1
  sto t0,0[sp]
  ldi t0,3
  sto t0,8[sp]
  bsr _foo
.00020:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00021,@function
	.size	_main00021,$-_main00021


;--}

	.global	_foo
	.global	_main00021
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
