  
	.text

;{++ _main00006

	.sdreg	29
_main00006:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; x = 50;
  ldi s0,50
; while x do
  beqz s0,.00014
.00013:
; x = x - 1;
  sub s0,s0,1
  bnez s0,.00013
.00014:
; return x;
  mov a0,s0
.00012:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00006,@function
	.size	_main00006,$-_main00006


;--}

	.global	_main00006
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
