    	.bss
	.align 4
_tick:
	.type	_tick,@object
	.size	_tick,8

 
	.text

;{++ _foo

	.sdreg	29
_foo:
  lda t0,_tick[gp]
  lda t2,_tick[gp]
  add t1,t2,1
  sto t1,[t0]
.00010:
  rti 
	.type	_foo,@function
	.size	_foo,$-_foo


;--}
    