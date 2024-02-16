     
	.text

;{++ _main00018

	.sdreg	29
_main00018:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; p = &s;
  lda t0,-16[fp]
  lda s0,-16[fp]
; s.x = 1;
  ldo t0,[fp]
  ldi t1,1
  sto t1,-16[t0]
; p->y = 2;
  ldi t0,2
  sto t0,128[s0]
; return p->y + p->x - 3;
  ldou t2,128[s0]
  ldo t3,[s0]
  add t1,t2,t3
  sub a0,t1,3
.00010:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00018,@function
	.size	_main00018,$-_main00018


;--}

	.global	_main00018
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
