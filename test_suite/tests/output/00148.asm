   	.bss
	.align 4
__noname_var0:
	.type	__noname_var0,@object
	.size	__noname_var0,16

 	.bss
	.align 4
_arr:
	.type	_arr,@object
	.size	_arr,32
	.8byte	1,2,3,4
 
	.text

;{++ _main00148

	.sdreg	29
_main00148:
; if(arr[0].a != 1)
  lda t1,_arr[gp]
  ldo t1,0[t1]
  ldi t2,1
  beq t1,t2,.00022
; return 1;
  ldi a0,1
.00021:
  rts 
.00022:
; if(arr[0].b != 2)
  lda t1,_arr[gp]
  ldo t1,8[t1]
  ldi t2,2
  beq t1,t2,.00024
; return 2;
  ldi a0,2
  bra .00021
.00024:
; if(arr[1].a != 3)
  lda t1,_arr[gp]
  ldo t1,0[t1]
  ldi t2,3
  beq t1,t2,.00026
; return 3;
  ldi a0,3
  bra .00021
.00026:
; if(arr[1].b != 4)
  lda t1,_arr[gp]
  ldo t1,8[t1]
  ldi t2,4
  beq t1,t2,.00028
; return 4;
  ldi a0,4
  bra .00021
.00028:
; return 0;
  mov a0,r0
  bra .00021
	.type	_main00148,@function
	.size	_main00148,$-_main00148


;--}

	.rodata
	.align	6

	.align	8





	.global	_main00148
	.extern	_start_data
	.global	__noname_var0
	.extern	_start_bss
	.global	_arr
	.extern	_start_rodata
