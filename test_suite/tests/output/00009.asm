  
	.text

;{++ _main00009

	.sdreg	29
_main00009:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; x = 1;
  ldi s0,1
; x = x * 10;
  mul s0,s0,10
; x = x / 2;
  asr s0,s0,1
; x = x % 3;
  div t1,s0,3
  mul t1,t1,3
  sub s0,s0,t1
; return x - 2;
  sub a0,s0,2
.00010:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00009,@function
	.size	_main00009,$-_main00009


;--}

	.global	_main00009
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
