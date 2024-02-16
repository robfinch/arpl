     	.bss
	.align 4
__brk_start:
	.type	__brk_start,@object
	.size	__brk_start,8

	.bss
	.align 4
__brk_end:
	.type	__brk_end,@object
	.size	__brk_end,8

    	.bss
	.align 4
_mask:
	.type	_mask,@object
	.size	_mask,8
	.8byte	0,0
 	.bss
	.align 4
_ret:
	.type	_ret,@object
	.size	_ret,8
	.8byte	0
   	.bss
	.align 4
__brk_start:
	.type	__brk_start,@object
	.size	__brk_start,8

  	.bss
	.align 4
__brk_end:
	.type	__brk_end,@object
	.size	__brk_end,8
	.8byte	0
 	.bss
	.align 4
_ret:
	.type	_ret,@object
	.size	_ret,8,0
 	.bss
	.align 4
__brk_end:
	.type	__brk_end,@object
	.size	__brk_end,8

 	.bss
	.align 4
__noname_var0:
	.type	__noname_var0,@object
	.size	__noname_var0,8

 	.bss
	.align 4
_ret:
	.type	_ret,@object
	.size	_ret,8

   
	.text

;{++ _get_args
	.sdreg	29
_get_args:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; if (a != 1)
  ldo t1,32[fp]
  ldi t2,1
  beq t1,t2,.00016
.00016:
.00015:
  mov sp,fp
  ldo fp,[sp]
  rtd 48,0
	.type	_get_args,@function
	.size	_get_args,$-_get_args


;--}
   