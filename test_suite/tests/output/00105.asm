  
	.text

;{++ _main00105

	.sdreg	29
_main00105:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; for(i = 0; i < 10; i++)
  mov s0,r0
  ldi t1,10
  bge s0,t1,.00017
.00016:
; if (!i)
  bnez s0,.00019
.00019:
  iblt s0,t1,.00016
.00017:
; return 0;
  mov a0,r0
.00015:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00105,@function
	.size	_main00105,$-_main00105


;--}

	.global	_main00105
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
