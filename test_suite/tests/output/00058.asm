  
	.text

;{++ _main00058

	.sdreg	29
_main00058:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; s = "abc" "def";
  lda s0,_main00058.00001[gp]
; if(s[0] != 'a') return 1;
  ldw t1,0[s0]
  ldi t2,97
  beq t1,t2,.00026
  ldi a0,1
.00025:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00026:
; if(s[1] != 'b') return 2;
  ldw t1,2[s0]
  ldi t2,98
  beq t1,t2,.00028
  ldi a0,2
  bra .00025
.00028:
; if(s[2] != 'c') return 3;
  ldw t1,4[s0]
  ldi t2,99
  beq t1,t2,.00030
  ldi a0,3
  bra .00025
.00030:
; if(s[3] != 'd') return 4;
  ldw t1,6[s0]
  ldi t2,100
  beq t1,t2,.00032
  ldi a0,4
  bra .00025
.00032:
; if(s[4] != 'e') return 5;
  ldw t1,8[s0]
  ldi t2,101
  beq t1,t2,.00034
  ldi a0,5
  bra .00025
.00034:
; if(s[5] != 'f') return 6;
  ldw t1,10[s0]
  ldi t2,102
  beq t1,t2,.00036
  ldi a0,6
  bra .00025
.00036:
; if(s[6] != 0) return 7;
  ldw t1,12[s0]
  beq t1,r0,.00038
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
