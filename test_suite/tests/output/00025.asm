    
	.text

;{++ _main00025

	.sdreg	29
_main00025:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
; p = "hello";
  lda t0,_main00025.00001[gp]
; p = "hello";
  lda s0,_main00025.00001[gp]
; return strlen(p) - 5;
  sto t0,-40[fp]
  sub sp,sp,8
  sto s0,0[sp]
  bsr _strlen
  sub a0,a0,5
.00011:
  ldo s0,[sp]
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
