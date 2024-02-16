                                                                                                                                                  	.bss
	.align 4
_array:
	.type	_array,@object
	.size	_array,128

    
	.text

;{++ _swap

	.sdreg	29
_swap:
; int tmp  = array[a];
  lda t0,_array[gp]
  ldo t2,0[t0+t1*]
; array[a] = array[b];
  lda t0,_array[gp]
  lda t1,_array[gp]
  ldo t2,0[t1+t0*]
  sto t2,0[t0+t1*]
; array[b] = tmp;
  lda t0,_array[gp]
  sto t2,0[t0+t0*]
.00010:
  rts 
	.type	_swap,@function
	.size	_swap,$-_swap


;--}
       
;{++ _partition

	.sdreg	29
_partition:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  ldo t1,40[fp]
  ldo t2,32[fp]
  ldo t4,32[fp]
  lda t0,_array[gp]
  ldo t5,0[t0+t4*]
  mov t3,t2
; swap(pivotIndex, right);
  sub sp,sp,16
  sto t4,0[sp]
  sto t1,8[sp]
  bsr _swap
; for(i = left; i < right; i++)
  mov t0,t2
  bge t0,t1,.00027
.00026:
; if(array[i] < pivotValue)
  lda t1,_array[gp]
  ldo t0,0[t1+t0*]
  bge t0,t5,.00029
; swap(i, index);
  sub sp,sp,16
  sto t0,0[sp]
  sto t3,8[sp]
  bsr _swap
; index += 1;
  add t3,t3,1
.00029:
.00028:
.00027:
; swap(right, index);
  sub sp,sp,16
  sto t1,0[sp]
  sto t3,8[sp]
  bsr _swap
; return index;
  mov a0,t3
.00025:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 48,0
	.type	_partition,@function
	.size	_partition,$-_partition


;--}
    
;{++ _quicksort

	.sdreg	29
_quicksort:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  ldo t0,32[fp]
  ldo t1,40[fp]
; if(left >= right)
  sub sp,sp,16
  sto t0,0[sp]
  sto t1,8[sp]
  bsr _partition
  mov t2,a0
  blt t0,t1,.00043
.00042:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 48,0
.00043:
; quicksort(left, index - 1);
  sub sp,sp,16
  sto t0,0[sp]
  sub t0,t2,1
  sto t0,8[sp]
  bsr _quicksort
; quicksort(index + 1, right);
  sub sp,sp,16
  add t0,t2,1
  sto t0,0[sp]
  sto t1,8[sp]
  bsr _quicksort
  bra .00042
	.type	_quicksort,@function
	.size	_quicksort,$-_quicksort


;--}
  
;{++ _main00176

	.sdreg	29
_main00176:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; array[0] = 62;
  lda t0,_array[gp]
  ldi t1,62
  sto t1,0[t0]
; array[1] = 83;
  lda t0,_array[gp]
  ldi t1,83
  sto t1,1[t0]
; array[2] = 4;
  lda t0,_array[gp]
  ldi t1,4
  sto t1,2[t0]
; array[3] = 89;
  lda t0,_array[gp]
  ldi t1,89
  sto t1,3[t0]
; array[4] = 36;
  lda t0,_array[gp]
  ldi t1,36
  sto t1,4[t0]
; array[5] = 21;
  lda t0,_array[gp]
  ldi t1,21
  sto t1,5[t0]
; array[6] = 74;
  lda t0,_array[gp]
  ldi t1,74
  sto t1,6[t0]
; array[7] = 37;
  lda t0,_array[gp]
  ldi t1,37
  sto t1,7[t0]
; array[8] = 65;
  lda t0,_array[gp]
  ldi t1,65
  sto t1,8[t0]
; array[9] = 33;
  lda t0,_array[gp]
  ldi t1,33
  sto t1,9[t0]
; array[10] = 96;
  lda t0,_array[gp]
  ldi t1,96
  sto t1,10[t0]
; array[11] = 38;
  lda t0,_array[gp]
  ldi t1,38
  sto t1,11[t0]
; array[12] = 53;
  lda t0,_array[gp]
  ldi t1,53
  sto t1,12[t0]
; array[13] = 16;
  lda t0,_array[gp]
  ldi t1,16
  sto t1,13[t0]
; array[14] = 74;
  lda t0,_array[gp]
  ldi t1,74
  sto t1,14[t0]
; array[15] = 55;
  lda t0,_array[gp]
  ldi t1,55
  sto t1,15[t0]
; for (i = 0; i < 16; i++)
  mov t0,r0
  ldi t1,16
  bge t0,t1,.00066
.00065:
; printf("%d ", array[i]);
  sub sp,sp,16
  lda t0,_main00176.00045[gp]
  sto t0,0[sp]
  lda t0,_array[gp]
  ldo t0,0[t0+t0*]
  sto t0,8[sp]
  bsr _printf
.00066:
; printf("\n");
  sub sp,sp,8
  lda t0,_main00176.00046[gp]
  sto t0,0[sp]
  bsr _printf
; quicksort(0, 15);
  sub sp,sp,16
  sto r0,0[sp]
  ldi t0,15
  sto t0,8[sp]
  bsr _quicksort
; for (i = 0; i < 16; i++)
  mov t0,r0
  ldi t1,16
  bge t0,t1,.00069
; printf("%d ", array[i]);
  sub sp,sp,16
  lda t0,_main00176.00047[gp]
  sto t0,0[sp]
  lda t0,_array[gp]
  ldo t0,0[t0+t0*]
  sto t0,8[sp]
  bsr _printf
.00069:
; printf("\n");
  sub sp,sp,8
  lda t0,_main00176.00048[gp]
  sto t0,0[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00064:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00176,@function
	.size	_main00176,$-_main00176


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00176.00045,@object
	.size	_main00176.00045,8
_main00176.00045: ; %d 

	.2byte	37,100,32,0

	.type	_main00176.00046,@object
	.size	_main00176.00046,4
_main00176.00046:

	.2byte	10,0

	.type	_main00176.00047,@object
	.size	_main00176.00047,8
_main00176.00047: ; %d 

	.2byte	37,100,32,0

	.type	_main00176.00048,@object
	.size	_main00176.00048,4
_main00176.00048:

	.2byte	10,0
	.extern	__Files
	.global	_getc
	.global	_main00176
	.global	_gets
	.global	_swap
	.global	_putc
	.global	_puts
	.global	_partition
	.global	_getchar
	.global	_quicksort
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
	.extern	_start_bss
	.global	_array
	.extern	_start_rodata
