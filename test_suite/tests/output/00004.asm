    
	.text

;{++ _main00004

	.sdreg	29
_main00004:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
; x = 4;
  ldi t0,4
  sto t0,-8[fp]
; p = &x;
  lda t0,-8[fp]
; *p = 0;
  sto r0,[t0]
; return *p;
  ldo t0,[t0]
; return *p;
  ldo a0,[t0]
.00010:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00004,@function
	.size	_main00004,$-_main00004


;--}

	.global	_main00004
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
