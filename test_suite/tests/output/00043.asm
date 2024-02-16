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
  sub sp,sp,32
; v.x = 1;
  ldi t0,1
  sto t0,0[t0]
; v.nest.y = 2;
  ldi t0,2
  sto t0,8[t0]
; v.nest.z = 3;
  ldi t0,3
  sto t0,16[t0]
; if (v.x + v.nest.y + v.nest.z != 6)
  sto t2,-32[fp]
  add t2,t0,8
  ldo t2,16[t2]
  add t1,t2,t2
  ldi t2,6
  beq t1,t2,.00013
; return 1;
  ldi a0,1
.00012:
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
