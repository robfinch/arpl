                               
	.text

;{++ _fooc

	.sdreg	29
_fooc:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
  ldo t0,24[fp]
  ldi t1,42
  sto t1,3[t0]
.00010:
  mov sp,fp
  ldo fp,[sp]
  rtd 72,0
	.type	_fooc,@function
	.size	_fooc,$-_fooc


;--}
     
;{++ _main00162

	.sdreg	29
_main00162:
  mov a0,r0
.00020:
  rts 
	.type	_main00162,@function
	.size	_main00162,$-_main00162


;--}

	.global	_fooc
	.global	_foor
	.global	_foos
	.global	_foov
	.global	_foovm
	.global	_foo
	.global	_main00162
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
