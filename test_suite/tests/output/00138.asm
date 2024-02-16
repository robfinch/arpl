   
	.text

;{++ _main00138

	.sdreg	29
_main00138:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
; char *a = "hi";
  lda t0,_main00138.00001[gp]
  sto t0,-8[fp]
; return (a[1] == 'i') ? 0 : 1;
  ldo t1,1[fp]
  ldw t1,1[t1]
  ldi t2,105
  bne t1,t2,.00014
  ldi t1,0
  bra .00015
.00014:
  ldi t1,1
.00015:
  mov a0,t1
.00013:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00138,@function
	.size	_main00138,$-_main00138


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00138.00001,@object
	.size	_main00138.00001,6
_main00138.00001: ; hi

	.2byte	104,105,0
	.global	_main00138
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
