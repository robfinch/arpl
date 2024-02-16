   
	.text

;{++ _main00073

	.sdreg	29
_main00073:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,48
  sto s0,[sp]
  sto s1,8[sp]
  ldo t1,[fp]
  lda s1,-16[fp]
; p = &arr[1];
  add s0,s1,1
; p -= 1;
  sub s0,s0,8
; *p = 123;
  ldi t0,123
  sto t0,[s0]
; if(arr[0] != 123)
  ldo t1,0[s1]
  ldi t2,123
  beq t1,t2,.00013
; return 1;
  ldi a0,1
.00012:
  ldo s0,[sp]
  ldo s1,8[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00013:
; return 0;
  mov a0,r0
  bra .00012
	.type	_main00073,@function
	.size	_main00073,$-_main00073


;--}

	.global	_main00073
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
