      	.bss
	.align 4
_s2:
	.type	_s2,@object
	.size	_s2,16

  
	.text
        
;{++ _main00129

	.sdreg	29
_main00129:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; goto s;
  bra .00001
.00012:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00002:
; return 0;
  mov a0,r0
  bra .00012
	.type	_main00129,@function
	.size	_main00129,$-_main00129


;--}

	.global	_main00129
	.extern	_start_data
	.global	_s2
	.extern	_start_bss
	.extern	_start_rodata
