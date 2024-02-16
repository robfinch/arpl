     
	.text

;{++ _main00041

	.sdreg	29
_main00041:
; c = 0;
  mov t2,r0
; while (n < 5000) {
  ldi t1,5000
  bge t1,t1,.00022
.00021:
; t = 2;
  ldi t0,2
; p = 1;
  ldi t3,1
; while (t*t <= n) {
  mul t1,t0,t0
  bgt t1,t1,.00024
.00023:
; if (n % t == 0)
  div t2,t1,t0
  mul t2,t2,t0
  sub t1,t1,t2
  bnez t1,.00025
; p = 0;
  mov t3,r0
.00025:
; t++;
  add t0,t0,1
  mul t1,t0,t0
  ble t1,t1,.00023
.00024:
; n++;
  add t1,t1,1
; if (p)
  beqz t3,.00027
; c++;
  add t2,t2,1
.00027:
  ldi t1,5000
  blt t1,t1,.00021
.00022:
; if (c != 669)
  ldi t1,669
  beq t2,t1,.00029
; return 1;
  ldi a0,1
.00020:
  rts 
.00029:
; return 0;
  mov a0,r0
  bra .00020
	.type	_main00041,@function
	.size	_main00041,$-_main00041


;--}

	.global	_main00041
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
