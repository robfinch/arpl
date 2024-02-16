  
	.text

;{++ _main00007

	.sdreg	29
_main00007:
; for(x = 10; x; x = x - 1)
  ldi t0,10
  beqz t0,.00020
.00019:
; ;
  sub t0,t0,1
  bnez t0,.00019
.00020:
; if (x) then
  beqz t0,.00022
; return 1;
  ldi a0,1
.00018:
  rts 
.00022:
; x = 10;
  ldi t0,10
; for (;x;)
  beqz t0,.00025
.00024:
; x = x - 1;
  sub t0,t0,1
  bnez t0,.00024
.00025:
; return x;
  mov a0,t0
  bra .00018
	.type	_main00007,@function
	.size	_main00007,$-_main00007


;--}

	.global	_main00007
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
