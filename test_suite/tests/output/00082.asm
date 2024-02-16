  
	.text

;{++ _main00082

	.sdreg	29
_main00082:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; if (x != 1)
  ldi t1,1
  ibeq s0,t1,.00013
; return 1;
  ldi a0,1
.00012:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00013:
; return 0;
  mov a0,r0
  bra .00012
	.type	_main00082,@function
	.size	_main00082,$-_main00082


;--}

	.global	_main00082
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
