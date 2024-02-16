 	.bss
	.align 4
_e:
	.type	_e,@object
	.size	_e,8

 	.bss
	.align 4
_e1:
	.type	_e1,@object
	.size	_e1,8
 	.bss
	.align 4
_e2:
	.type	_e2,@object
	.size	_e2,8
 	.bss
	.align 4
_s:
	.type	_s,@object
	.size	_s,8

 	.bss
	.align 4
_s1:
	.type	_s1,@object
	.size	_s1,8
 	.bss
	.align 4
_s2:
	.type	_s2,@object
	.size	_s2,8
                                      
	.text

;{++ _f1

	.sdreg	29
_f1:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
  sub sp,sp,8
  sto s0,0[sp]
.00010:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 56,0
	.type	_f1,@function
	.size	_f1,$-_f1


;--}
   
;{++ _f2

	.sdreg	29
_f2:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
  sub sp,sp,8
  sto s0,0[sp]
.00020:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 56,0
	.type	_f2,@function
	.size	_f2,$-_f2


;--}
   
;{++ _f3

	.sdreg	29
_f3:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
  sub sp,sp,8
  sto s0,0[sp]
.00030:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 56,0
	.type	_f3,@function
	.size	_f3,$-_f3


;--}
   
;{++ _f4

	.sdreg	29
_f4:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
  sub sp,sp,8
  sto s0,0[sp]
.00040:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 80,0
	.type	_f4,@function
	.size	_f4,$-_f4


;--}
   
;{++ _f5

	.sdreg	29
_f5:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
  sub sp,sp,8
  sto s0,0[sp]
  jsr [s0]
.00050:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 48,0
	.type	_f5,@function
	.size	_f5,$-_f5


;--}
     
;{++ _main00209

	.sdreg	29
_main00209:
  mov a0,r0
.00060:
  rts 
	.type	_main00209,@function
	.size	_main00209,$-_main00209


;--}

	.global	_f1
	.global	_f2
	.global	_f3
	.global	_f4
	.global	_f5
	.global	_f8
	.global	_main00209
	.extern	_start_data
	.global	_s2
	.extern	_start_bss
	.global	_e
	.global	_s
	.global	_e2
	.global	_s1
	.extern	_start_rodata
	.global	_e1
