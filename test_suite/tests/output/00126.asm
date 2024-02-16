  
	.text

;{++ _main00126

	.sdreg	29
_main00126:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; x = 3;
  ldi s0,3
; x = !x;
  not s0,s0
  not s0,s0
; x = ~x;
  com s0,s0
; x = -x;
  neg s0,s0
; if(x != 2)
  ldi t1,2
  beq s0,t1,.00013
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
	.type	_main00126,@function
	.size	_main00126,$-_main00126


;--}

	.global	_main00126
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
