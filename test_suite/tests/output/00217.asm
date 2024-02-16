    	.bss
	.align 4
_t:
	.type	_t,@object
	.size	_t,2
	.2byte	0,0,0,0,0,0,0,0
	.2byte	0,0
   
	.text
   
;{++ _main00217

	.sdreg	29
_main00217:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; char *data = t;
  ldi t2,4
  ldi t3,5
  ldi t1,12
; *(unsigned*)(data + r) += a - b;
  asl t0,t2,1
  sub t1,t3,t1
  ldo t1,0[t0+t0*]
  add t1,t1,t1
  sto t1,0[t0+t0*]
; printf("data = \"%s\"\n", data);
  sub sp,sp,16
  lda t0,_main00217.00002[gp]
  sto t0,0[sp]
  sto t0,8[sp]
  bsr _printf
; return 0;
  mov a0,r0
.00012:
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00217,@function
	.size	_main00217,$-_main00217


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00217.00002,@object
	.size	_main00217.00002,26
_main00217.00002: ; data = "%s"

	.2byte	100,97,116,97,32,61,32,34
	.2byte	37,115,34,10,0
	.global	_printf
	.global	_main00217
	.extern	_start_data
	.extern	_start_bss
	.global	_t
	.extern	_start_rodata
