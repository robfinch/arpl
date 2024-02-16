  
	.text

;{++ _main00035

	.sdreg	29
_main00035:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; x = 4;
  ldi s0,4
; if(!x != 0)
  not t1,s0
  beq t1,r0,.00017
; return 1;
  ldi a0,1
.00016:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00017:
; if(!!x != 1)
  not t2,s0
  not t1,t2
  ldi t2,1
  beq t1,t2,.00019
; return 1;
  ldi a0,1
  bra .00016
.00019:
; if(-x != 0 - 4)
  neg t1,s0
  ldi t2,16777212
  beq t1,t2,.00021
; return 1;
  ldi a0,1
  bra .00016
.00021:
; return 0;
  mov a0,r0
  bra .00016
	.type	_main00035,@function
	.size	_main00035,$-_main00035


;--}

	.global	_main00035
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
