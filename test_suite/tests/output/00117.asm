
	.text

;{++ _main00117

	.sdreg	29
_main00117:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
; return x[1];
  lda t0,-8[fp]
  ldo t0,8[t0]
  ldo a0,8[t0]
.00011:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00117,@function
	.size	_main00117,$-_main00117


;--}

	.rodata
	.align	6

	.align	8




	.align	8

	.align 4
_x:
	.type	_x,@object
	.size	_x,8
	.8byte	1,0
	.global	_main00117
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
