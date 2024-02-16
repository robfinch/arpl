   
	.text

;{++ _main00079

	.sdreg	29
_main00079:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,48
  sto s0,[sp]
  sto s1,8[sp]
; y = 0;
  mov s1,r0
; x = ((y) + 1);
  add s0,s1,1
; if(x != 1)
  ldi t1,1
  beq s0,t1,.00013
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
	.type	_main00079,@function
	.size	_main00079,$-_main00079


;--}

	.global	_main00079
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
