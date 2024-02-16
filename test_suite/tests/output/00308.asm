    
	.text

;{++ _main

	.sdreg	29
_main:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
; for (n = 0; n < 10; n++)
  mov s0,r0
  ldi t1,10
  bge s0,t1,.00016
.00015:
; puts("Hello World!");
  sub sp,sp,8
  lda t0,_main.00001[gp]
  sto t0,0[sp]
  bsr _puts
  iblt s0,t1,.00015
.00016:
.00014:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main,@function
	.size	_main,$-_main


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main.00001,@object
	.size	_main.00001,26
_main.00001: ; Hello World!

	.2byte	72,101,108,108,111,32,87,111
	.2byte	114,108,100,33,0
	.global	_main
	.extern	_puts
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
