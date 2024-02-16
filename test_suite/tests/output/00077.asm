    
	.text

;{++ _foo

	.sdreg	29
_foo:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,56
  sto s0,[sp]
  sto s1,8[sp]
  sto s2,16[sp]
  lda s1,32[fp]
  ldo t1,[fp]
  lda s2,-800[fp]
; y[0] = 2000;
  ldi t0,2000
  sto t0,0[s2]
; if(x[0] != 1000)
  ldo t1,0[s1]
  ldi t2,1000
  beq t1,t2,.00021
; return 1;
  ldi a0,1
.00020:
  ldo s0,[sp]
  ldo s1,8[sp]
  ldo s2,16[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 832,0
.00021:
; p = x;
  mov s0,s1
; if(p[0] != 1000)
  ldo t1,0[s0]
  ldi t2,1000
  beq t1,t2,.00023
; return 2;
  ldi a0,2
  bra .00020
.00023:
; p = y;
  mov s0,s2
; if(p[0] != 2000)
  ldo t1,0[s0]
  ldi t2,2000
  beq t1,t2,.00025
; return 3;
  ldi a0,3
  bra .00020
.00025:
; if(sizeof(x) != sizeof(void*))
  ldi t0,1
  beqz t0,.00027
; return 4;
  ldi a0,4
  bra .00020
.00027:
; if(sizeof(y) <= sizeof(x))
  ldi t0,1
  beqz t0,.00029
; return 5;
  ldi a0,5
  bra .00020
.00029:
; return 0;
  mov a0,r0
  bra .00020
	.type	_foo,@function
	.size	_foo,$-_foo


;--}
  
;{++ _main00077

	.sdreg	29
_main00077:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
  lda s0,-800[fp]
; x[0] = 1000;
  ldi t0,1000
  sto t0,0[s0]
; return foo(x);
  sub sp,sp,8
  sto s0,0[sp]
  bsr _foo
.00040:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00077,@function
	.size	_main00077,$-_main00077


;--}

	.global	_main00077
	.global	_foo
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
