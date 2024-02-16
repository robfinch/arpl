   
	.text

;{++ _main00032

	.sdreg	29
_main00032:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,48
  sto s0,[sp]
  sto s1,8[sp]
  ldo t1,[fp]
  lda s1,-16[fp]
; arr[0] = 2;
  ldi t0,2
  sto t0,0[s1]
; arr[1] = 3;
  ldi t0,3
  sto t0,8[s1]
; p = &arr[0];
  add s0,s1,r0
; if(*(p++) != 2)
  ldo t1,[s0]
  ldi t2,2
  beq t1,t2,.00023
; return 1;
  ldi a0,1
.00022:
  ldo s0,[sp]
  ldo s1,8[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00023:
; if(*(p++) != 3)
  ldo t1,[s0]
  ldi t2,3
  beq t1,t2,.00025
; return 2;
  ldi a0,2
  bra .00022
.00025:
; p = &arr[1];
  add s0,s1,1
; if(*(p--) != 3)
  ldo t1,[s0]
  ldi t2,3
  beq t1,t2,.00027
; return 1;
  ldi a0,1
  bra .00022
.00027:
; if(*(p--) != 2)
  ldo t1,[s0]
  ldi t2,2
  beq t1,t2,.00029
; return 2;
  ldi a0,2
  bra .00022
.00029:
; p = &arr[0];
  add s0,s1,r0
; if(*(++p) != 3)
  add s0,s0,8
  ldo t1,[s0]
  ldi t2,3
  beq t1,t2,.00031
; return 1;
  ldi a0,1
  bra .00022
.00031:
; p = &arr[1];
  add s0,s1,1
; if(*(--p) != 2)
  sub s0,s0,8
  ldo t1,[s0]
  ldi t2,2
  beq t1,t2,.00033
; return 1;
  ldi a0,1
  bra .00022
.00033:
; return 0;
  mov a0,r0
  bra .00022
	.type	_main00032,@function
	.size	_main00032,$-_main00032


;--}

	.global	_main00032
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
