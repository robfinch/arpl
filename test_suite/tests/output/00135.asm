    
	.text

;{++ _main00135

	.sdreg	29
_main00135:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,48
  sto s0,[sp]
  sto s1,8[sp]
; i = -1ll < 0;
  ldi s0,1
; u = -1llu < 0;
  ldi s1,1
; return 0;
  mov a0,r0
.00010:
  ldo s0,[sp]
  ldo s1,8[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00135,@function
	.size	_main00135,$-_main00135


;--}

	.global	_main00135
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
