    
	.text

;{++ _main00042

	.sdreg	29
_main00042:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; u.b = 3;
  ldi s0,3
; if (u.a != 3 || u.b != 3)
  ldi t1,3
  bne s0,t1,.00016
  ldi t2,3
  beq s0,t2,.00014
.00016:
; return 1;
  ldi a0,1
.00013:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00014:
; return 0;
  mov a0,r0
  bra .00013
	.type	_main00042,@function
	.size	_main00042,$-_main00042


;--}

	.global	_main00042
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
