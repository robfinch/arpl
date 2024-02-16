  
	.text
    
;{++ _main

	.sdreg	29
_main:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,72
  sto s0,[sp]
  sto s1,8[sp]
  sto s2,16[sp]
  sto s3,24[sp]
  sto s4,32[sp]
; integer a = 10;
  ldi s0,10
  ldi s2,1
  ldi s3,2
  ldi s1,3
  ldi s4,4
; return ( a ?? b : c : d : e );
  bnez s0,.00017
  mov t0,s2
  bra .00016
.00017:
  ldi t1,1
  bne s0,t1,.00018
  mov t0,s3
  bra .00016
.00018:
  ldi t1,2
  bne s0,t1,.00019
  mov t0,s1
  bra .00016
.00019:
  mov t0,s4
.00020:
.00016:
  mov a0,t0
.00015:
  ldo s0,[sp]
  ldo s1,8[sp]
  ldo s2,16[sp]
  ldo s3,24[sp]
  ldo s4,32[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main,@function
	.size	_main,$-_main


;--}

	.global	_main
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
