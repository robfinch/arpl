    
	.text

;{++ _main00103

	.sdreg	29
_main00103:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
; x = 0;
  sto r0,-8[fp]
; foo = (void*)&x;
  lda t0,-8[fp]
  sto t0,-16[fp]
; bar = &foo;
  lda t0,-16[fp]
; return **(int**)bar;
  ldou t0,[t0]
; return **(int**)bar;
  ldou a0,[t0]
.00010:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00103,@function
	.size	_main00103,$-_main00103


;--}

	.global	_main00103
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata