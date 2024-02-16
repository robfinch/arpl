    	.bss
	.align 4
__noname_var0:
	.type	__noname_var0,@object
	.size	__noname_var0,96

     
	.text

;{++ _f1

	.sdreg	29
_f1:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
  ldw s0,128[fp]
; if (f.i != p->i)
  ldo t1,[fp]
  ldo t1,0[t1]
  beq t1,s0,.00013
.00013:
; return 0;
  mov a0,r0
.00012:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 144,0
	.type	_f1,@function
	.size	_f1,$-_f1


;--}
   
;{++ _main00140

	.sdreg	29
_main00140:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,40
  sto s0,[sp]
  lda t0,-96[fp]
  lda s0,-96[fp]
; f.i = f.j = 1;
  ldo t0,[fp]
  ldi t1,1
  sto t1,768[s0]
  ldo t1,768[s0]
  sto t1,-96[t0]
; f1(f, &f, 2);
  sub sp,sp,16
  lea t0,0[sp]
  sto t0,8[sp]
  sto s0,0[sp]
  ldi t0,96
  sto t0,96
  bsr __aacpy
  ldi t0,0
  sto t0,0[sp]
  sto s0,0[sp]
  ldi t0,2
  sto t0,8[sp]
  bsr _f1
; f1(f, &f, 2, 1, f, &f);
  sub sp,sp,32
  lea t0,0[sp]
  sto t0,8[sp]
  sto s0,0[sp]
  ldi t0,96
  sto t0,96
  bsr __aacpy
  ldi t0,0
  sto t0,0[sp]
  sto s0,0[sp]
  ldi t0,2
  sto t0,8[sp]
  ldi t0,1
  sto t0,16[sp]
  lea t0,0[sp]
  sto t0,8[sp]
  sto s0,0[sp]
  ldi t0,96
  sto t0,96
  bsr __aacpy
  ldi t0,0
  sto t0,24[sp]
  sto s0,24[sp]
  bsr _f1
  add sp,sp,8
; return 0;
  mov a0,r0
.00024:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00140,@function
	.size	_main00140,$-_main00140


;--}

	.global	_f1
	.global	_main00140
	.extern	_start_data
	.global	__noname_var0
	.extern	_start_bss
	.extern	_start_rodata
