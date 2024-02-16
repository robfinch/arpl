  
	.text

;{++ _main00105

	.sdreg	29
_main00105:
; for(i = 0; i < 10; i++)
  mov t0,r0
  ldi t1,10
  bge t0,t1,.00017
; if (!i)
  bnez t0,.00019
.00019:
.00017:
; return 0;
  mov a0,r0
.00015:
  rts 
	.type	_main00105,@function
	.size	_main00105,$-_main00105


;--}

	.global	_main00105
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
