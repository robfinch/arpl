 	.bss
	.align 4
_x:
	.type	_x,@object
	.size	_x,2

 	.bss
	.align 4
_Y:
	.type	_Y,@object
	.size	_Y,2

 
	.text

;{++ _foo

	.sdreg	29
_foo:
  ldi a0,4
.00010:
  rts 
	.type	_foo,@function
	.size	_foo,$-_foo


;--}
 