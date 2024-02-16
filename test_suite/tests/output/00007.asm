  
	.text

;{++ _main00007

	.sdreg	29
_main00007:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; for(x = 10; x; x = x - 1)
  ldi s0,10
  beqz s0,.00020
.00019:
; ;
  sub s0,s0,1
  bnez s0,.00019
.00020:
; if (x) then
  beqz s0,.00022
; return 1;
  ldi a0,1
.00018:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00022:
; x = 10;
  ldi s0,10
; for (;x;)
  beqz s0,.00025
.00024:
; x = x - 1;
  sub s0,s0,1
.00026:
  bnez s0,.00024
.00025:
; return x;
  mov a0,s0
  bra .00018
	.type	_main00007,@function
	.size	_main00007,$-_main00007


;--}

	.global	_main00007
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
