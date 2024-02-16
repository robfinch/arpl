   
	.text

;{++ _main00013

	.sdreg	29
_main00013:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
; x = 0;
  sto r0,-8[fp]
; p = &x;
  lda t0,-8[fp]
; return p[0];
  ldo t0,0[t0]
  ldo a0,0[t0]
.00010:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00013,@function
	.size	_main00013,$-_main00013


;--}

	.global	_main00013
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
