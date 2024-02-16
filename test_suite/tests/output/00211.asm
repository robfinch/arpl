    	.bss
	.align 4
_array:
	.type	_array,@object
	.size	_array,248

 
	.text

;{++ _main00211

	.sdreg	29
_main00211:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  sub sp,sp,16
  lda t0,_main00211.00001[gp]
  sto t0,0[sp]
  ldi t0,15
  sto t0,8[sp]
  bsr _printf
.00011:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00211,@function
	.size	_main00211,$-_main00211


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00211.00001,@object
	.size	_main00211.00001,20
_main00211.00001: ; n+1 = %d

	.2byte	110,43,49,32,61,32,37,100
	.2byte	10,0
	.extern	_printf
	.global	_main00211
	.extern	_start_data
	.extern	_start_bss
	.global	_array
	.extern	_start_rodata
