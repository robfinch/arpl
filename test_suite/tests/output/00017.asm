     
	.text

;{++ _main00017

	.sdreg	29
_main00017:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
  ldh t1,[fp]
; aa = 10;
  ldi t0,10
  sto t0,-8[fp]
; s.x = 3;
  ldi t0,3
  sto t0,0[t0]
; s.y = 5;
  ldi t0,5
  sto t0,8[t0]
; return s.y - s.x - 2;
  sto t1,-32[fp]
  ldo t1,8[t0]
  ldo t2,0[t0]
  sub t1,t1,t2
  sub a0,t1,2
.00010:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00017,@function
	.size	_main00017,$-_main00017


;--}

	.global	_main00017
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
