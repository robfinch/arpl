        
	.text

;{++ _foo

	.sdreg	29
_foo:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,72
  sto s0,[sp]
; i = 47;
  ldi t0,47
  sto t0,-8[fp]
; ma = 0x3f;
  ldi t0,63
  sto t0,-16[fp]
; mb = ma + 0x20;
  ldo t1,-16[fp]
  add t0,t1,32
  sto t0,-24[fp]
; df = 15.5D;
  ldo s0,0x4bc0[gp]
; vqf = ma(mb(vqf + df) * once);
  mov v4,t7
  vfadds v2,v4,s0,t0
  loadv v5,64[fp]
  vfmul v0,v2,v5,t1
  ldo a0,v0
.00010:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 128,0
	.type	_foo,@function
	.size	_foo,$-_foo


;--}
    