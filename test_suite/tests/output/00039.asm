   
	.text

;{++ _main00039

	.sdreg	29
_main00039:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; x = 2;
  ldi t0,2
  sto t0,-16[fp]
; p = &x;
  ldo t1,[fp]
  lda s0,-16[fp]
; if(*((int*)p) != 2)
  ldo t1,[s0]
  ldi t2,2
  beq t1,t2,.00013
; return 1;
  ldi a0,1
.00012:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00013:
; return 0;
  mov a0,r0
  bra .00012
	.type	_main00039,@function
	.size	_main00039,$-_main00039


;--}

	.global	_main00039
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
