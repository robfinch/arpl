 	.bss
	.align 4
_s:
	.type	_s,@object
	.size	_s,2
	.2byte	0,0,0,0,0,0
 
	.text

;{++ _main00115

	.sdreg	29
_main00115:
; if (s[0] != 'a')
  lda t1,00115.00001[gp]
  ldw t1,0[t1]
  ldi t2,97
  beq t1,t2,.00020
; return 1;
  ldi a0,1
.00019:
  rts 
.00020:
; if (s[1] != 'b')
  lda t1,00115.00001[gp]
  ldw t1,1[t1]
  ldi t2,98
  beq t1,t2,.00022
; return 2;
  ldi a0,2
  bra .00019
.00022:
; if (s[2] != 'c')
  lda t1,00115.00001[gp]
  ldw t1,2[t1]
  ldi t2,99
  beq t1,t2,.00024
; return 3;
  ldi a0,3
  bra .00019
.00024:
; if (s[3] != '\0')
  lda t1,00115.00001[gp]
  ldw t1,3[t1]
  beq t1,r0,.00026
; return 4;
  ldi a0,4
  bra .00019
.00026:
; return 0;
  mov a0,r0
  bra .00019
	.type	_main00115,@function
	.size	_main00115,$-_main00115


;--}

	.rodata
	.align	6

	.align	8


	.global	_main00115
	.extern	_start_data
	.extern	_start_bss
	.global	_s
	.extern	_start_rodata
