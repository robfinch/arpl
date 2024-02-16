 
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
  lda a0,_s[gp]
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
_main00089:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  ldo t0,_go
  sto t0,-32[fp]
.00041:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00089,@function
	.size	_main00089,$-_main00089


;--}

	.rodata
	.align	6

	.align	8



	.global	_main00089
	.global	_anon
	.global	_zero
	.global	_go
	.extern	_start_data
	.extern	_start_bss
	.global	_s
	.extern	_start_rodata
