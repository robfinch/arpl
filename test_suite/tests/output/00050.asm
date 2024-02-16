   	.bss
	.align 4
__noname_var0:
	.type	__noname_var0,@object
	.size	__noname_var0,16

       	.bss
	.align 4
__noname_var2:
	.type	__noname_var2,@object
	.size	__noname_var2,40

 	.bss
	.align 4
_v:
	.type	_v,@object
	.size	_v,40
	.8byte	1,2

	.8byte	4,5
 
	.text

;{++ _main00050

	.sdreg	29
_main00050:
; if(v.a != 1)
  lda t1,00050.00000[gp]
  ldi t2,1
  beq t1,t2,.00026
; return 1;
  ldi a0,1
.00025:
  rts 
.00026:
; if(v.b != 2)
  lda t1,00050.00056[gp]
  ldou t1,8[t1]
  ldi t2,2
  beq t1,t2,.00028
; return 2;
  ldi a0,2
  bra .00025
.00028:
; if(v.c != 3 || v.d != 3)
  lda t1,00050.00056[gp]
  ldou t1,16[t1]
  ldi t2,3
  bne t1,t2,.00032
  lda t2,00050.00056[gp]
  ldou t2,16[t2]
  ldi t3,3
  beq t2,t3,.00030
.00032:
; return 3;
  ldi a0,3
  bra .00025
.00030:
; if(v.s.a != 4)
  lda t2,00050.00056[gp]
  add t1,t2,24
  ldou t1,24[t1]
  ldi t2,4
  beq t1,t2,.00033
; return 4;
  ldi a0,4
  bra .00025
.00033:
; if(v.s.b != 5)
  lda t2,00050.00056[gp]
  add t1,t2,24
  ldou t1,32[t1]
  ldi t2,5
  beq t1,t2,.00035
; return 5;
  ldi a0,5
  bra .00025
.00035:
; return 0;
  mov a0,r0
  bra .00025
	.type	_main00050,@function
	.size	_main00050,$-_main00050


;--}

	.rodata
	.align	6

	.align	8






	.global	_main00050
	.extern	_start_data
	.global	__noname_var0
	.global	__noname_var2
	.extern	_start_bss
	.global	_v
	.extern	_start_rodata
