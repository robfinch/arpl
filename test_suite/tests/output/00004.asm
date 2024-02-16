    
	.text

;{++ _main00004

	.sdreg	29
_main00004:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; x = 4;
  ldi t0,4
  sto t0,-8[fp]
; p = &x;
  lda s0,-8[fp]
; *p = 0;
  sto r0,[s0]
; return *p;
  ldo a0,[s0]
.00010:
  ldo s0,[sp]
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
