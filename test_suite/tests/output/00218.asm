 	.bss
	.align 4
__noname_var0:
	.type	__noname_var0,@object
	.size	__noname_var0,2

      	.bss
	.align 4
__noname_var1:
	.type	__noname_var1,@object
	.size	__noname_var1,24

  	.bss
	.align 4
__noname_var2:
	.type	__noname_var2,@object
	.size	__noname_var2,24

 	.bss
	.align 4
__noname_var3:
	.type	__noname_var3,@object
	.size	__noname_var3,2

 	.bss
	.align 4
__noname_var4:
	.type	__noname_var4,@object
	.size	__noname_var4,2

    
	.text

;{++ _blah

	.sdreg	29
_blah:
  mov a0,r0
.00010:
  rts 
	.type	_blah,@function
	.size	_blah,$-_blah


;--}
  
;{++ _convert_like_real

	.sdreg	29
_convert_like_real:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
.00032:
; printf("unsigned enum bit-fields broken\n");
  sub sp,sp,8
  lda t0,_convert_like_real.00012[gp]
  sto t0,0[sp]
  bsr _printf
.00031:
  mov sp,fp
  ldo fp,[sp]
  rtd 40,0
	.type	_convert_like_real,@function
	.size	_convert_like_real,$-_convert_like_real


;--}
  
;{++ _main00218

	.sdreg	29
_main00218:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; convs.common.code = AMBIG_CONV;
  lda t0,-24[fp]
  ldw t1,384[t0]
  ldi t2,1
  dep t1,t2,0,7
  stw t1,384[t0]
; convert_like_real (&convs);
  sub sp,sp,8
  lda t0,-24[fp]
  sto t0,0[sp]
  bsr _convert_like_real
; return 0;
  mov a0,r0
.00050:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00218,@function
	.size	_main00218,$-_main00218


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_convert_like_real.00012,@object
	.size	_convert_like_real.00012,66
_convert_like_real.00012: ; unsigned enum bit-fields broken

	.2byte	117,110,115,105,103,110,101,100
	.2byte	32,101,110,117,109,32,98,105
	.2byte	116,45,102,105,101,108,100,115
	.2byte	32,98,114,111,107,101,110,10
	.2byte	0
	.global	_convert_like_real
	.extern	_printf
	.global	_blah
	.global	_main00218
	.extern	_start_data
	.global	__noname_var0
	.global	__noname_var1
	.global	__noname_var2
	.global	__noname_var3
	.global	__noname_var4
	.extern	_start_bss
	.extern	_start_rodata
