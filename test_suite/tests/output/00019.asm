    
	.text

;{++ _main00019

	.sdreg	29
_main00019:
; s.x = 0;
  sto r0,8[t0]
; s.p = &s;
  sto t0,0[t0]
; return s.p->p->p->p->p->x;
  ldo t0,0[t0]
  ldo t0,0[t0]
  ldo t0,0[t0]
  ldo t0,0[t0]
  ldo t0,0[t0]
  ldo t0,8[t0]
  ldo a0,8[t0]
.00010:
  rts 
	.type	_main00019,@function
	.size	_main00019,$-_main00019


;--}

	.global	_main00019
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
