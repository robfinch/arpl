 
	.text

;{++ _f

	.sdreg	29
_f:
  ldi a0,100
.00010:
  rts 
	.type	_f,@function
	.size	_f,$-_f


;--}
 
;{++ _main00030

	.sdreg	29
_main00030:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; if (f() > 1000)
  sto t0,-32[fp]
  bsr _f
  ldo t0,-32[fp]
  ldi t1,1000
  ble a0,t1,.00033
; return 1;
  ldi a0,1
.00032:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00033:
; if (f() >= 1000)
  sto t0,-32[fp]
  bsr _f
  ldo t0,-32[fp]
  ldi t1,1000
  blt a0,t1,.00035
; return 1;
  ldi a0,1
  bra .00032
.00035:
; if (1000 < f())
  ldi t1,1000
  sto t0,-32[fp]
  sto t1,-24[fp]
  bsr _f
  ldo t1,-24[fp]
  ldo t0,-32[fp]
  bge t1,a0,.00037
; return 1;
  ldi a0,1
  bra .00032
.00037:
; if (1000 <= f())
  ldi t1,1000
  sto t0,-32[fp]
  sto t1,-24[fp]
  bsr _f
  ldo t1,-24[fp]
  ldo t0,-32[fp]
  bgt t1,a0,.00039
; return 1;
  ldi a0,1
  bra .00032
.00039:
; if (1000 == f())
  sto t0,-32[fp]
  bsr _f
  ldo t0,-32[fp]
  ldi t1,1000
  bne a0,t1,.00041
; return 1;
  ldi a0,1
  bra .00032
.00041:
; if (100 != f())
  sto t0,-32[fp]
  bsr _f
  ldi t1,100
  beq a0,t1,.00043
; return 1;
  ldi a0,1
  bra .00032
.00043:
; return 0;
  mov a0,r0
  bra .00032
	.type	_main00030,@function
	.size	_main00030,$-_main00030


;--}

	.global	_f
	.global	_main00030
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
