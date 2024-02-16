                                                                                                                                                                                                                                                                    	.bss
	.align 4
_A:
	.type	_A,@object
	.size	_A,32


	.data
	.align	6

	.data
	.align 4
_B:
	.type	_B,@object
	.size	_B,32

	.data
	.align 4
_C:
	.type	_C,@object
	.size	_C,32

       
	.text

;{++ _PrintAll

	.sdreg	29
_PrintAll:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
; printf("A: ");
  sub sp,sp,8
  lda t0,_PrintAll.00001[gp]
  sto t0,0[sp]
  bsr _printf
; for(i=0;i<4;i++)printf(" %d ",A[i]);
  mov s0,r0
  ldi t1,4
  bge s0,t1,.00031
.00030:
  sub sp,sp,16
  lda t0,_PrintAll.00002[gp]
  sto t0,0[sp]
  lda t0,00181.00158[r56]
  ldo t0,0[t0+s0*]
  sto t0,8[sp]
  bsr _printf
.00032:
.00031:
; printf("\n");
  sub sp,sp,8
  lda t0,_PrintAll.00003[gp]
  sto t0,0[sp]
  bsr _printf
; printf("B: ");
  sub sp,sp,8
  lda t0,_PrintAll.00004[gp]
  sto t0,0[sp]
  bsr _printf
; for(i=0;i<4;i++)printf(" %d ",B[i]);
  mov s0,r0
  bge s0,t1,.00034
.00033:
  sub sp,sp,16
  lda t0,_PrintAll.00005[gp]
  sto t0,0[sp]
  lda t0,00181.00192[r56]
  ldo t0,0[t0+s0*]
  sto t0,8[sp]
  bsr _printf
.00034:
; printf("\n");
  sub sp,sp,8
  lda t0,_PrintAll.00006[gp]
  sto t0,0[sp]
  bsr _printf
; printf("C: ");
  sub sp,sp,8
  lda t0,_PrintAll.00007[gp]
  sto t0,0[sp]
  bsr _printf
; for(i=0;i<4;i++)printf(" %d ",C[i]);
  mov s0,r0
  bge s0,t1,.00037
.00036:
  sub sp,sp,16
  lda t0,_PrintAll.00008[gp]
  sto t0,0[sp]
  lda t0,00181.00224[gp]
  ldo t0,0[t0+s0*]
  sto t0,8[sp]
  bsr _printf
  iblt s0,t1,.00036
.00037:
; printf("\n");
  sub sp,sp,8
  lda t0,_PrintAll.00009[gp]
  sto t0,0[sp]
  bsr _printf
; printf("------------------------------------------\n");
  sub sp,sp,8
  lda t0,_PrintAll.00010[gp]
  sto t0,0[sp]
  bsr _printf
.00029:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_PrintAll,@function
	.size	_PrintAll,$-_PrintAll


;--}
    
