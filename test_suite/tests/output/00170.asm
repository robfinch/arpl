                                                                                                                                                  	.bss
	.align 4
__noname_var0:
	.type	__noname_var0,@object
	.size	__noname_var0,2

 	.bss
	.align 4
__noname_var1:
	.type	__noname_var1,@object
	.size	__noname_var1,2

    	.bss
	.align 4
__noname_var2:
	.type	__noname_var2,@object
	.size	__noname_var2,8

    	.bss
	.align 4
_TWO:
	.type	_TWO,@object
	.size	_TWO,8

    	.bss
	.align 4
__noname_var3:
	.type	__noname_var3,@object
	.size	__noname_var3,8

  
	.text

;{++ _should_compile

	.sdreg	29
_should_compile:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
  ldou t0,32[fp]
  ldi t1,0
  sto t1,0[t0]
.00010:
  mov sp,fp
  ldo fp,[sp]
  rtd 40,0
	.type	_should_compile,@function
	.size	_should_compile,$-_should_compile


;--}
  
;{++ _it_real_fn

	.sdreg	29
_it_real_fn:
  lda a0,_TWO[r56]
.00020:
  rts 
	.type	_it_real_fn,@function
	.size	_it_real_fn,$-_it_real_fn


;--}
  
;{++ _deref_uintptr
	.sdreg	29
_deref_uintptr:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,32
  ldou t0,32[fp]
  ldo t0,[t0]
  ldo a0,[t0]
.00031:
  mov sp,fp
  ldo fp,[sp]
  rtd 40,0
	.type	_deref_uintptr,@function
	.size	_deref_uintptr,$-_deref_uintptr


;--}
 	.align 4
__noname_var4:
	.type	__noname_var4,@object
	.size	__noname_var4,2

   
;{++ _main00170

	.sdreg	29
_main00170:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; enum fred frod;
  ldi t0,1
  stw t0,-4[fp]
; printf("%d %d %d %d %d %d %d %d\n", a, b, c, d, e, f, g, h);
  sub sp,sp,72
  lda t0,_main00170.00032[gp]
  sto t0,0[sp]
  sto r0,8[sp]
  ldi t0,1
  sto t0,16[sp]
  ldi t0,2
  sto t0,24[sp]
  ldi t0,3
  sto t0,32[sp]
  ldi t0,54
  sto t0,40[sp]
  ldi t0,73
  sto t0,48[sp]
  ldi t0,6
  sto t0,56[sp]
  ldi t0,7
  sto t0,64[sp]
  bsr _printf
  add sp,sp,56
; printf("%d\n", frod);
  sub sp,sp,16
  lda t0,_main00170.00033[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", frod);
  sub sp,sp,16
  lda t0,_main00170.00034[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
; printf ("enum to int: %u\n", deref_uintptr(&epos));
  sub sp,sp,16
  lda t0,_main00170.00035[gp]
  sto t0,0[sp]
  sub sp,sp,8
  lda t0,-4[fp]
  sto t0,0[sp]
  bsr _deref_uintptr
  sto a0,8[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00045:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00170,@function
	.size	_main00170,$-_main00170


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00170.00032,@object
	.size	_main00170.00032,50
_main00170.00032: ; %d %d %d %d %d %d %d %d

	.2byte	37,100,32,37,100,32,37,100
	.2byte	32,37,100,32,37,100,32,37
	.2byte	100,32,37,100,32,37,100,10
	.2byte	0

	.type	_main00170.00033,@object
	.size	_main00170.00033,8
_main00170.00033: ; %d

	.2byte	37,100,10,0

	.type	_main00170.00034,@object
	.size	_main00170.00034,8
_main00170.00034: ; %d

	.2byte	37,100,10,0

	.type	_main00170.00035,@object
	.size	_main00170.00035,34
_main00170.00035: ; enum to int: %u

	.2byte	101,110,117,109,32,116,111,32
	.2byte	105,110,116,58,32,37,117,10
	.2byte	0
	.global	_TWO
	.extern	__Files
	.global	_getc
	.global	_gets
	.global	_putc
	.global	_puts
	.global	_should_compile
	.global	_getchar
	.global	_freopen
	.global	_tmpfile
	.global	_putchar
	.global	_fgetpos
	.global	_fprintf
	.global	_setvbuf
	.global	_fread
	.global	_fsetpos
	.global	_sprintf
	.global	_fgetc
	.global	_vprintf
	.global	_scanf
	.global	_fseek
	.global	_it_real_fn
	.global	_ftell
	.global	_fopen
	.global	_fgets
	.global	_fputc
	.global	_fputs
	.global	_clearerr
	.global	__Fgpos
	.global	__Fspos
	.global	_vfprintf
	.global	_fscanf
	.global	_rename
	.global	_fclose
	.global	_vsprintf
	.global	_sscanf
	.global	_ungetc
	.global	_fflush
	.global	_rewind
	.global	_setbuf
	.global	_tmpnam
	.global	_remove
	.global	_ferror
	.global	_fwrite
	.global	_setjmp
	.global	_printf
	.global	_perror
	.global	_main00170
	.global	_feof
	.extern	_start_data
	.global	__noname_var0
	.global	__noname_var1
	.global	__noname_var2
	.global	__noname_var3
	.global	__noname_var4
	.extern	_start_bss
	.extern	_start_rodata
