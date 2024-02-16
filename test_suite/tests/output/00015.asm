  
	.text

;{++ _main00015

	.sdreg	29
_main00015:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
  lda s0,-16[fp]
; arr[0] = 1;
  ldi t0,1
  sto t0,0[s0]
; arr[1] = 2;
  ldi t0,2
  sto t0,8[s0]
; return arr[0] + arr[1] - 3;
  ldo t2,0[s0]
  ldo t3,8[s0]
  add t1,t2,t3
  sub a0,t1,3
.00010:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00015,@function
	.size	_main00015,$-_main00015


;--}

	.global	_main00015
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
