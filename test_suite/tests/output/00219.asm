                                                                                                                                                  	.bss
	.align 4
_a:
	.type	_a,@object
	.size	_a,8
	.8byte	0
  	.bss
	.align 4
__noname_var0:
	.type	__noname_var0,@object
	.size	__noname_var0,8

  	.bss
	.align 4
__noname_var1:
	.type	__noname_var1,@object
	.size	__noname_var1,8

 
	.text

;{++ _a_f

	.sdreg	29
_a_f:
  ldi a0,20
.00010:
  rts 
	.type	_a_f,@function
	.size	_a_f,$-_a_f


;--}
 
;{++ _b_f

	.sdreg	29
_b_f:
  ldi a0,10
.00020:
  rts 
	.type	_b_f,@function
	.size	_b_f,$-_b_f


;--}
     
;{++ _foo

	.sdreg	29
_foo:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
  ldo a0,32[fp]
.00030:
  mov sp,fp
  ldo fp,[sp]
  rtd 40,0
	.type	_foo,@function
	.size	_foo,$-_foo


;--}
        