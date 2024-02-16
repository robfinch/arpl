    
	.text

;{++ _main00130

	.sdreg	29
_main00130:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,64
  sto s0,[sp]
  sto s1,8[sp]
  sto s2,16[sp]
  sto s3,24[sp]
  lda s0,-16[fp]
  lda s1,-64[fp]
; p = arr;
  mov s3,s0
; q = &arr[1][3];
  add t1,s0,1
  add s2,t1,r0
; arr[1][3] = 2;
  add t0,s0,1
  ldi t1,2
  stw t1,0[t0]
; v[0] = 2;
  ldi t0,2
  sto t0,0[s1]
; if (arr[1][3] != 2)
  add t1,s0,1
  ldw t1,0[t1]
  ldi t2,2
  beq t1,t2,.00019
; return 1;
  ldi a0,1
.00018:
  ldo s0,[sp]
  ldo s1,8[sp]
  ldo s2,16[sp]
  ldo s3,24[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00019:
; if (p[1][3] != 2)
  ldo t1,1[s3]
  ldo t2,1[s3]
  extu t3,t2,3,3
  ldi t4,2
  beq t3,t4,.00021
; return 1;
  ldi a0,1
  bra .00018
.00021:
; if (*q != 2)
  ldw t1,[s2]
  ldi t2,2
  beq t1,t2,.00023
; return 1;
  ldi a0,1
  bra .00018
.00023:
; if (*v != 2)
  ldo t1,[s1]
  ldi t2,2
  beq t1,t2,.00025
; return 1;
  ldi a0,1
  bra .00018
.00025:
; return 0;
  mov a0,r0
  bra .00018
	.type	_main00130,@function
	.size	_main00130,$-_main00130


;--}

	.global	_main00130
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
