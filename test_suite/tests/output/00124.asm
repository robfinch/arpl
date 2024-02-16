   
	.text

;{++ _f2

	.sdreg	29
_f2:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
  ldo t1,32[fp]
  ldo t2,40[fp]
  sub a0,t1,t2
.00010:
  mov sp,fp
  ldo fp,[sp]
  rtd 48,0
	.type	_f2,@function
	.size	_f2,$-_f2


;--}
      