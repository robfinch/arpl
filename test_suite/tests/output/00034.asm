  
	.text

;{++ _main00034

	.sdreg	29
_main00034:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
; x = 0;
  mov s0,r0
; while(1)
  ldi t0,1
  beqz t0,.00027
.00026:
.00027:
; while(1) {
  beqz t0,.00029
.00028:
; if (x == 5) {
  ldi t1,5
  bne s0,t1,.00030
; break;
  bra .00029
.00030:
; x = x + 1;
  add s0,s0,1
; continue;
  bra .00028
.00029:
.00032:
; if (x == 10) {
  ldi t1,10
  bne s0,t1,.00035
; break;
  bra .00033
.00035:
; x = x + 1;
  add s0,s0,1
.00033:
.00037:
; if (x == 15) {
  ldi t1,15
  bne s0,t1,.00039
; break;
  bra .00038
.00039:
; x = x + 1;
  add s0,s0,1
; continue;
  bra .00037
.00038:
; return x - 15;
  sub a0,s0,15
.00025:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00034,@function
	.size	_main00034,$-_main00034


;--}

	.global	_main00034
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
