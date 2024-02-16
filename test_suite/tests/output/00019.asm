    
	.text

;{++ _main00019

	.sdreg	29
_main00019:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,48
  sto s0,[sp]
  sto s1,8[sp]
  lda t0,-16[fp]
  lda s1,-16[fp]
; s.x = 0;
  sto r0,128[s1]
; s.p = &s;
  mov s0,s1
; return s.p->p->p->p->p->x;
  ldo t0,[s0]
  ldo t0,[t0]
  ldo t0,[t0]
  ldou t0,128[t0]
  ldou a0,128[t0]
.00010:
  ldo s0,[sp]
  ldo s1,8[sp]
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
