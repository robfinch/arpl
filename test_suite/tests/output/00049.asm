 	.bss
	.align 4
_x:
	.type	_x,@object
	.size	_x,8
	.8byte	10
   	.bss
	.align 4
__noname_var0:
	.type	__noname_var0,@object
	.size	__noname_var0,16

 	.bss
	.align 4
_s:
	.type	_s,@object
	.size	_s,16
	.8byte	1
	.8byte	_x
 
	.text

;{++ _main00049

	.sdreg	29
_main00049:
; if(s.a != 1)
  lda t1,_s[gp]
  ldo t1,0[t1]
  ldi t2,1
  beq t1,t2,.00016
; return 1;
  ldi a0,1
.00015:
  rts 
.00016:
; if(*s.p != 10)
  lda t1,_s[gp]
  ldo t1,8[t1]
  ldo t1,[t1]
  ldi t2,10
  beq t1,t2,.00018
; return 2;
  ldi a0,2
  bra .00015
.00018:
; return 0;
  mov a0,r0
  bra .00015
	.type	_main00049,@function
	.size	_main00049,$-_main00049


;--}

	.rodata
	.align	6

	.align	8



	.global	_main00049
	.extern	_start_data
	.global	__noname_var0
	.extern	_start_bss
	.global	_s
	.global	_x
	.extern	_start_rodata
