   	.bss
	.align 4
__noname_var0:
	.type	__noname_var0,@object
	.size	__noname_var0,8

 
	.text

;{++ _foo

	.sdreg	29
_foo:
  mov a0,r0
.00010:
  rts 
	.type	_foo,@function
	.size	_foo,$-_foo


;--}
  
;{++ _main00087

	.sdreg	29
_main00087:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
; v.fptr = foo;
  ldi s0,0
; return v.fptr();
  jsr [s0]
.00020:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00087,@function
	.size	_main00087,$-_main00087


;--}

	.global	_main00087
	.global	_foo
	.extern	_start_data
	.global	__noname_var0
	.extern	_start_bss
	.extern	_start_rodata
