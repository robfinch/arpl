   	.bss
	.align 4
__noname_var0:
	.type	__noname_var0,@object
	.size	__noname_var0,16

    	.bss
	.align 4
__noname_var1:
	.type	__noname_var1,@object
	.size	__noname_var1,40

 	.bss
	.align 4
_gs1:
	.type	_gs1,@object
	.size	_gs1,16
	.8byte	1,2
 	.bss
	.align 4
_s:
	.type	_s,@object
	.size	_s,8
	.8byte	_s_data
_s_data:
,1,2
	.8byte	00150.00056
	.8byte	0

	.8byte	1,2
 
	.text

;{++ _main00150

	.sdreg	29
_main00150:
; if(s->s1.a != 1)
  lda t1,00150.00072[gp]
  ldo t1,[t1]
  ldi t2,1
  beq t1,t2,.00030
; return 1;
  ldi a0,1
.00029:
  rts 
.00030:
; if(s->s1.b != 2)
  lda t1,00150.00072[gp]
  ldou t1,128[t1]
  ldi t2,2
  beq t1,t2,.00032
; return 2;
  ldi a0,2
  bra .00029
.00032:
; if(s->ps1->a != 1)
  lda t1,00150.00072[gp]
  ldou t1,640[t1]
  ldo t1,[t1]
  ldi t2,1
  beq t1,t2,.00034
; return 3;
  ldi a0,3
  bra .00029
.00034:
; if(s->ps1->b != 2)
  lda t1,00150.00072[gp]
  ldo t1,640[t1]
  ldou t1,8[t1]
  ldi t2,2
  beq t1,t2,.00036
; return 4;
  ldi a0,4
  bra .00029
.00036:
; if(s->arr[0] != 1)
  lda t2,00150.00072[gp]
  add t1,t2,24
  ldo t1,0[t1]
  ldi t2,1
  beq t1,t2,.00038
; return 5;
  ldi a0,5
  bra .00029
.00038:
; if(s->arr[1] != 2)
  lda t2,00150.00072[gp]
  add t1,t2,24
  ldo t1,1[t1]
  ldi t2,2
  beq t1,t2,.00040
; return 6;
  ldi a0,6
  bra .00029
.00040:
; return 0;
  mov a0,r0
  bra .00029
	.type	_main00150,@function
	.size	_main00150,$-_main00150


;--}

	.rodata
	.align	6

	.align	8









	.global	_main00150
	.extern	_start_data
	.global	__noname_var0
	.global	__noname_var1
	.extern	_start_bss
	.global	_gs1
	.global	_s
	.extern	_start_rodata
