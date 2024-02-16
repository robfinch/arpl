      
	.text

;{++ _kb_wait_1
	.sdreg	29
_kb_wait_1:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
.00020:
; if (1) printf("timeout=%ld\n", timeout);
  ldi t0,1
  beqz t0,.00022
  sub sp,sp,16
  lda t0,00215.00002[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
  bra .00023
.00022:
; while (1)
  ldi t0,1
  beqz t0,.00025
.00024:
; printf("error\n");
  sub sp,sp,8
  lda t0,00215.00003[gp]
  sto t0,0[sp]
  bsr _printf
  ldi t0,1
  bnez t0,.00024
.00025:
.00023:
; timeout--;
  sub t0,t0,1
;} while (timeout);
  bnez t0,.00020
.00019:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_kb_wait_1,@function
	.size	_kb_wait_1,$-_kb_wait_1


;--}
   
;{++ _kb_wait_2
	.sdreg	29
_kb_wait_2:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
.00046:
; if (1) printf("timeout=%ld\n", timeout);
  ldi t0,1
  beqz t0,.00048
  sub sp,sp,16
  lda t0,00215.00027[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
  bra .00049
.00048:
; printf("error\n");
  sub sp,sp,8
  lda t0,00215.00028[gp]
  sto t0,0[sp]
  bsr _printf
.00049:
; timeout--;
  sub t0,t0,1
;} while (timeout);
  bnez t0,.00046
.00045:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_kb_wait_2,@function
	.size	_kb_wait_2,$-_kb_wait_2


;--}
   
;{++ _kb_wait_2_1
	.sdreg	29
_kb_wait_2_1:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
.00072:
; if (1) printf("timeout=%ld\n", timeout);
  ldi t0,1
  beqz t0,.00074
  sub sp,sp,16
  lda t0,00215.00054[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
  bra .00075
.00074:
.00076:
; printf("error\n");
  sub sp,sp,8
  lda t0,00215.00055[gp]
  sto t0,0[sp]
  bsr _printf
;} while (1);
  ldi t0,1
  bnez t0,.00076
.00075:
; timeout--;
  sub t0,t0,1
;} while (timeout);
  bnez t0,.00072
.00071:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_kb_wait_2_1,@function
	.size	_kb_wait_2_1,$-_kb_wait_2_1


;--}
   
;{++ _kb_wait_2_2
	.sdreg	29
_kb_wait_2_2:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
.00096:
; if (1) printf("timeout=%ld\n", timeout);
  ldi t0,1
  beqz t0,.00098
  sub sp,sp,16
  lda t0,00215.00079[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
  bra .00099
.00098:
.00080:
; printf("error\n");
  sub sp,sp,8
  lda t0,00215.00081[gp]
  sto t0,0[sp]
  bsr _printf
; goto label;
  bra .00080
.00099:
; timeout--;
  sub t0,t0,1
;} while (timeout);
  bnez t0,.00096
.00095:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_kb_wait_2_2,@function
	.size	_kb_wait_2_2,$-_kb_wait_2_2


;--}
    
;{++ _kb_wait_3
	.sdreg	29
_kb_wait_3:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
.00117:
; if (1) printf("timeout=%ld\n", timeout);
  ldi t0,1
  beqz t0,.00119
  sub sp,sp,16
  lda t0,00215.00101[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
  bra .00120
.00119:
; int i = 1;
  ldi t1,1
; i = i + 3;
  ldo t1,-16[fp]
  add t0,t1,3
  sto t0,-16[fp]
.00120:
; timeout--;
  sub t0,t0,1
;} while (timeout);
  bnez t0,.00117
.00116:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_kb_wait_3,@function
	.size	_kb_wait_3,$-_kb_wait_3


;--}
   
