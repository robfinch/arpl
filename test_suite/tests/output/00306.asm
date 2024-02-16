        
	.text

;{++ _foo

	.sdreg	29
_foo:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,72
  sto s0,[sp]
; i = 47;
  ldi t0,47
  sto t0,-8[fp]
; ma = 0x3f;
  ldi t0,63
  sto t0,-16[fp]
; mb = ma + 0x20;
  ldo t1,-16[fp]
  add t0,t1,32
  sto t0,-24[fp]
; df = 15.5D;
  ldo s0,0x4bc0[gp]
; vqf = ma(mb(vqf + df) * once);
  mov v4,t7
  vfadds v2,v4,s0,t0
  loadv v5,64[fp]
  vfmul v0,v2,v5,t1
  ldo a0,v0
.00010:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 128,0
	.type	_foo,@function
	.size	_foo,$-_foo


;--}
    
;{++ _main00306

	.sdreg	29
_main00306:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,48
  sto s0,[sp]
  sto s1,8[sp]
; qf = foo(125, vec);
  sub sp,sp,16
  ldi t0,125
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _foo
  mov s1,a0
; i = 47;
  ldi s0,47
; qf = (long double)_Generic(15.3,
  fcvtdq t0,s1
  ;empty.q t0,-48[fp]
  sub sp,sp,8
  lda t1,_main00306.00014[gp]
  sto t1,0[sp]
  bsr _printf
  add sp,sp,8
; return (i);
  mov a0,s0
.00025:
  ldo s0,[sp]
  ldo s1,8[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00306,@function
	.size	_main00306,$-_main00306


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00306.00011,@object
	.size	_main00306.00011,24
_main00306.00011: ; hello world

	.2byte	104,101,108,108,111,32,119,111
	.2byte	114,108,100,0

	.type	_main00306.00012,@object
	.size	_main00306.00012,10
_main00306.00012: ; long

	.2byte	108,111,110,103,0

	.type	_main00306.00013,@object
	.size	_main00306.00013,12
_main00306.00013: ; float

	.2byte	102,108,111,97,116,0

	.type	_main00306.00014,@object
	.size	_main00306.00014,14
_main00306.00014: ; double

	.2byte	100,111,117,98,108,101,0

	.type	_main00306.00015,@object
	.size	_main00306.00015,16
_main00306.00015: ; default

	.2byte	100,101,102,97,117,108,116,0
	.extern	_printf
	.global	_foo
	.global	_main00306
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
