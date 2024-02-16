   
	.text

;{++ _f2

	.sdreg	29
_f2:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
  ldo t1,32[fp]
  ldo t2,40[fp]
  sub a0,t1,t2
.00010:
  mov sp,fp
  ldo fp,[sp]
  rtd 48,0
	.type	_f2,@function
	.size	_f2,$-_f2


;--}
      
;{++ _f1

	.sdreg	29
_f1:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
; if (a != b)
  lda t1,_f1.09999[r56]
  ldo t2,24[fp]
  beq t1,t2,.00023
; return f2;
  ldi a0,_f2
.00022:
  mov sp,fp
  ldo fp,[sp]
  rtd 48,0
.00023:
; return 0;
  mov a0,r0
  bra .00022
	.type	_f1,@function
	.size	_f1,$-_f1


;--}
        
;{++ _main00124

	.sdreg	29
_main00124:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; return (*(*p)(0, 2))(2, 2);
  sub sp,sp,16
  ldi t0,2
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _p
  add sp,sp,16
.00034:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00124,@function
	.size	_main00124,$-_main00124


;--}

	.extern	_p
	.global	_f1
	.global	_f2
	.global	_main00124
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
