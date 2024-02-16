  
	.text
 
;{++ _main00113

	.sdreg	29
_main00113:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,48
  sto s0,[sp]
  sto s1,8[sp]
; int a = 0;
  mov s0,r0
  add t1,s0,1
  itof t0,t1
  itof s1,t1
; return f == a;
  itof t1,s0
  ;empty t0,s1,t1,1
  mov a0,t0
.00012:
  ldo s0,[sp]
  ldo s1,8[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00113,@function
	.size	_main00113,$-_main00113


;--}

	.global	_main00113
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
