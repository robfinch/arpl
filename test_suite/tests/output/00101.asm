  
	.text

;{++ _main00101

	.sdreg	29
_main00101:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; c = 0;
  mov s0,r0
.00013:
;while (0);
  ldi t0,0
  bnez t0,.00013
; return c;
  mov a0,s0
.00012:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00101,@function
	.size	_main00101,$-_main00101


;--}

	.global	_main00101
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
