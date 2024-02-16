    
	.text

;{++ _main00143

	.sdreg	29
_main00143:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,80
  sto s0,[sp]
  sto s1,8[sp]
  sto s2,16[sp]
  sto s3,24[sp]
  sto s4,32[sp]
  sto s5,40[sp]
  lda s3,-188[fp]
  lda t0,-344[fp]
  lda s4,-344[fp]
; for(n = 0; n < 39; n++) {
  mov s0,r0
  ldi t1,39
  bge s0,t1,.00033
.00032:
; a[n] = n;
  stt s0,0[s3+s0*]
; b[n] = 0;
  stt r0,0[s4+s0*]
.00033:
; from = a;
  mov s1,s3
; to = b;
  mov s2,s4
; count = 39;
  ldi s5,39
; n = (count + 7) / 8;
  add t1,7,s5
  asr s0,t1,3
; switch (count % 8) {
  and t0,s5,7
; case 0: do { *to++ = *from++;
  bne t0,0,.00035
  ldt t0,[s1]
  stt t0,[s2]
  add s2,s2,4
  add s1,s1,4
.00001:
  ldt t0,[s1]
  stt t0,[s2]
  add s2,s2,4
  add s1,s1,4
  bra .00039
.00002:
  ldt t0,[s1]
  stt t0,[s2]
  add s2,s2,4
  add s1,s1,4
  bra .00039
.00003:
  ldt t0,[s1]
  stt t0,[s2]
  add s2,s2,4
  add s1,s1,4
  bra .00039
.00004:
  ldt t0,[s1]
  stt t0,[s2]
  add s2,s2,4
  add s1,s1,4
  bra .00039
.00007:
  ldt t0,[s1]
  stt t0,[s2]
  add s2,s2,4
  add s1,s1,4
.00039:
.00035:
; for(n = 0; n < 39; n++)
  mov s0,r0
  ldi t1,39
  bge s0,t1,.00041
.00040:
; if(a[n] != b[n])
  ldt t1,0[s3+s0*]
  ldt t2,0[s4+s0*]
  beq t1,t2,.00043
; return 1;
  ldi a0,1
.00031:
  ldo s0,[sp]
  ldo s1,8[sp]
  ldo s2,16[sp]
  ldo s3,24[sp]
  ldo s4,32[sp]
  ldo s5,40[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
.00043:
  ldi t1,39
  iblt s0,t1,.00040
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