;{++ _Move

	.sdreg	29
_Move:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,64
  sto s0,[sp]
  sto s1,8[sp]
  sto s2,16[sp]
  sto s3,24[sp]
  ldo s1,32[fp]
  ldo s3,40[fp]
; int i = 0, j = 0;
  mov s0,r0
  mov s2,r0
; while (i<4 && (source[i])==0) i++;
  zslt t1,s0,4,1
  ldo t3,0[s1+s0*]
  zseq t2,t3,0,1
  and t3,t1,t2
  beqz t3,.00070
.00069:
  add s0,s0,1
  zslt t1,s0,4,1
  ldo t3,0[s1+s0*]
  zseq t2,t3,0,1
  and t3,t1,t2
  bnez t3,.00069
.00070:
; while (j<4 && (dest[j])==0) j++;
  zslt t1,s2,4,1
  ldo t3,0[s3+s2*]
  zseq t2,t3,0,1
  and t3,t1,t2
  beqz t3,.00080
.00079:
  add s2,s2,1
  zslt t1,s2,4,1
  ldo t3,0[s3+s2*]
  zseq t2,t3,0,1
  and t3,t1,t2
  bnez t3,.00079
.00080:
; dest[j-1] = source[i];
  sub t0,s2,1
  ldo t1,0[s1+s0*]
  sto t1,0[s3+t0*]
; source[i] = 0;
  sto r0,0[s1+s0*]
; PrintAll();       /* Print configuration after each move. */
  bsr _PrintAll
; return dest[j-1];
  sub t0,s2,1
  ldo t0,0[s3+t0*]
  ldo a0,0[s3+t0*]
.00068:
  ldo s0,[sp]
  ldo s1,8[sp]
  ldo s2,16[sp]
  ldo s3,24[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 48,0
	.type	_Move,@function
	.size	_Move,$-_Move


;--}
      
;{++ _Hanoi

	.sdreg	29
_Hanoi:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
; if(n==1){
  ldi t1,1
  bne s0,t1,.00101
; Move(source,dest);
  sub sp,sp,16
  sto s0,0[sp]
  sto s0,8[sp]
  bsr _Move
.00100:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 64,0
.00101:
; Hanoi(n-1,source,spare,dest);
  sub sp,sp,32
  sub t0,s0,1
  sto t0,0[sp]
  sto s0,8[sp]
  sto s0,16[sp]
  sto s0,24[sp]
  bsr _Hanoi
; Move(source,dest);
  sub sp,sp,16
  sto s0,0[sp]
  sto s0,8[sp]
  bsr _Move
; Hanoi(n-1,spare,dest,source);
  sub sp,sp,32
  sub t0,s0,1
  sto t0,0[sp]
  sto s0,8[sp]
  sto s0,16[sp]
  sto s0,24[sp]
  bsr _Hanoi
; return;
  bra .00100
	.type	_Hanoi,@function
	.size	_Hanoi,$-_Hanoi


;--}
  
;{++ _main00181

	.sdreg	29
_main00181:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
; for(i=0;i<4;i++)A[i]=i+1;
  mov s0,r0
  ldi t1,4
  bge s0,t1,.00126
  lda t0,00181.00158[r56]
  add t1,s0,1
  sto t1,0[t0+s0*]
  ldi t1,4
.00126:
; for(i=0;i<4;i++)B[i]=0;
  mov s0,r0
  bge s0,t1,.00129
  lda t0,00181.00192[r56]
  sto r0,0[t0+s0*]
.00129:
; for(i=0;i<4;i++)C[i]=0;
  mov s0,r0
  bge s0,t1,.00132
.00131:
  lda t0,00181.00224[gp]
  sto r0,0[t0+s0*]
  iblt s0,t1,.00131
.00132:
; printf("Solution of Tower of Hanoi Problem with %d Disks\n\n",4);
  sub sp,sp,16
  lda t0,_main00181.00103[gp]
  sto t0,0[sp]
  ldi t0,4
  sto t0,8[sp]
  bsr _printf
; printf("Starting state:\n");
  sub sp,sp,8
  lda t0,_main00181.00104[gp]
  sto t0,0[sp]
  bsr _printf
; PrintAll();
  bsr _PrintAll
; printf("\n\nSubsequent states:\n\n");
  sub sp,sp,8
  lda t0,_main00181.00105[gp]
  sto t0,0[sp]
  bsr _printf
; Hanoi(4,A,B,C);
  sub sp,sp,32
  ldi t0,4
  sto t0,0[sp]
  lda t0,00181.00158[r56]
  sto t0,8[sp]
  lda t0,00181.00192[r56]
  sto t0,16[sp]
  lda t0,00181.00224[gp]
  sto t0,24[sp]
  bsr _Hanoi
; return 0;
  mov a0,r0
.00124:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00181,@function
	.size	_main00181,$-_main00181


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_PrintAll.00001,@object
	.size	_PrintAll.00001,8
_PrintAll.00001: ; A: 

	.2byte	65,58,32,0

	.type	_PrintAll.00002,@object
	.size	_PrintAll.00002,10
_PrintAll.00002: ;  %d 

	.2byte	32,37,100,32,0

	.type	_PrintAll.00003,@object
	.size	_PrintAll.00003,4
_PrintAll.00003:

	.2byte	10,0

	.type	_PrintAll.00004,@object
	.size	_PrintAll.00004,8
_PrintAll.00004: ; B: 

	.2byte	66,58,32,0

	.type	_PrintAll.00005,@object
	.size	_PrintAll.00005,10
_PrintAll.00005: ;  %d 

	.2byte	32,37,100,32,0

	.type	_PrintAll.00006,@object
	.size	_PrintAll.00006,4
_PrintAll.00006:

	.2byte	10,0

	.type	_PrintAll.00007,@object
	.size	_PrintAll.00007,8
_PrintAll.00007: ; C: 

	.2byte	67,58,32,0

	.type	_PrintAll.00008,@object
	.size	_PrintAll.00008,10
_PrintAll.00008: ;  %d 

	.2byte	32,37,100,32,0

	.type	_PrintAll.00009,@object
	.size	_PrintAll.00009,4
_PrintAll.00009:

	.2byte	10,0

	.type	_PrintAll.00010,@object
	.size	_PrintAll.00010,88
_PrintAll.00010: ; ------------------------------------------

	.2byte	45,45,45,45,45,45,45,45
	.2byte	45,45,45,45,45,45,45,45
	.2byte	45,45,45,45,45,45,45,45
	.2byte	45,45,45,45,45,45,45,45
	.2byte	45,45,45,45,45,45,45,45
	.2byte	45,45,10,0

	.type	_main00181.00103,@object
	.size	_main00181.00103,102
_main00181.00103: ; Solution of Tower of Hanoi Problem with %d Disks

	.2byte	83,111,108,117,116,105,111,110
	.2byte	32,111,102,32,84,111,119,101
	.2byte	114,32,111,102,32,72,97,110
	.2byte	111,105,32,80,114,111,98,108
	.2byte	101,109,32,119,105,116,104,32
	.2byte	37,100,32,68,105,115,107,115
	.2byte	10,10,0

	.type	_main00181.00104,@object
	.size	_main00181.00104,34
_main00181.00104: ; Starting state:

	.2byte	83,116,97,114,116,105,110,103
	.2byte	32,115,116,97,116,101,58,10
	.2byte	0

	.type	_main00181.00105,@object
	.size	_main00181.00105,46
_main00181.00105: ; Subsequent states:

	.2byte	10,10,83,117,98,115,101,113
	.2byte	117,101,110,116,32,115,116,97
	.2byte	116,101,115,58,10,10,0
	.extern	__Mbxlen
	.global	_A
	.global	_B
	.extern	__Wcxtomb
	.extern	__Mbxtowc
	.extern	__Files
	.extern	__Randseed
	.extern	__Mbcurmax
	.global	_free
	.global	_labs
	.global	_getc
	.global	_rand
	.global	_system
	.global	_strtol
	.global	_atof
	.global	_atoi
	.global	_ldiv
	.global	_atol
	.global	_gets
	.global	_exit
	.global	_putc
	.global	__Mbtowc
	.global	__Wctomb
	.global	_puts
	.global	_bsearch
	.global	_getchar
	.global	_realloc
	.global	_freopen
	.global	_Hanoi
	.global	_tmpfile
	.global	_putchar
	.global	_fgetpos
	.global	_fprintf
	.global	__Stod
	.global	_setvbuf
	.global	_fread
	.global	_fsetpos
	.global	_sprintf
	.global	_fgetc
	.global	_vprintf
	.global	_scanf
	.global	_fseek
	.global	_mblen
	.global	_ftell
	.global	_fopen
	.global	_abort
	.global	_srand
	.global	_fgets
	.global	_strtoul
	.global	_fputc
	.global	_PrintAll
	.global	_fputs
	.global	_abs
	.global	_qsort
	.global	_div
	.global	_clearerr
	.global	__Fgpos
	.global	__Fspos
	.global	_calloc
	.global	_vfprintf
	.global	_fscanf
	.global	_mbstowcs
	.global	_wcstombs
	.global	__Stoul
	.global	_rename
	.global	_malloc
	.global	_fclose
	.global	_vsprintf
	.global	_sscanf
	.global	_ungetc
	.global	_fflush
	.global	_rewind
	.global	_setbuf
	.global	_getenv
	.global	_mbtowc
	.global	_wctomb
	.global	_tmpnam
	.global	_remove
	.global	_atexit
	.global	_ferror
	.global	_fwrite
	.global	_setjmp
	.global	_printf
	.global	_Move
	.global	_perror
	.global	_main00181
	.global	_feof
	.global	_strtod
	.extern	_start_data
	.extern	_start_bss
	.global	_C
	.extern	_start_rodata
