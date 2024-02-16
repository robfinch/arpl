 	.bss
	.align 4
_flags:
	.type	_flags,@object
	.size	_flags,16382

    
	.text

;{++ _sieve

	.sdreg	29
_sieve:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,72
  sto s0,[sp]
  sto s1,8[sp]
  sto s2,16[sp]
  sto s3,24[sp]
  sto s4,32[sp]
; printf("10 iterations\n");
  sub sp,sp,8
  lda t0,_sieve.00001[gp]
  sto t0,0[sp]
  bsr _printf
  add sp,sp,8
; for(iter=1;iter<= 10;iter++){
  ldi s1,1
  ldi t1,10
  bgt s1,t1,.00027
.00026:
; count=0;
  mov s3,r0
; for(i = 0; i<=8190;i++)
  mov s0,r0
  ldi t1,8190
  bgt s0,t1,.00030
.00029:
; flags[i]=true;
  lda t0,00303.00000[gp]
  ldi t1,1
  stw t1,0[t0+s0*]
.00031:
  ldi t1,8190
.00030:
; for(i=0;i <= 8190; i++){
  mov s0,r0
  bgt s0,t1,.00033
.00032:
; if(flags[i]){
  lda t0,00303.00000[gp]
  ldw t0,0[t0+s0*]
  beqz t0,.00035
; prime = i+i+3;
  add t1,s0,s0
  add s4,t1,3
; k=i+prime;
  add t0,s0,s4
; k=i+prime;
  add s2,s0,s4
; while(k<=8190){
  ldi t1,8190
  bgt s2,t1,.00038
.00037:
; flags[k] = false;
  lda t0,00303.00000[gp]
  stw r0,0[t0+s2*]
; k += prime;
  add s2,s2,s4
  ble s2,t1,.00037
.00038:
; count = count+1;
  add s3,s3,1
.00035:
  ible s0,t1,.00032
.00033:
.00028:
  ldi t1,10
  ible s1,t1,.00026
.00027:
; printf("\n%d primes\n",count);
  sub sp,sp,16
  lda t0,_sieve.00002[gp]
  sto t0,0[sp]
  sto s3,8[sp]
  bsr _printf
  add sp,sp,16
.00025:
  ldo s0,[sp]
  ldo s1,8[sp]
  ldo s2,16[sp]
  ldo s3,24[sp]
  ldo s4,32[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_sieve,@function
	.size	_sieve,$-_sieve


;--}
   
       

;{++ _main00303__sub1__sub2

	.sdreg	29
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto s0,[sp]
  sto s1,8[sp]
  ldo s0,40[fp]
  ldo s1,32[fp]
; c = c + g + i;
  ldo t2,32[fp]
  ldo t3,[fp]
  ldo t3,-8[t3]
  add t1,t2,t3
  ldo t2,[fp]
  ldo t2,[t2]
  ldo t2,-8[t2]
  add t0,t1,t2
  sto t0,32[fp]
; d = d + h;
  ldo t1,40[fp]
  ldo t2,[fp]
  ldo t2,-16[t2]
  add t0,t1,t2
  sto t0,40[fp]
; return (c*d);
  ldo t1,32[fp]
  ldo t2,40[fp]
  mul a0,t1,t2
.00043:
  ldo s0,[sp]
  ldo s1,8[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 48,0
	.type	_main00303__sub1__sub2,@function
	.size	_main00303__sub1__sub2,$-_main00303__sub1__sub2



;{++ _main00303__foo1

	.sdreg	29
; return 43;
  ldi a0,43
.00055:
  rts 
	.type	_main00303__foo1,@function
	.size	_main00303__foo1,$-_main00303__foo1


;{++ _main00303__sub1

	.sdreg	29
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
; g = 2; h = 3;
  ldi t0,2
  sto t0,-8[fp]
  ldi t0,3
  sto t0,-16[fp]
; return (a+b);
  lda t1,_sub1.09999[r56]
  lda t2,_sub1.09999[r56]
  add a0,t1,t2
.00065:
  mov sp,fp
  ldo fp,[sp]
  rtd 48,0
	.type	_main00303__sub1,@function
	.size	_main00303__sub1,$-_main00303__sub1


;{++ _main00303

	.sdreg	29
_main00303:
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,32
; i = 47;
  ldi t0,47
  sto t0,-8[fp]
; printf("%d", foo());
  sub sp,sp,16
  lda t0,_main00303.00049[gp]
  sto t0,0[sp]
  bsr _foo
  sto a0,8[sp]
  bsr _printf
  add sp,sp,16
; printf("%d", foo1()*8);
  sub sp,sp,16
  lda t0,_main00303.00050[gp]
  sto t0,0[sp]
  sto t0,-32[fp]
  bsr _foo1
  asl.b t0,a0,3
  sto t0,8[sp]
  bsr _printf
  add sp,sp,16
; sieve();
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sto lr0,8[fp]
  sub sp,sp,72
  sto s0,[sp]
  sto s1,8[sp]
  sto s2,16[sp]
  sto s3,24[sp]
  sto s4,32[sp]
; printf("10 iterations\n");
  sub sp,sp,8
  lda t0,_sieve.00001[gp]
  sto t0,0[sp]
  bsr _printf
  add sp,sp,8
; for(iter=1;iter<= 10;iter++){
  ldi s1,1
  ldi t1,10
  bgt s1,t1,.02075
; count=0;
  mov s3,r0
; for(i = 0; i<=8190;i++)
  mov s0,r0
  ldi t1,8190
  bgt s0,t1,.02078
; flags[i]=true;
  lda t0,00303.00000[gp]
  ldi t1,1
  stw t1,0[t0+s0*]
  ldi t1,8190
.02078:
; for(i=0;i <= 8190; i++){
  mov s0,r0
  bgt s0,t1,.02081
; if(flags[i]){
  lda t0,00303.00000[gp]
  ldw t0,0[t0+s0*]
  beqz t0,.02083
; prime = i+i+3;
  add t1,s0,s0
  add s4,t1,3
; k=i+prime;
  add t0,s0,s4
; k=i+prime;
  add s2,s0,s4
; while(k<=8190){
  ldi t1,8190
  bgt s2,t1,.02086
.02085:
; flags[k] = false;
  lda t0,00303.00000[gp]
  stw r0,0[t0+s2*]
; k += prime;
  add s2,s2,s4
  ble s2,t1,.02085
.02086:
; count = count+1;
  add s3,s3,1
.02083:
  ible s0,t1,.00032
.02081:
  ldi t1,10
  ible s1,t1,.00026
.02075:
; printf("\n%d primes\n",count);
  sub sp,sp,16
  lda t0,_sieve.00002[gp]
  sto t0,0[sp]
  sto s3,8[sp]
  bsr _printf
  add sp,sp,16
  ldo s0,[sp]
  ldo s1,8[sp]
  ldo s2,16[sp]
  ldo s3,24[sp]
  ldo s4,32[sp]
  mov sp,fp
  ldo fp,[sp]
.00080:
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
	.type	_main00303,@function
	.size	_main00303,$-_main00303


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_sieve.00001,@object
	.size	_sieve.00001,30
_sieve.00001: ; 10 iterations

	.2byte	49,48,32,105,116,101,114,97
	.2byte	116,105,111,110,115,10,0

	.type	_sieve.00002,@object
	.size	_sieve.00002,24
_sieve.00002: ; %d primes

	.2byte	10,37,100,32,112,114,105,109
	.2byte	101,115,10,0

	.type	_main00303.00049,@object
	.size	_main00303.00049,6
_main00303.00049: ; %d

	.2byte	37,100,0

	.type	_main00303.00050,@object
	.size	_main00303.00050,6
_main00303.00050: ; %d

	.2byte	37,100,0
	.extern	_foo
	.extern	_printf
	.global	_sieve
	.global	_main00303
	.extern	_start_data
	.extern	_start_bss
	.global	_flags
	.extern	_start_rodata
