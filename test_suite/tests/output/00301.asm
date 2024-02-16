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
  
;{++ _main00301

	.sdreg	29
_main00301:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; x = foo();
  bsr _foo
  sto a0,-8[fp]
; printf("hello world");
  sub sp,sp,8
  lda t0,_main00301.00011[gp]
  sto t0,0[sp]
  bsr _printf
  add sp,sp,8
; return (x[5:3]);
  ldo t1,-8[fp]
  extu a0,t1,3,5
.00021:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00301,@function
	.size	_main00301,$-_main00301


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00301.00011,@object
	.size	_main00301.00011,24
_main00301.00011: ; hello world

	.2byte	104,101,108,108,111,32,119,111
	.2byte	114,108,100,0
	.extern	_printf
	.global	_foo
	.global	_main00301
	.extern	_start_data
	.extern	_start_bss
	.global	_Y
	.extern	_start_rodata
