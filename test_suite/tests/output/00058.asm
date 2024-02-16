  
	.text

;{++ _main00058

	.sdreg	29
_main00058:
; s = "abc" "def";
  lda t0,_main00058.00001[gp]
; if(s[0] != 'a') return 1;
  ldw t0,0[t0]
  ldi t1,97
  beq t0,t1,.00026
  ldi a0,1
.00025:
  rts 
.00026:
; if(s[1] != 'b') return 2;
  ldw t0,1[t0]
  ldi t1,98
  beq t0,t1,.00028
  ldi a0,2
  bra .00025
.00028:
; if(s[2] != 'c') return 3;
  ldw t0,2[t0]
  ldi t1,99
  beq t0,t1,.00030
  ldi a0,3
  bra .00025
.00030:
; if(s[3] != 'd') return 4;
  ldw t0,3[t0]
  ldi t1,100
  beq t0,t1,.00032
  ldi a0,4
  bra .00025
.00032:
; if(s[4] != 'e') return 5;
  ldw t0,4[t0]
  ldi t1,101
  beq t0,t1,.00034
  ldi a0,5
  bra .00025
.00034:
; if(s[5] != 'f') return 6;
  ldw t0,5[t0]
  ldi t1,102
  beq t0,t1,.00036
  ldi a0,6
  bra .00025
.00036:
; if(s[6] != 0) return 7;
  ldw t0,6[t0]
  beq t0,r0,.00038
  ldi a0,7
  bra .00025
.00038:
; return 0;
  mov a0,r0
  bra .00025
	.type	_main00058,@function
	.size	_main00058,$-_main00058


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00058.00001,@object
	.size	_main00058.00001,16
_main00058.00001: ; abcWdef

	.2byte	97,98,99,87,100,101,102,0
	.global	_main00058
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
