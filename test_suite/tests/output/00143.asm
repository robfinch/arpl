    
	.text

;{++ _main00143

	.sdreg	29
_main00143:
; for(n = 0; n < 39; n++) {
  mov t0,r0
  ldi t1,39
  bge t0,t1,.00033
; a[n] = n;
  stt t0,0[t3+t0*]
; b[n] = 0;
  stt r0,0[t4+t0*]
.00033:
; to = b;
  mov t2,t4
; count = 39;
  ldi t5,39
; n = (count + 7) / 8;
  add t1,7,t5
; switch (count % 8) {
  and t0,t5,7
; case 0: do { *to++ = *from++;
  bne t0,0,.00035
  ldt t0,[t1]
  stt t0,[t2]
  add t2,t2,4
  add t1,t1,4
.00001:
  ldt t0,[t1]
  stt t0,[t2]
  add t2,t2,4
  add t1,t1,4
  bra .00039
.00002:
  ldt t0,[t1]
  stt t0,[t2]
  add t2,t2,4
  add t1,t1,4
  bra .00039
.00004:
  ldt t0,[t1]
  stt t0,[t2]
  add t2,t2,4
  add t1,t1,4
  bra .00039
.00007:
  ldt t0,[t1]
  stt t0,[t2]
.00039:
.00035:
; for(n = 0; n < 39; n++)
  mov t0,r0
  ldi t1,39
  bge t0,t1,.00041
; if(a[n] != b[n])
  ldt t0,0[t3+t0*]
  ldt t0,0[t4+t0*]
  beq t0,t0,.00043
; return 1;
  ldi a0,1
.00031:
  rts 
.00043:
.00041:
; return 0;
  mov a0,r0
  bra .00031
	.type	_main00143,@function
	.size	_main00143,$-_main00143


;--}

	.global	_main00143
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
