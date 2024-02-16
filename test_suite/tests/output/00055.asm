 	.bss
	.align 4
__noname_var0:
	.type	__noname_var0,@object
	.size	__noname_var0,2

  
	.text

;{++ _main00055

	.sdreg	29
_main00055:
; if(x != 0)
  ldi t0,0
  beqz t0,.00017
; return 1;
  ldi a0,1
.00016:
  rts 
.00017:
; if(y != 2)
  ldi t0,0
  beqz t0,.00019
; return 2;
  ldi a0,2
  bra .00016
.00019:
; if(z != 3)
  ldi t0,1
  beqz t0,.00021
; return 3;
  ldi a0,3
  bra .00016
.00021:
; e = x;
  mov a0,r0
  bra .00016
	.type	_main00055,@function
	.size	_main00055,$-_main00055


;--}

	.global	_main00055
	.extern	_start_data
	.global	__noname_var0
	.extern	_start_bss
	.extern	_start_rodata
