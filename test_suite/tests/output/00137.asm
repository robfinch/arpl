   
	.text

;{++ _main00137

	.sdreg	29
_main00137:
; p = "hello"  " is better than bye";
  lda t1,_main00137.00001[gp]
; return (*p == 'h') ? 0 : 1;
  ldw t1,[t1]
  ldi t2,104
  bne t1,t2,.00014
  ldi t1,0
  bra .00015
.00014:
  ldi t1,1
.00015:
  mov a0,t1
.00013:
  rts 
	.type	_main00137,@function
	.size	_main00137,$-_main00137


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00137.00001,@object
	.size	_main00137.00001,52
_main00137.00001: ; helloW is better than bye

	.2byte	104,101,108,108,111,87,32,105
	.2byte	115,32,98,101,116,116,101,114
	.2byte	32,116,104,97,110,32,98,121
	.2byte	101,0
	.global	_main00137
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
