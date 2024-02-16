  
	.text

;{++ _main00015

	.sdreg	29
_main00015:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
  ldo t1,[fp]
; arr[0] = 1;
  ldi t0,1
  sto t0,0[t0]
; arr[1] = 2;
  ldi t0,2
  sto t0,8[t0]
; return arr[0] + arr[1] - 3;
  sto t1,-32[fp]
  ldo t1,0[t0]
  ldo t2,8[t0]
  add t1,t1,t2
  sub a0,t1,3
.00010:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00015,@function
	.size	_main00015,$-_main00015


;--}

	.global	_main00015
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
