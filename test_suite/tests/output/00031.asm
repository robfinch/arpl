 
	.text

;{++ _zero

	.sdreg	29
_zero:
  mov a0,r0
.00010:
  rts 
	.type	_zero,@function
	.size	_zero,$-_zero


;--}
 
;{++ _one

	.sdreg	29
_one:
  ldi a0,1
.00020:
  rts 
	.type	_one,@function
	.size	_one,$-_one


;--}
   
;{++ _main00031

	.sdreg	29
_main00031:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; x = zero();
  bsr _zero
; if (x != 1)
  ldi t1,1
  ibeq t0,t1,.00047
; return 1;
  ldi a0,1
.00046:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00047:
; if (y != 1)
  ldi t1,1
  beq t1,t1,.00049
; return 1;
  ldi a0,1
  bra .00046
.00049:
; x = one();
  bsr _one
  mov t0,a0
; y = --x;
  sub t0,t0,1
; y = --x;
  sub t1,t0,1
; if (x != 0)
  beq t0,r0,.00051
; return 1;
  bra .00046
.00051:
; if (y != 0)
  beq t1,r0,.00053
; return 1;
  bra .00046
.00053:
; x = zero();
  bsr _zero
; if (x != 1)
  ldi t1,1
  ibeq t0,t1,.00055
; return 1;
  ldi a0,1
  bra .00046
.00055:
; if (y != 0)
  beq t1,r0,.00057
; return 1;
  bra .00046
.00057:
; x = one();
  bsr _one
  mov t0,a0
  mov t1,a0
  sub t0,t0,1
; if (x != 0)
  beq t0,r0,.00059
; return 1;
  bra .00046
.00059:
; if (y != 1)
  ldi t1,1
  beq t1,t1,.00061
; return 1;
  ldi a0,1
  bra .00046
.00061:
; return 0;
  mov a0,r0
  bra .00046
	.type	_main00031,@function
	.size	_main00031,$-_main00031


;--}

	.global	_zero
	.global	_one
	.global	_main00031
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
