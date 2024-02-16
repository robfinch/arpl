  
	.text

;{++ _f1

	.sdreg	29
_f1:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
  ldo t1,32[fp]
  add a0,t1,1
.00010:
  mov sp,fp
  ldo fp,[sp]
  rtd 40,0
	.type	_f1,@function
	.size	_f1,$-_f1


;--}
     
;{++ _main00078

	.sdreg	29
_main00078:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; char s = 1;
  ldi t0,1
  stw t0,-2[fp]
; if (f1(&s) != 2)
  sto t0,-32[fp]
  sub sp,sp,8
  lda t1,-2[fp]
  sto t1,0[sp]
  bsr _f1
  ldi t1,2
  beq a0,t1,.00023
; return 1;
  ldi a0,1
.00022:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00023:
; return 0;
  mov a0,r0
  bra .00022
	.type	_main00078,@function
	.size	_main00078,$-_main00078


;--}

	.global	_main00078
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
