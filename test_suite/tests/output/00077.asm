    
	.text

;{++ _foo

	.sdreg	29
_foo:
; y[0] = 2000;
  ldi t0,2000
  sto t0,0[t2]
; if(x[0] != 1000)
  ldo t1,0[t1]
  ldi t2,1000
  beq t1,t2,.00021
; return 1;
  ldi a0,1
.00020:
  rts 
.00021:
; p = x;
  mov t0,t1
; if(p[0] != 1000)
  ldo t0,0[t0]
  ldi t1,1000
  beq t0,t1,.00023
; return 2;
  ldi a0,2
  bra .00020
.00023:
; p = y;
  mov t0,t2
; if(p[0] != 2000)
  ldo t0,0[t0]
  ldi t1,2000
  beq t0,t1,.00025
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
  sub sp,sp,32
; x[0] = 1000;
  ldi t0,1000
  sto t0,0[t0]
; return foo(x);
  sub sp,sp,8
  sto t0,0[sp]
  bsr _foo
.00040:
  ldo lr0,8[fp]
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
