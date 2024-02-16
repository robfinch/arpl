   
	.text

;{++ _main00032

	.sdreg	29
_main00032:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
  lda t1,-16[fp]
; arr[0] = 2;
  ldi t0,2
  sto t0,0[t1]
; arr[1] = 3;
  ldi t0,3
  sto t0,8[t1]
; p = &arr[0];
  add t0,t1,r0
; if(*(p++) != 2)
  ldo t0,[t0]
  ldi t1,2
  beq t0,t1,.00023
; return 1;
  ldi a0,1
.00022:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00023:
; if(*(p++) != 3)
  ldo t0,[t0]
  ldi t1,3
  beq t0,t1,.00025
; return 2;
  ldi a0,2
  bra .00022
.00025:
; p = &arr[1];
  add t0,t1,1
; if(*(p--) != 3)
  ldo t0,[t0]
  ldi t1,3
  beq t0,t1,.00027
; return 1;
  ldi a0,1
  bra .00022
.00027:
; if(*(p--) != 2)
  ldo t0,[t0]
  ldi t1,2
  beq t0,t1,.00029
; return 2;
  ldi a0,2
  bra .00022
.00029:
; p = &arr[0];
  add t0,t1,r0
; if(*(++p) != 3)
  add t0,t0,8
  ldo t1,[t0]
  sto t1,-32[fp]
  ldi t1,3
  beq t1,t1,.00031
; return 1;
  ldi a0,1
  bra .00022
.00031:
; p = &arr[1];
  add t0,t1,1
; if(*(--p) != 2)
  sub t0,t0,8
  ldo t1,[t0]
  sto t1,-32[fp]
  ldi t1,2
  beq t1,t1,.00033
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
