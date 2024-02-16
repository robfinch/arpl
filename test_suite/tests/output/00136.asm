 	.bss
	.align 4
_a:
	.type	_a,@object
	.size	_a,8

 	.bss
	.align 4
_b:
	.type	_b,@object
	.size	_b,8

 	.bss
	.align 4
_c:
	.type	_c,@object
	.size	_c,8

 	.bss
	.align 4
_d:
	.type	_d,@object
	.size	_d,8

 	.bss
	.align 4
_e:
	.type	_e,@object
	.size	_e,8

 	.bss
	.align 4
_f:
	.type	_f,@object
	.size	_f,8

 	.bss
	.align 4
_e_:
	.type	_e_,@object
	.size	_e_,8

 	.bss
	.align 4
_f_:
	.type	_f_,@object
	.size	_f_,8

 	.bss
	.align 4
_e_:
	.type	_e_,@object
	.size	_e_,8

 	.bss
	.align 4
_f_:
	.type	_f_,@object
	.size	_f_,8

 
	.text

;{++ _main00136

	.sdreg	29
_main00136:
  mov a0,r0
.00010:
  rts 
	.type	_main00136,@function
	.size	_main00136,$-_main00136


;--}

	.global	_main00136
	.extern	_start_data
	.global	_e_
	.global	_f_
	.extern	_start_bss
	.global	_a
	.global	_b
	.global	_c
	.global	_d
	.global	_e
	.global	_f
	.extern	_start_rodata
