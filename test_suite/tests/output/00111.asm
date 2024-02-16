  
	.text
 
;{++ _main00111

	.sdreg	29
_main00111:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,48
  sto s0,[sp]
  sto s1,8[sp]
; short s = 1;
  ldi s0,1
  ldi s1,1
; s -= l;
  sub s0,s0,1
; s -= l;
  sub a0,s0,1
.00010:
  ldo s0,[sp]
  ldo s1,8[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00111,@function
	.size	_main00111,$-_main00111


;--}

	.global	_main00111
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
