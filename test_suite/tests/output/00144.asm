    
	.text

;{++ _main00144

	.sdreg	29
_main00144:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,56
  sto s0,[sp]
  sto s1,8[sp]
  sto s2,16[sp]
; i = i ? 0 : 0l;
  beqz s0,.00027
  ldi t0,0
.00027:
.00028:
  mov s0,t0
; p = i ? (void *) 0 : 0;
  beqz s0,.00029
.00029:
.00030:
  mov s2,t0
; p = i ? 0 : (void *) 0;
  beqz s0,.00031
.00031:
.00032:
  mov s2,t0
; p = i ? 0 : (const void *) 0;
  beqz s0,.00033
.00033:
  mov s2,t0
; q = i ? 0 : p;
  beqz s0,.00035
  bra .00036
.00035:
  mov t0,s2
.00036:
  mov s1,t0
; q = i ? p : 0;
  beqz s0,.00037
  mov t0,s2
.00037:
  mov s1,t0
; q = i ? q : 0;
  beqz s0,.00039
  mov t0,s1
.00039:
  mov s1,t0
; q = i ? 0 : q;
  beqz s0,.00041
  bra .00042
.00041:
  mov t0,s1
.00042:
  mov s1,t0
  mov a0,t0
.00026:
  ldo s0,[sp]
  ldo s1,8[sp]
  ldo s2,16[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00144,@function
	.size	_main00144,$-_main00144


;--}

	.global	_main00144
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
