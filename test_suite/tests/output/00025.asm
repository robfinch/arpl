    
	.text

;{++ _main00025

	.sdreg	29
_main00025:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; p = "hello";
  lda t0,_main00025.00001[gp]
; return strlen(p) - 5;
  sto t0,-32[fp]
  sub sp,sp,8
  sto t0,0[sp]
  bsr _strlen
  sub a0,a0,5
.00011:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00025,@function
	.size	_main00025,$-_main00025


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00025.00001,@object
	.size	_main00025.00001,12
_main00025.00001: ; hello

	.2byte	104,101,108,108,111,0
	.global	_strlen
	.global	_main00025
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
