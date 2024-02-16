    
	.text

;{++ _main00153

	.sdreg	29
_main00153:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; s.f = 0;
  mov s0,r0
  mov a0,r0
.00010:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00153,@function
	.size	_main00153,$-_main00153


;--}

	.global	_main00153
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
