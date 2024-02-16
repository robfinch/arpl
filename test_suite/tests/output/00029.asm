  
	.text

;{++ _main00029

	.sdreg	29
_main00029:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; x = 1;
  ldi s0,1
; x = x ^ 3;
  xor s0,s0,3
; return x - 2;
  sub a0,s0,2
.00010:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00029,@function
	.size	_main00029,$-_main00029


;--}

	.global	_main00029
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
