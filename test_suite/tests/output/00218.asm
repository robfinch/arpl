 	.bss
	.align 4
__noname_var0:
	.type	__noname_var0,@object
	.size	__noname_var0,2

      	.bss
	.align 4
__noname_var1:
	.type	__noname_var1,@object
	.size	__noname_var1,24

  	.bss
	.align 4
__noname_var2:
	.type	__noname_var2,@object
	.size	__noname_var2,24

 	.bss
	.align 4
__noname_var3:
	.type	__noname_var3,@object
	.size	__noname_var3,2

 	.bss
	.align 4
__noname_var4:
	.type	__noname_var4,@object
	.size	__noname_var4,2

    
	.text

;{++ _blah

	.sdreg	29
_blah:
  mov a0,r0
.00010:
  rts 
	.type	_blah,@function
	.size	_blah,$-_blah


;--}
  