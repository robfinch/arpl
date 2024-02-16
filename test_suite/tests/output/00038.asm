  
	.text

;{++ _main00038

	.sdreg	29
_main00038:
; if (sizeof(0) < 2)
  ldi t0,0
  beqz t0,.00021
; return 1;
  ldi a0,1
.00020:
  rts 
.00021:
; if (sizeof 0 < 2)
  ldi t0,0
  beqz t0,.00023
; return 1;
  ldi a0,1
  bra .00020
.00023:
; if (sizeof(char) < 1)
  ldi t0,0
  beqz t0,.00025
; return 1;
  ldi a0,1
  bra .00020
.00025:
; if (sizeof(int) - 2 < 0)
  ldi t0,0
  beqz t0,.00027
; return 1;
  ldi a0,1
  bra .00020
.00027:
; if (sizeof(&x) != sizeof p)
  ldi t0,0
  beqz t0,.00029
; return 1;
  ldi a0,1
  bra .00020
.00029:
; return 0;
  mov a0,r0
  bra .00020
	.type	_main00038,@function
	.size	_main00038,$-_main00038


;--}

	.global	_main00038
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
