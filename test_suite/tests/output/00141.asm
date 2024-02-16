   
	.text

;{++ _main00141

	.sdreg	29
_main00141:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,56
  sto s0,[sp]
  sto s1,8[sp]
  sto s2,16[sp]
; foobar = foo + bar;
  add s2,s0,s1
; return 0;
  mov a0,r0
.00010:
  ldo s0,[sp]
  ldo s1,8[sp]
  ldo s2,16[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00141,@function
	.size	_main00141,$-_main00141


;--}

	.global	_main00141
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
