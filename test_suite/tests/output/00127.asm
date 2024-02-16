 	.bss
	.align 4
_c:
	.type	_c,@object
	.size	_c,8

 
	.text

;{++ _main00127

	.sdreg	29
_main00127:
; if(0) {
  ldi t0,0
  beqz t0,.00019
; return 1;
  ldi a0,1
.00018:
  rts 
.00019:
; } else if(0) {
  ldi t0,0
  beqz t0,.00021
; } else {
  bra .00022
.00021:
; if(1) {
  ldi t0,1
  beqz t0,.00023
; if(c)
  lda t0,00127.00000[gp]
  beqz t0,.00025
; return 1;
  ldi a0,1
  bra .00018
.00025:
; return 0;
  mov a0,r0
  bra .00018
  bra .00024
.00023:
; return 1;
  ldi a0,1
  bra .00018
.00024:
.00022:
.00020:
  bra .00018
	.type	_main00127,@function
	.size	_main00127,$-_main00127


;--}

	.global	_main00127
	.extern	_start_data
	.extern	_start_bss
	.global	_c
	.extern	_start_rodata
