                                                                                                                                                    	.bss
	.align 4
__noname_var0:
	.type	__noname_var0,@object
	.size	__noname_var0,16

 	.bss
	.align 4
_point_array:
	.type	_point_array,@object
	.size	_point_array,1600

  
	.text

;{++ _main00195

	.sdreg	29
_main00195:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; int my_point = 10;
  ldi t0,10
  sto t0,-8[fp]
; point_array[my_point].x = 12.34;
  lda t0,_point_array[gp]
  ldo t1,0x0000[gp]
  sto t1,0[t0]
; point_array[my_point].y = 56.78;
  lda t0,_point_array[gp]
  ldo t1,0x0000[gp]
  sto t1,8[t0]
; printf("%f, %f\n", point_array[my_point].x, point_array[my_point].y);
  sub sp,sp,24
  lda t0,_main00195.00001[gp]
  sto t0,0[sp]
  lda t0,_point_array[gp]
  ldo t0,0[t0]
  sto t0,8[sp]
  lda t0,_point_array[gp]
  ldo t0,8[t0]
  sto t0,16[sp]
  bsr _printf
  add sp,sp,8
; return 0;
  mov a0,r0
.00011:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00195,@function
	.size	_main00195,$-_main00195


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00195.00001,@object
	.size	_main00195.00001,16
_main00195.00001: ; %f, %f

	.2byte	37,102,44,32,37,102,10,0
	.extern	__Files
	.global	_getc
	.global	_main00195
	.global	_gets
	.global	_putc
	.global	_puts
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
	.global	_feof
	.extern	_start_data
	.global	_point_array
	.global	__noname_var0
	.extern	_start_bss
	.extern	_start_rodata
