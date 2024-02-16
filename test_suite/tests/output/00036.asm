  
	.text

;{++ _main00036

	.sdreg	29
_main00036:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; x = 0;
  mov s0,r0
; x += 2;
  add s0,s0,2
  add s0,s0,2
; if (x != 4)
  ldi t1,4
  beq s0,t1,.00017
; return 1;
  ldi a0,1
.00016:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00017:
; x -= 1;
  sub s0,s0,1
; if (x != 3)
  ldi t1,3
  beq s0,t1,.00019
; return 2;
  ldi a0,2
  bra .00016
.00019:
; x *= 2;
  mul s0,s0,2
; if (x != 6)
  ldi t1,6
  beq s0,t1,.00021
; return 3;
  ldi a0,3
  bra .00016
.00021:
; return 0;
  mov a0,r0
  bra .00016
	.type	_main00036,@function
	.size	_main00036,$-_main00036


;--}

	.global	_main00036
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
