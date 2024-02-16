  
	.text

;{++ _main00102

	.sdreg	29
_main00102:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
; x = 1;
  ldi t0,1
; if ((x << 1) != 2)
  asl t1,t0,1
  sto t1,-32[fp]
  ldi t1,2
  beq t1,t1,.00013
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
	.type	_main00102,@function
	.size	_main00102,$-_main00102


;--}

	.global	_main00102
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
