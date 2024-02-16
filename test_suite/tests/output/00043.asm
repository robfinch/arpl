     	.bss
	.align 4
__noname_var0:
	.type	__noname_var0,@object
	.size	__noname_var0,24

  
	.text

;{++ _main00043

	.sdreg	29
_main00043:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
  lda s0,-24[fp]
; v.x = 1;
  ldi t0,1
  sto t0,0[s0]
; v.nest.y = 2;
  add t0,s0,8
  ldi t1,2
  sto t1,8[t0]
; v.nest.z = 3;
  add t0,s0,8
  ldi t1,3
  sto t1,16[t0]
; if (v.x + v.nest.y + v.nest.z != 6)
  ldo t3,0[s0]
  add t4,s0,8
  ldo t4,8[t4]
  add t2,t3,t4
  add t3,s0,8
  ldo t3,16[t3]
  add t1,t2,t3
  ldi t2,6
  beq t1,t2,.00013
; return 1;
  ldi a0,1
.00012:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00013:
; return 0;
  mov a0,r0
  bra .00012
	.type	_main00043,@function
	.size	_main00043,$-_main00043


;--}

	.global	_main00043
	.extern	_start_data
	.global	__noname_var0
	.extern	_start_bss
	.extern	_start_rodata
