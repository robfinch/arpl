    
	.text

;{++ _main00019

	.sdreg	29
_main00019:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
  lda s0,-16[fp]
; s.x = 0;
  sto r0,8[s0]
; s.p = &s;
  sto s0,0[s0]
; return s.p->p->p->p->p->x;
  ldo s0,0[s0]
  ldo s0,0[s0]
  ldo s0,0[s0]
  ldo s0,0[s0]
  ldo s0,0[s0]
  ldo a0,8[s0]
.00010:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00019,@function
	.size	_main00019,$-_main00019


;--}

	.global	_main00019
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
