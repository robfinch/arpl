  
	.text

;{++ _main00008

	.sdreg	29
_main00008:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; x = 50;
  ldi s0,50
.00013:
; x = x - 1;
  sub s0,s0,1
;while x;
  bnez s0,.00013
; return x;
  mov a0,s0
.00012:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00008,@function
	.size	_main00008,$-_main00008


;--}

	.global	_main00008
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
