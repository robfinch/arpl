 
	.text

;{++ _zero

	.sdreg	29
_zero:
  mov a0,r0
.00010:
  rts 
	.type	_zero,@function
	.size	_zero,$-_zero


;--}
   	.align 4
_s:
	.type	_s,@object
	.size	_s,8
	.8byte	_zero
 
;{++ _anon

	.sdreg	29
_anon:
  lda a0,00089.00000[gp]
.00021:
  rts 
	.type	_anon,@function
	.size	_anon,$-_anon


;--}
   
;{++ _go

	.sdreg	29
_go:
  ldi a0,_anon
.00031:
  rts 
	.type	_go,@function
	.size	_go,$-_go


;--}
 
;{++ _main00089

	.sdreg	29
