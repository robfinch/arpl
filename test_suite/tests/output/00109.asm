  
	.text
 
;{++ _main00109

	.sdreg	29
_main00109:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,48
  sto s0,[sp]
  sto s1,8[sp]
; int x = 0;
  mov s1,r0
  ldi s0,1
; if(x ? 1 : 0)
  beqz s1,.00021
  ldi t0,1
  bra .00022
.00021:
  ldi t0,0
.00022:
  beqz t0,.00019
; return 1;
  ldi a0,1
.00018:
  ldo s0,[sp]
  ldo s1,8[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00019:
; if(y ? 0 : 1)
  beqz s0,.00025
  ldi t0,0
  bra .00026
.00025:
  ldi t0,1
.00026:
  beqz t0,.00023
; return 2;
  ldi a0,2
  bra .00018
.00023:
; return 0;
  mov a0,r0
  bra .00018
	.type	_main00109,@function
	.size	_main00109,$-_main00109


;--}

	.global	_main00109
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
