     
	.text

;{++ _foo

	.sdreg	29
_foo:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; i = 47;
  ldi s0,47
; printf("%0d",
  sub sp,sp,16
  lda t0,_foo.00001[gp]
  sto t0,0[sp]
  orf t0,r0,0
  sto t0,8[sp]
  bsr _printf
  add sp,sp,16
; return (i);
  mov a0,s0
.00012:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_foo,@function
	.size	_foo,$-_foo


;--}
   
;{++ _bar

	.sdreg	29
_bar:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
; i = 47;
  ldi s0,47
; x = 10;
  ldi t0,10
  sto t0,-16[fp]
; printf("%0d",
  sub sp,sp,16
  lda t0,_bar.00013[gp]
  sto t0,0[sp]
  ldo t0,-16[fp]
  sto t0,-40[fp]
  sto t1,-32[fp]
  sub sp,sp,8
  lda t2,_bar.00014[gp]
  sto t2,0[sp]
  bsr _printf
  add sp,sp,8
  mov t1,a0
  sto t1,8[sp]
  bsr _printf
  add sp,sp,16
; return (i);
  mov a0,s0
.00025:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_bar,@function
	.size	_bar,$-_bar


;--}
   
;{++ _main00305

	.sdreg	29
_main00305:
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
  lda t1,_main00305.00030[gp]
  sto t1,0[sp]
  bsr _printf
  add sp,sp,8
; return (i);
  mov a0,s0
.00041:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00305,@function
	.size	_main00305,$-_main00305


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_foo.00001,@object
	.size	_foo.00001,8
_foo.00001: ; %0d

	.2byte	37,48,100,0

	.type	_foo.00002,@object
	.size	_foo.00002,16
_foo.00002: ; default

	.2byte	100,101,102,97,117,108,116,0

	.type	_bar.00013,@object
	.size	_bar.00013,8
_bar.00013: ; %0d

	.2byte	37,48,100,0

	.type	_bar.00014,@object
	.size	_bar.00014,16
_bar.00014: ; default

	.2byte	100,101,102,97,117,108,116,0

	.type	_main00305.00027,@object
	.size	_main00305.00027,24
_main00305.00027: ; hello world

	.2byte	104,101,108,108,111,32,119,111
	.2byte	114,108,100,0

	.type	_main00305.00028,@object
	.size	_main00305.00028,10
_main00305.00028: ; long

	.2byte	108,111,110,103,0

	.type	_main00305.00029,@object
	.size	_main00305.00029,12
_main00305.00029: ; float

	.2byte	102,108,111,97,116,0

	.type	_main00305.00030,@object
	.size	_main00305.00030,14
_main00305.00030: ; double

	.2byte	100,111,117,98,108,101,0

	.type	_main00305.00031,@object
	.size	_main00305.00031,16
_main00305.00031: ; default

	.2byte	100,101,102,97,117,108,116,0
	.extern	_printf
	.global	_bar
	.global	_foo
	.global	_main00305
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
