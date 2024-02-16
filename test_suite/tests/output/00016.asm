   
	.text

;{++ _main00016

	.sdreg	29
_main00016:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,48
  sto s0,[sp]
  sto s1,8[sp]
  lda s0,-16[fp]
; p = &arr[1];
  add s1,s0,1
; *p = 0;
  sto r0,[s1]
; return arr[1];
  ldo a0,8[s0]
.00010:
  ldo s0,[sp]
  ldo s1,8[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00016,@function
	.size	_main00016,$-_main00016


;--}

	.global	_main00016
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
