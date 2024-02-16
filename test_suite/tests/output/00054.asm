 	.bss
	.align 4
__noname_var0:
	.type	__noname_var0,@object
	.size	__noname_var0,2

  
	.text

;{++ _main00054

	.sdreg	29
_main00054:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; if(x != 0)
  ldi t0,0
  beqz t0,.00017
; return 1;
  ldi a0,1
.00016:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00017:
; if(y != 1)
  ldi t0,0
  beqz t0,.00019
; return 2;
  ldi a0,2
  bra .00016
.00019:
; if(z != 2)
  ldi t0,0
  beqz t0,.00021
; return 3;
  ldi a0,3
  bra .00016
.00021:
; e = x;
  mov s0,r0
  mov a0,r0
  bra .00016
	.type	_main00054,@function
	.size	_main00054,$-_main00054


;--}

	.global	_main00054
	.extern	_start_data
	.global	__noname_var0
	.extern	_start_bss
	.extern	_start_rodata
