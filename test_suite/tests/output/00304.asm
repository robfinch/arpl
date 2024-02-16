   
	.text
  
;{++ _foo

	.sdreg	29
_foo:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; i = 47;
  ldi t0,47
  sto t0,-8[fp]
; printf("%0d",
  sub sp,sp,16
  lda t0,_foo.00001[gp]
  sto t0,0[sp]
  sto t1,8[sp]
  bsr _printf
  add sp,sp,16
.00002:
.00003:
.00004:
; default: printf("default");
  sub sp,sp,8
  lda t0,_foo.00010[gp]
  sto t0,0[sp]
  bsr _printf
  add sp,sp,8
  bra .-0001
.00004:
; default: printf("default");
  sub sp,sp,8
  lda t0,_foo.00010[gp]
  sto t0,0[sp]
  bsr _printf
  add sp,sp,8
  bra .-0001
.00008:
; default: printf("default");
  sub sp,sp,8
  lda t0,_foo.00010[gp]
  sto t0,0[sp]
  bsr _printf
  add sp,sp,8
.00022:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_foo,@function
	.size	_foo,$-_foo


;--}
      
;{++ _main00304

	.sdreg	29
_main00304:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; i = 47;
  ldi t0,47
  sto t0,-8[fp]
.00024:
  sub sp,sp,8
  lda t0,_main00304.00025[gp]
  sto t0,0[sp]
  bsr _printf
  add sp,sp,8
  sto a0,-8[fp]
; case long: printf("long");
  sub sp,sp,8
  lda t0,_main00304.00028[gp]
  sto t0,0[sp]
  bsr _printf
  add sp,sp,8
; case float: printf("float");
  sub sp,sp,8
  lda t0,_main00304.00031[gp]
  sto t0,0[sp]
  bsr _printf
  add sp,sp,8
; case double: printf("double");
  sub sp,sp,8
  lda t0,_main00304.00034[gp]
  sto t0,0[sp]
  bsr _printf
  add sp,sp,8
; case long double: qf=4.5Q;//67.25Q;
  sub sp,sp,24
  ldi t0,16
  sto t0,16[sp]
  ldh t0,0x4480[gp]
  sto t0,8[sp]
  ldh t0,-24[fp]
  sto t0,[sp]
  bsr _aacpy
; default: printf("default");
  sub sp,sp,8
  lda t0,_main00304.00036[gp]
  sto t0,0[sp]
  bsr _printf
  add sp,sp,8
  bra .-0001
  sub sp,sp,8
  lda t0,_main00304.00028[gp]
  sto t0,0[sp]
  bsr _printf
  add sp,sp,8
; case float: printf("float");
  sub sp,sp,8
  lda t0,_main00304.00031[gp]
  sto t0,0[sp]
  bsr _printf
  add sp,sp,8
; case double: printf("double");
  sub sp,sp,8
  lda t0,_main00304.00034[gp]
  sto t0,0[sp]
  bsr _printf
  add sp,sp,8
; case long double: qf=4.5Q;//67.25Q;
  sub sp,sp,24
  ldi t0,16
  sto t0,16[sp]
  ldh t0,0x4480[gp]
  sto t0,8[sp]
  ldh t0,-24[fp]
  sto t0,[sp]
  bsr _aacpy
; default: printf("default");
  sub sp,sp,8
  lda t0,_main00304.00036[gp]
  sto t0,0[sp]
  bsr _printf
  add sp,sp,8
  bra .-0001
  sub sp,sp,8
  lda t0,_main00304.00031[gp]
  sto t0,0[sp]
  bsr _printf
  add sp,sp,8
.00032:
  sub sp,sp,8
  lda t0,_main00304.00034[gp]
  sto t0,0[sp]
  bsr _printf
  add sp,sp,8
; case long double: qf=4.5Q;//67.25Q;
  sub sp,sp,24
  ldi t0,16
  sto t0,16[sp]
  ldh t0,0x4480[gp]
  sto t0,8[sp]
  ldh t0,-24[fp]
  sto t0,[sp]
  bsr _aacpy
; default: printf("default");
  sub sp,sp,8
  lda t0,_main00304.00036[gp]
  sto t0,0[sp]
  bsr _printf
  add sp,sp,8
  bra .-0001
.00033:
  sub sp,sp,24
  ldi t0,16
  sto t0,16[sp]
  ldh t0,0x4480[gp]
  sto t0,8[sp]
  ldh t0,-24[fp]
  sto t0,[sp]
  bsr _aacpy
; default: printf("default");
  sub sp,sp,8
  lda t0,_main00304.00036[gp]
  sto t0,0[sp]
  bsr _printf
  add sp,sp,8
.00048:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00304,@function
	.size	_main00304,$-_main00304


;--}
 
	.rodata
	.align	6

	.align	8



	.align	6
	.type	_foo.00001,@object
	.size	_foo.00001,8
_foo.00001: ; %0d

	.2byte	37,48,100,0

	.type	_foo.00010,@object
	.size	_foo.00010,16
_foo.00010: ; default

	.2byte	100,101,102,97,117,108,116,0

	.type	_main00304.00025,@object
	.size	_main00304.00025,24
_main00304.00025: ; hello world

	.2byte	104,101,108,108,111,32,119,111
	.2byte	114,108,100,0

	.type	_main00304.00028,@object
	.size	_main00304.00028,10
_main00304.00028: ; long

	.2byte	108,111,110,103,0

	.type	_main00304.00031,@object
	.size	_main00304.00031,12
_main00304.00031: ; float

	.2byte	102,108,111,97,116,0

	.type	_main00304.00034,@object
	.size	_main00304.00034,14
_main00304.00034: ; double

	.2byte	100,111,117,98,108,101,0

	.type	_main00304.00036,@object
	.size	_main00304.00036,16
_main00304.00036: ; default

	.2byte	100,101,102,97,117,108,116,0
	.extern	_printf
	.global	_foo
	.global	_main00304
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
