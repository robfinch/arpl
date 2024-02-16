   
	.text
  
;{++ _main00053

	.sdreg	29
_main00053:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
  lda t1,-8[fp]
; s1.x = 1;
  ldi t0,1
  sto t0,0[t1]
; s2.y = 1;
  sto t0,8[t0]
; if (s1.x - s2.y != 0)
  ldo t1,0[t1]
  sto t1,-32[fp]
  ldo t1,8[t0]
  sub t1,t1,t1
  beq t1,r0,.00013
; return 1;
  ldi a0,1
.00012:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00013:
; return 0;
  mov a0,r0
  bra .00012
	.type	_main00053,@function
	.size	_main00053,$-_main00053


;--}

	.global	_main00053
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
