    
	.text

;{++ _main00130

	.sdreg	29
_main00130:
.00018:
  rts 
; if (p[1][3] != 2)
  ldo t1,1[t3]
  ldo t2,1[t3]
  extu t3,t2,3,3
  ldi t4,2
  beq t3,t4,.00021
; return 1;
  ldi a0,1
  bra .00018
.00021:
; if (*q != 2)
  ldw t1,[t2]
  ldi t2,2
  beq t1,t2,.00023
; return 1;
  ldi a0,1
  bra .00018
.00023:
; if (*v != 2)
  ldo t1,[t1]
  ldi t2,2
  beq t1,t2,.00025
; return 1;
  ldi a0,1
  bra .00018
.00025:
; return 0;
  mov a0,r0
  bra .00018
	.type	_main00130,@function
	.size	_main00130,$-_main00130


;--}

	.global	_main00130
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
