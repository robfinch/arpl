                                                                                                                                                                                                                               
	.text

;{++ _main00179

	.sdreg	29
_main00179:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
  lda t0,-20[fp]
; strcpy(a, "hello");
  sub sp,sp,16
  sto t0,0[sp]
  lda t0,_main00179.00001[gp]
  sto t0,8[sp]
  bsr _strcpy
; printf("%s\n", a);
  sub sp,sp,16
  lda t0,_main00179.00002[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
; strncpy(a, "gosh", 2);
  sub sp,sp,24
  sto t0,0[sp]
  lda t0,_main00179.00003[gp]
  sto t0,8[sp]
  ldi t0,2
  sto t0,16[sp]
  bsr _strncpy
; printf("%s\n", a);
  sub sp,sp,16
  lda t0,_main00179.00004[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", strcmp(a, "apple") > 0);
  sub sp,sp,16
  lda t0,_main00179.00005[gp]
  sto t0,0[sp]
  sto t0,-32[fp]
  sub sp,sp,16
  sto t0,0[sp]
  lda t1,_main00179.00006[gp]
  sto t1,8[sp]
  bsr _strcmp
  zsgt t0,a0,0,1
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", strcmp(a, "goere") > 0);
  sub sp,sp,16
  lda t0,_main00179.00007[gp]
  sto t0,0[sp]
  sto t0,-32[fp]
  sub sp,sp,16
  sto t0,0[sp]
  lda t1,_main00179.00008[gp]
  sto t1,8[sp]
  bsr _strcmp
  zsgt t0,a0,0,1
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", strcmp(a, "zebra") < 0);
  sub sp,sp,16
  lda t0,_main00179.00009[gp]
  sto t0,0[sp]
  sto t0,-32[fp]
  sub sp,sp,16
  sto t0,0[sp]
  lda t1,_main00179.00010[gp]
  sto t1,8[sp]
  bsr _strcmp
  zslt t0,a0,0,1
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", strlen(a));
  sub sp,sp,16
  lda t0,_main00179.00011[gp]
  sto t0,0[sp]
  sub sp,sp,8
  sto t0,0[sp]
  bsr _strlen
  sto a0,8[sp]
  bsr _printf
; strcat(a, "!");
  sub sp,sp,16
  sto t0,0[sp]
  lda t0,_main00179.00012[gp]
  sto t0,8[sp]
  bsr _strcat
; printf("%s\n", a);
  sub sp,sp,16
  lda t0,_main00179.00013[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", strncmp(a, "apple", 2) > 0);
  sub sp,sp,16
  lda t0,_main00179.00014[gp]
  sto t0,0[sp]
  sto t0,-32[fp]
  sub sp,sp,24
  sto t0,0[sp]
  lda t1,_main00179.00015[gp]
  sto t1,8[sp]
  ldi t1,2
  sto t1,16[sp]
  bsr _strncmp
  zsgt t0,a0,0,1
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", strncmp(a, "goere", 2) == 0);
  sub sp,sp,16
  lda t0,_main00179.00016[gp]
  sto t0,0[sp]
  sto t0,-32[fp]
  sub sp,sp,24
  sto t0,0[sp]
  lda t1,_main00179.00017[gp]
  sto t1,8[sp]
  ldi t1,2
  sto t1,16[sp]
  bsr _strncmp
  zseq t0,a0,0,1
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", strncmp(a, "goerg", 2) == 0);
  sub sp,sp,16
  lda t0,_main00179.00018[gp]
  sto t0,0[sp]
  sto t0,-32[fp]
  sub sp,sp,24
  sto t0,0[sp]
  lda t1,_main00179.00019[gp]
  sto t1,8[sp]
  ldi t1,2
  sto t1,16[sp]
  bsr _strncmp
  zseq t0,a0,0,1
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", strncmp(a, "zebra", 2) < 0);
  sub sp,sp,16
  lda t0,_main00179.00020[gp]
  sto t0,0[sp]
  sto t0,-32[fp]
  sub sp,sp,24
  sto t0,0[sp]
  lda t1,_main00179.00021[gp]
  sto t1,8[sp]
  ldi t1,2
  sto t1,16[sp]
  bsr _strncmp
  zslt t0,a0,0,1
  sto t0,8[sp]
  bsr _printf
; printf("%s\n", strchr(a, 'o'));
  sub sp,sp,16
  lda t0,_main00179.00022[gp]
  sto t0,0[sp]
  sub sp,sp,16
  sto t0,0[sp]
  ldi t0,111
  sto t0,8[sp]
  bsr _strchr
  sto a0,8[sp]
  bsr _printf
; printf("%s\n", strrchr(a, 'l'));
  sub sp,sp,16
  lda t0,_main00179.00023[gp]
  sto t0,0[sp]
  sub sp,sp,16
  sto t0,0[sp]
  ldi t0,108
  sto t0,8[sp]
  bsr _strrchr
  sto a0,8[sp]
  bsr _printf
; printf("%d\n", strrchr(a, 'x') == (void *)0);
  sub sp,sp,16
  lda t0,_main00179.00024[gp]
  sto t0,0[sp]
  sto t0,-32[fp]
  sub sp,sp,16
  sto t0,0[sp]
  ldi t1,120
  sto t1,8[sp]
  bsr _strrchr
  zseq t0,a0,0,1
  sto t0,8[sp]
  bsr _printf
; memset(&a[1], 'r', 4);
  sub sp,sp,24
  add t0,t0,1
  sto t0,0[sp]
  ldi t0,114
  sto t0,8[sp]
  ldi t0,4
  sto t0,16[sp]
  bsr _memset
; printf("%s\n", a);
  sub sp,sp,16
  lda t0,_main00179.00025[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
; memcpy(&a[2], a, 2);
  sub sp,sp,24
  add t0,t0,2
  sto t0,0[sp]
  sto t0,8[sp]
  ldi t0,2
  sto t0,16[sp]
  bsr _memcpy
; printf("%s\n", a);
  sub sp,sp,16
  lda t0,_main00179.00026[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", memcmp(a, "apple", 4) > 0);
  sub sp,sp,16
  lda t0,_main00179.00027[gp]
  sto t0,0[sp]
  sto t0,-32[fp]
  sub sp,sp,24
  sto t0,0[sp]
  lda t1,_main00179.00028[gp]
  sto t1,8[sp]
  ldi t1,4
  sto t1,16[sp]
  bsr _memcmp
  zsgt t0,a0,0,1
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", memcmp(a, "grgr", 4) == 0);
  sub sp,sp,16
  lda t0,_main00179.00029[gp]
  sto t0,0[sp]
  sto t0,-32[fp]
  sub sp,sp,24
  sto t0,0[sp]
  lda t1,_main00179.00030[gp]
  sto t1,8[sp]
  ldi t1,4
  sto t1,16[sp]
  bsr _memcmp
  zseq t0,a0,0,1
  sto t0,8[sp]
  bsr _printf
; printf("%d\n", memcmp(a, "zebra", 4) < 0);
  sub sp,sp,16
  lda t0,_main00179.00031[gp]
  sto t0,0[sp]
  sto t0,-32[fp]
  sub sp,sp,24
  sto t0,0[sp]
  lda t1,_main00179.00032[gp]
  sto t1,8[sp]
  ldi t1,4
  sto t1,16[sp]
  bsr _memcmp
  zslt t0,a0,0,1
  sto t0,8[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00064:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00179,@function
	.size	_main00179,$-_main00179


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00179.00001,@object
	.size	_main00179.00001,12
_main00179.00001: ; hello

	.2byte	104,101,108,108,111,0

	.type	_main00179.00002,@object
	.size	_main00179.00002,8
_main00179.00002: ; %s

	.2byte	37,115,10,0

	.type	_main00179.00003,@object
	.size	_main00179.00003,10
_main00179.00003: ; gosh

	.2byte	103,111,115,104,0

	.type	_main00179.00004,@object
	.size	_main00179.00004,8
_main00179.00004: ; %s

	.2byte	37,115,10,0

	.type	_main00179.00005,@object
	.size	_main00179.00005,8
_main00179.00005: ; %d

	.2byte	37,100,10,0

	.type	_main00179.00006,@object
	.size	_main00179.00006,12
_main00179.00006: ; apple

	.2byte	97,112,112,108,101,0

	.type	_main00179.00007,@object
	.size	_main00179.00007,8
_main00179.00007: ; %d

	.2byte	37,100,10,0

	.type	_main00179.00008,@object
	.size	_main00179.00008,12
_main00179.00008: ; goere

	.2byte	103,111,101,114,101,0

	.type	_main00179.00009,@object
	.size	_main00179.00009,8
_main00179.00009: ; %d

	.2byte	37,100,10,0

	.type	_main00179.00010,@object
	.size	_main00179.00010,12
_main00179.00010: ; zebra

	.2byte	122,101,98,114,97,0

	.type	_main00179.00011,@object
	.size	_main00179.00011,8
_main00179.00011: ; %d

	.2byte	37,100,10,0

	.type	_main00179.00012,@object
	.size	_main00179.00012,4
_main00179.00012: ; !

	.2byte	33,0

	.type	_main00179.00013,@object
	.size	_main00179.00013,8
_main00179.00013: ; %s

	.2byte	37,115,10,0

	.type	_main00179.00014,@object
	.size	_main00179.00014,8
_main00179.00014: ; %d

	.2byte	37,100,10,0

	.type	_main00179.00015,@object
	.size	_main00179.00015,12
_main00179.00015: ; apple

	.2byte	97,112,112,108,101,0

	.type	_main00179.00016,@object
	.size	_main00179.00016,8
_main00179.00016: ; %d

	.2byte	37,100,10,0

	.type	_main00179.00017,@object
	.size	_main00179.00017,12
_main00179.00017: ; goere

	.2byte	103,111,101,114,101,0

	.type	_main00179.00018,@object
	.size	_main00179.00018,8
_main00179.00018: ; %d

	.2byte	37,100,10,0

	.type	_main00179.00019,@object
	.size	_main00179.00019,12
_main00179.00019: ; goerg

	.2byte	103,111,101,114,103,0

	.type	_main00179.00020,@object
	.size	_main00179.00020,8
_main00179.00020: ; %d

	.2byte	37,100,10,0

	.type	_main00179.00021,@object
	.size	_main00179.00021,12
_main00179.00021: ; zebra

	.2byte	122,101,98,114,97,0

	.type	_main00179.00022,@object
	.size	_main00179.00022,8
_main00179.00022: ; %s

	.2byte	37,115,10,0

	.type	_main00179.00023,@object
	.size	_main00179.00023,8
_main00179.00023: ; %s

	.2byte	37,115,10,0

	.type	_main00179.00024,@object
	.size	_main00179.00024,8
_main00179.00024: ; %d

	.2byte	37,100,10,0

	.type	_main00179.00025,@object
	.size	_main00179.00025,8
_main00179.00025: ; %s

	.2byte	37,115,10,0

	.type	_main00179.00026,@object
	.size	_main00179.00026,8
_main00179.00026: ; %s

	.2byte	37,115,10,0

	.type	_main00179.00027,@object
	.size	_main00179.00027,8
_main00179.00027: ; %d

	.2byte	37,100,10,0

	.type	_main00179.00028,@object
	.size	_main00179.00028,12
_main00179.00028: ; apple

	.2byte	97,112,112,108,101,0

	.type	_main00179.00029,@object
	.size	_main00179.00029,8
_main00179.00029: ; %d

	.2byte	37,100,10,0

	.type	_main00179.00030,@object
	.size	_main00179.00030,10
_main00179.00030: ; grgr

	.2byte	103,114,103,114,0

	.type	_main00179.00031,@object
	.size	_main00179.00031,8
_main00179.00031: ; %d

	.2byte	37,100,10,0

	.type	_main00179.00032,@object
	.size	_main00179.00032,12
_main00179.00032: ; zebra

	.2byte	122,101,98,114,97,0
	.extern	__Files
	.global	_getc
	.global	_strcpy
	.global	_main00179
	.global	_strtok
	.global	_strspn
	.global	_strstr
	.global	_gets
	.global	_putc
	.global	__Strerror
	.global	_puts
	.global	_getchar
	.global	_freopen
	.global	_tmpfile
	.global	_memmove
	.global	_putchar
	.global	_fgetpos
	.global	_fprintf
	.global	_setvbuf
	.global	_strncat
	.global	_fread
	.global	_strcoll
	.global	_fsetpos
	.global	_sprintf
	.global	_strncmp
	.global	_strpbrk
	.global	_strrchr
	.global	_fgetc
	.global	_vprintf
	.global	_scanf
	.global	_strcspn
	.global	_fseek
	.global	_strncpy
	.global	_strxfrm
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
	.global	_memchr
	.global	_sscanf
	.global	_memcmp
	.global	_strerror
	.global	_ungetc
	.global	_fflush
	.global	_rewind
	.global	_setbuf
	.global	_memcpy
	.global	_memset
	.global	_tmpnam
	.global	_remove
	.global	_ferror
	.global	_fwrite
	.global	_strcat
	.global	_setjmp
	.global	_printf
	.global	_strchr
	.global	_strlen
	.global	_strcmp
	.global	_perror
	.global	_feof
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