;{++ _kb_wait_4
	.sdreg	29
_kb_wait_4:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
.00150:
; if (1) printf("timeout=%ld\n", timeout);
  ldi t0,1
  beqz t0,.00152
  sub sp,sp,16
  lda t0,00215.00122[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
  bra .00153
.00152:
; case 2:
  bne t0,2,.00157
; printf("timeout is 2");
  sub sp,sp,8
  lda t0,00215.00123[gp]
  sto t0,0[sp]
  bsr _printf
.00157:
  bne t0,1,.00128
.00128:
.00153:
; timeout--;
  sub t0,t0,1
;} while (timeout);
  bnez t0,.00150
.00149:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_kb_wait_4,@function
	.size	_kb_wait_4,$-_kb_wait_4


;--}
 
;{++ _main00215

	.sdreg	29
_main00215:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; printf("begin\n");
  sub sp,sp,8
  lda t0,_main00215.00161[gp]
  sto t0,0[sp]
  bsr _printf
; kb_wait_1();
  bsr _kb_wait_1
; kb_wait_2();
  bsr _kb_wait_2
; kb_wait_2_1();
  bsr _kb_wait_2_1
; kb_wait_2_2();
  bsr _kb_wait_2_2
; kb_wait_3();
  bsr _kb_wait_3
; kb_wait_4();
  bsr _kb_wait_4
; printf("end\n");
  sub sp,sp,8
  lda t0,_main00215.00162[gp]
  sto t0,0[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00172:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00215,@function
	.size	_main00215,$-_main00215


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	00215.00002,@object
	.size	00215.00002,26
00215.00002: ; timeout=%ld

	.2byte	116,105,109,101,111,117,116,61
	.2byte	37,108,100,10,0

	.type	00215.00003,@object
	.size	00215.00003,14
00215.00003: ; error

	.2byte	101,114,114,111,114,10,0

	.type	00215.00027,@object
	.size	00215.00027,26
00215.00027: ; timeout=%ld

	.2byte	116,105,109,101,111,117,116,61
	.2byte	37,108,100,10,0

	.type	00215.00028,@object
	.size	00215.00028,14
00215.00028: ; error

	.2byte	101,114,114,111,114,10,0

	.type	00215.00054,@object
	.size	00215.00054,26
00215.00054: ; timeout=%ld

	.2byte	116,105,109,101,111,117,116,61
	.2byte	37,108,100,10,0

	.type	00215.00055,@object
	.size	00215.00055,14
00215.00055: ; error

	.2byte	101,114,114,111,114,10,0

	.type	00215.00079,@object
	.size	00215.00079,26
00215.00079: ; timeout=%ld

	.2byte	116,105,109,101,111,117,116,61
	.2byte	37,108,100,10,0

	.type	00215.00081,@object
	.size	00215.00081,14
00215.00081: ; error

	.2byte	101,114,114,111,114,10,0

	.type	00215.00101,@object
	.size	00215.00101,26
00215.00101: ; timeout=%ld

	.2byte	116,105,109,101,111,117,116,61
	.2byte	37,108,100,10,0

	.type	00215.00122,@object
	.size	00215.00122,26
00215.00122: ; timeout=%ld

	.2byte	116,105,109,101,111,117,116,61
	.2byte	37,108,100,10,0

	.type	00215.00123,@object
	.size	00215.00123,26
00215.00123: ; timeout is 2

	.2byte	116,105,109,101,111,117,116,32
	.2byte	105,115,32,50,0

	.type	_kb_wait_4.00125,@object
	.size	_kb_wait_4.00125,26
_kb_wait_4.00125: ; timeout is 1

	.2byte	116,105,109,101,111,117,116,32
	.2byte	105,115,32,49,0

	.type	_kb_wait_4.00127,@object
	.size	_kb_wait_4.00127,28
_kb_wait_4.00127: ; timeout is 0?

	.2byte	116,105,109,101,111,117,116,32
	.2byte	105,115,32,48,63,0

	.type	_main00215.00161,@object
	.size	_main00215.00161,14
_main00215.00161: ; begin

	.2byte	98,101,103,105,110,10,0

	.type	_main00215.00162,@object
	.size	_main00215.00162,10
_main00215.00162: ; end

	.2byte	101,110,100,10,0
	.extern	_printf
	.global	_main00215
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
