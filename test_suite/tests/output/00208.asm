
	.text
 
;{++ _f1char

	.sdreg	29


	.bss
	.align	6
	.type	_f1char.00000,@object
	.size	_f1char.00000,10
_f1char.00000: ; __aggregate_tag
	.byte	0,0,0,0,0,0,0,0,0,0



_f1char:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
; return !s[0];
  lda t1,1[fp]
  ldw t1,0[t1]
  not a0,t1
.00013:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_f1char,@function
	.size	_f1char,$-_f1char


;--}
   