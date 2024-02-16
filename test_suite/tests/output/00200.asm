     
	.text

;{++ _check
	.sdreg	29
_check:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  ldo t1,48[fp]
  ldo t2,40[fp]
; int failed = arg1 != shift;
  zsne t0,t2,t1,1
; if (debug || failed)
  ldo t1,00200.00001[r56]
  or t2,t1,t0
  beqz t2,.00021
; printf ("%s %d %d\n", s, arg1, shift);
  sub sp,sp,32
  lda t0,00200.00004[gp]
  sto t0,0[sp]
  ldo t0,32[fp]
  sto t0,8[sp]
  sto t2,16[sp]
  sto t1,24[sp]
  bsr _printf
  add sp,sp,16
.00021:
; nfailed += failed;
  ldo t0,00200.00002[gp]
  add t0,t0,t0
  sto t0,00200.00002[gp]
.00018:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 56,0
	.type	_check,@function
	.size	_check,$-_check


;--}
   