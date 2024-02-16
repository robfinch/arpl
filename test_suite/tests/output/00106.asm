  	.bss
	.align 4
__noname_var0:
	.type	__noname_var0,@object
	.size	__noname_var0,8

  	.bss
	.align 4
__noname_var1:
	.type	__noname_var1,@object
	.size	__noname_var1,8

  
	.text

;{++ _main00106

	.sdreg	29
_main00106:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
; s2.s1.x = 1;
  lda t1,-8[fp]
  add t0,t1,r0
  ldi t1,1
  sto t1,0[t0]
; return 0;
  mov a0,r0
.00010:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00106,@function
	.size	_main00106,$-_main00106


;--}

	.global	_main00106
	.extern	_start_data
	.global	__noname_var0
	.global	__noname_var1
	.extern	_start_bss
	.extern	_start_rodata
