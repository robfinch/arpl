     
	.text

;{++ _main00041

	.sdreg	29
_main00041:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,64
  sto s0,[sp]
  sto s1,8[sp]
  sto s2,16[sp]
  sto s3,24[sp]
; c = 0;
  mov s2,r0
; n = 2;
  ldi s1,2
; while (n < 5000) {
  ldi t1,5000
  bge s1,t1,.00022
.00021:
; t = 2;
  ldi s0,2
; p = 1;
  ldi s3,1
; while (t*t <= n) {
  mul t1,s0,s0
  bgt t1,s1,.00024
.00023:
; if (n % t == 0)
  div t2,s1,s0
  mul t2,t2,s0
  sub t1,s1,t2
  bnez t1,.00025
; p = 0;
  mov s3,r0
.00025:
; t++;
  add s0,s0,1
  mul t1,s0,s0
  ble t1,s1,.00023
.00024:
; n++;
  add s1,s1,1
; if (p)
  beqz s3,.00027
; c++;
  add s2,s2,1
.00027:
  ldi t1,5000
  blt s1,t1,.00021
.00022:
; if (c != 669)
  ldi t1,669
  beq s2,t1,.00029
; return 1;
  ldi a0,1
.00020:
  ldo s0,[sp]
  ldo s1,8[sp]
  ldo s2,16[sp]
  ldo s3,24[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00029:
; return 0;
  mov a0,r0
  bra .00020
	.type	_main00041,@function
	.size	_main00041,$-_main00041


;--}

	.global	_main00041
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
