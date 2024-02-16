    
	.text

;{++ _main00005

	.sdreg	29
_main00005:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,56
  sto s0,[sp]
  sto s1,8[sp]
  sto s2,16[sp]
  lda s0,-16[fp]
  lda t0,-8[fp]
  lda s1,-8[fp]
; x = 0;
  sto r0,[s1]
; p = &x;
  sto s1,[s0]
; pp = &p;
  mov s2,s0
; if (*p) then
  ldo t0,[s0]
  beqz t0,.00017
; return 1;
  ldi a0,1
.00016:
  ldo s0,[sp]
  ldo s1,8[sp]
  ldo s2,16[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00017:
; if (**pp) then
  ldou t0,[s2]
  beqz t0,.00019
; return 1;
  bra .00016
.00019:
; **pp = 1;
  ldi t0,1
.00020:
; if (x) then
  ldo t0,[s1]
  beqz t0,.00021
; return 0;
  mov a0,r0
  bra .00016
.00021:
; return 1;
  ldi a0,1
  bra .00016
.00022:
  bra .00016
	.type	_main00005,@function
	.size	_main00005,$-_main00005


;--}

	.global	_main00005
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
