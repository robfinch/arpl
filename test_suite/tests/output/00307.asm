    	.bss
	.align 4
_tick:
	.type	_tick,@object
	.size	_tick,8

 
	.text

;{++ _foo

	.sdreg	29
_foo:
  lda t0,00307.00000[gp]
  lda t2,00307.00000[gp]
  add t1,t2,1
  sto t1,[t0]
.00010:
  rti 
	.type	_foo,@function
	.size	_foo,$-_foo


;--}
    
;{++ _main00307

	.sdreg	29
_main00307:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
; i = 47;
  ldi s0,47
; qf = (long double)_Generic(15.3,
  ldh t1,-16[fp]
  fcvtdq t0,t1
  ;empty.q t0,-40[fp]
  sub sp,sp,8
  lda t1,_main00307.00014[gp]
  sto t1,0[sp]
  bsr _printf
  add sp,sp,8
; return (i);
  mov a0,s0
.00025:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00307,@function
	.size	_main00307,$-_main00307


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00307.00011,@object
	.size	_main00307.00011,24
_main00307.00011: ; hello world

	.2byte	104,101,108,108,111,32,119,111
	.2byte	114,108,100,0

	.type	_main00307.00012,@object
	.size	_main00307.00012,10
_main00307.00012: ; long

	.2byte	108,111,110,103,0

	.type	_main00307.00013,@object
	.size	_main00307.00013,12
_main00307.00013: ; float

	.2byte	102,108,111,97,116,0

	.type	_main00307.00014,@object
	.size	_main00307.00014,14
_main00307.00014: ; double

	.2byte	100,111,117,98,108,101,0

	.type	_main00307.00015,@object
	.size	_main00307.00015,16
_main00307.00015: ; default

	.2byte	100,101,102,97,117,108,116,0
	.extern	_printf
	.global	_foo
	.global	_main00307
	.extern	_start_data
	.global	_tick
	.extern	_start_bss
	.extern	_start_rodata
