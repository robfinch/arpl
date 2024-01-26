    
	.text

;{++ _puts
	.sdreg	29
_puts:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
  ldo s0,0[fp]
; until (*str==0) begin
  ldw t1,[s0]
  beq t1,r0,.00015
.00014:
; putchar(*str);
  sub sp,sp,8
  ldw t0,[s0]
  sto t0,0[sp]
  bsr _putchar
; str++;
  add s0,s0,2
  ldw t1,[s0]
  bnez t1,.00014
.00015:
.00013:
  ldo s0,[sp]
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 40,0
	.type	_puts,@function
	.size	_puts,$-_puts


;--}
    
;{++ _main

	.sdreg	29
_main:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
; for (x = 1; x < 10; x++)
  ldi s0,1
  ldi t1,10
  bge s0,t1,.00031
.00030:
; puts("Hello World!");
  sub sp,sp,8
  lda t0,_main.00016[gp]
  orm t0,_main.00016
  sto t0,0[sp]
  bsr _puts
.00032:
  add s0,s0,1
  blt s0,t1,.00030
.00031:
.00029:
  ldo s0,[sp]
  ldo lr0,8[fp]
  mov sp,fp
  ldo fp,[sp]
  rtd 48,0
	.type	_main,@function
	.size	_main,$-_main


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main.00016,@object
	.size	_main.00016,26
_main.00016: ; Hello World!

	.2byte	72,101,108,108,111,32,87,111
	.2byte	114,108,100,33,0
	.global	_main
	.extern	_putchar
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
