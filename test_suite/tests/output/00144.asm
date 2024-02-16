    
	.text

;{++ _main00144

	.sdreg	29
_main00144:
; i = i ? 0 : 0l;
  beqz t0,.00027
  ldi t1,0
  ldi t0,0
.00027:
.00028:
; p = i ? (void *) 0 : 0;
  beqz t0,.00029
.00029:
  mov t2,t0
; p = i ? 0 : (void *) 0;
  beqz t0,.00031
.00031:
  mov t2,t0
; p = i ? 0 : (const void *) 0;
  beqz t0,.00033
.00033:
  mov t2,t0
; q = i ? 0 : p;
  beqz t0,.00035
  bra .00036
.00035:
  mov t0,t2
.00036:
  mov t1,t0
; q = i ? p : 0;
  beqz t0,.00037
  mov t0,t2
  bra .00038
.00037:
  ldi t0,0
.00038:
  mov t1,t0
; q = i ? q : 0;
  beqz t0,.00039
  mov t0,t1
  bra .00040
.00039:
  ldi t0,0
.00040:
  mov t1,t0
; q = i ? 0 : q;
  beqz t0,.00041
  ldi t1,0
  ldi t0,0
  bra .00042
.00041:
  mov t0,t1
.00042:
  mov a0,t0
.00026:
  rts 
	.type	_main00144,@function
	.size	_main00144,$-_main00144


;--}

	.global	_main00144
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
