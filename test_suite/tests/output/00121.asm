   	.bss
	.align 4
_a:
	.type	_a,@object
	.size	_a,8

 
	.text

;{++ _main00121

	.sdreg	29
_main00121:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  sto t0,-32[fp]
  sub sp,sp,8
  ldi t1,1
  sto t1,0[sp]
  bsr _f
  ldo t0,-32[fp]
  sto t0,-32[fp]
  sub sp,sp,8
  sto t1,0[sp]
  bsr _g
  sub a0,a0,a0
.00010:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00121,@function
	.size	_main00121,$-_main00121


;--}
  
;{++ _f

	.sdreg	29
_f:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
  ldo a0,24[fp]
.00020:
  mov sp,fp
  ldo fp,[sp]
  rtd 40,0
	.type	_f,@function
	.size	_f,$-_f


;--}
  
;{++ _g

	.sdreg	29
_g:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
  ldo a0,24[fp]
.00030:
  mov sp,fp
  ldo fp,[sp]
  rtd 40,0
	.type	_g,@function
	.size	_g,$-_g


;--}

	.global	_f
	.global	_g
	.global	_main00121
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
