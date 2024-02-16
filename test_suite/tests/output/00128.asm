 	.bss
	.align 4
_a:
	.type	_a,@object
	.size	_a,8

 	.bss
	.align 4
_b:
	.type	_b,@object
	.size	_b,8

 	.bss
	.align 4
_c:
	.type	_c,@object
	.size	_c,2

 	.bss
	.align 4
_d:
	.type	_d,@object
	.size	_d,2

 	.bss
	.align 4
_e:
	.type	_e,@object
	.size	_e,2

 	.bss
	.align 4
_f:
	.type	_f,@object
	.size	_f,8

 	.bss
	.align 4
_g:
	.type	_g,@object
	.size	_g,8

 	.bss
	.align 4
_h:
	.type	_h,@object
	.size	_h,8

 	.bss
	.align 4
_i:
	.type	_i,@object
	.size	_i,8

 	.bss
	.align 4
_j:
	.type	_j,@object
	.size	_j,4

 	.bss
	.align 4
_k:
	.type	_k,@object
	.size	_k,4

  
	.text

;{++ _main00128

	.sdreg	29
_main00128:
; a = b;
  lda t0,_a[gp]
  lda t1,_b[gp]
  ldo t2,[t1]
  sto t2,[t0]
; a = c;
  lda t0,_a[gp]
  lda t1,_c[gp]
  ldw t2,[t1]
  sto t2,[t0]
; a = d;
  lda t0,_a[gp]
  lda t1,_d[gp]
  ldw t2,[t1]
  sto t2,[t0]
; a = e;
  lda t0,_a[gp]
  lda t1,_e[gp]
  ldw t2,[t1]
  sto t2,[t0]
; a = f;
  lda t0,_a[gp]
  lda t1,_f[gp]
  ldo t2,[t1]
  sto t2,[t0]
; a = g;
  lda t0,_a[gp]
  lda t1,_g[gp]
  ldo t2,[t1]
  sto t2,[t0]
; a = h;
  lda t0,_a[gp]
  lda t1,_h[gp]
  ldo t2,[t1]
  sto t2,[t0]
; a = i;
  lda t0,_a[gp]
  lda t1,_i[gp]
  ldo t2,[t1]
  sto t2,[t0]
; a = j;
  lda t0,_a[gp]
  lda t1,_j[gp]
  ldt t2,[t1]
  sto t2,[t0]
; a = k;
  lda t0,_a[gp]
  lda t1,_k[gp]
  ldt t2,[t1]
  sto t2,[t0]
; b = a;
  lda t0,_b[gp]
  lda t1,_a[gp]
  ldo t2,[t1]
  sto t2,[t0]
; b = c;
  lda t0,_b[gp]
  lda t1,_c[gp]
  ldw t2,[t1]
  sto t2,[t0]
; b = d;
  lda t0,_b[gp]
  lda t1,_d[gp]
  ldw t2,[t1]
  sto t2,[t0]
; b = e;
  lda t0,_b[gp]
  lda t1,_e[gp]
  ldw t2,[t1]
  sto t2,[t0]
; b = f;
  lda t0,_b[gp]
  lda t1,_f[gp]
  ldo t2,[t1]
  sto t2,[t0]
; b = g;
  lda t0,_b[gp]
  lda t1,_g[gp]
  ldo t2,[t1]
  sto t2,[t0]
; b = h;
  lda t0,_b[gp]
  lda t1,_h[gp]
  ldo t2,[t1]
  sto t2,[t0]
; b = i;
  lda t0,_b[gp]
  lda t1,_i[gp]
  ldo t2,[t1]
  sto t2,[t0]
; b = j;
  lda t0,_b[gp]
  lda t1,_j[gp]
  ldt t2,[t1]
  sto t2,[t0]
; b = k;
  lda t0,_b[gp]
  lda t1,_k[gp]
  ldt t2,[t1]
  sto t2,[t0]
; c = a;
  lda t0,_c[gp]
  lda t1,_a[gp]
  ldo t2,[t1]
  stw t2,[t0]
; c = b;
  lda t0,_c[gp]
  lda t1,_b[gp]
  ldo t2,[t1]
  stw t2,[t0]
; c = d;
  lda t0,_c[gp]
  lda t1,_d[gp]
  ldw t2,[t1]
  stw t2,[t0]
; c = e;
  lda t0,_c[gp]
  lda t1,_e[gp]
  ldw t2,[t1]
  stw t2,[t0]
; c = f;
  lda t0,_c[gp]
  lda t1,_f[gp]
  ldo t2,[t1]
  stw t2,[t0]
; c = g;
  lda t0,_c[gp]
  lda t1,_g[gp]
  ldo t2,[t1]
  stw t2,[t0]
; c = h;
  lda t0,_c[gp]
  lda t1,_h[gp]
  ldo t2,[t1]
  stw t2,[t0]
; c = i;
  lda t0,_c[gp]
  lda t1,_i[gp]
  ldo t2,[t1]
  stw t2,[t0]
; c = j;
  lda t0,_c[gp]
  lda t1,_j[gp]
  ldt t2,[t1]
  stw t2,[t0]
; c = k;
  lda t0,_c[gp]
  lda t1,_k[gp]
  ldt t2,[t1]
  stw t2,[t0]
; d = a;
  lda t0,_d[gp]
  lda t1,_a[gp]
  ldo t2,[t1]
  stw t2,[t0]
; d = b;
  lda t0,_d[gp]
  lda t1,_b[gp]
  ldo t2,[t1]
  stw t2,[t0]
; d = c;
  lda t0,_d[gp]
  lda t1,_c[gp]
  ldw t2,[t1]
  stw t2,[t0]
; d = e;
  lda t0,_d[gp]
  lda t1,_e[gp]
  ldw t2,[t1]
  stw t2,[t0]
; d = f;
  lda t0,_d[gp]
  lda t1,_f[gp]
  ldo t2,[t1]
  stw t2,[t0]
; d = g;
  lda t0,_d[gp]
  lda t1,_g[gp]
  ldo t2,[t1]
  stw t2,[t0]
; d = h;
  lda t0,_d[gp]
  lda t1,_h[gp]
  ldo t2,[t1]
  stw t2,[t0]
; d = i;
  lda t0,_d[gp]
  lda t1,_i[gp]
  ldo t2,[t1]
  stw t2,[t0]
; d = j;
  lda t0,_d[gp]
  lda t1,_j[gp]
  ldt t2,[t1]
  stw t2,[t0]
; d = k;
  lda t0,_d[gp]
  lda t1,_k[gp]
  ldt t2,[t1]
  stw t2,[t0]
; e = a;
  lda t0,_e[gp]
  lda t1,_a[gp]
  ldo t2,[t1]
  stw t2,[t0]
; e = b;
  lda t0,_e[gp]
  lda t1,_b[gp]
  ldo t2,[t1]
  stw t2,[t0]
; e = c;
  lda t0,_e[gp]
  lda t1,_c[gp]
  ldw t2,[t1]
  stw t2,[t0]
; e = d;
  lda t0,_e[gp]
  lda t1,_d[gp]
  ldw t2,[t1]
  stw t2,[t0]
; e = f;
  lda t0,_e[gp]
  lda t1,_f[gp]
  ldo t2,[t1]
  stw t2,[t0]
; e = g;
  lda t0,_e[gp]
  lda t1,_g[gp]
  ldo t2,[t1]
  stw t2,[t0]
; e = h;
  lda t0,_e[gp]
  lda t1,_h[gp]
  ldo t2,[t1]
  stw t2,[t0]
; e = i;
  lda t0,_e[gp]
  lda t1,_i[gp]
  ldo t2,[t1]
  stw t2,[t0]
; e = j;
  lda t0,_e[gp]
  lda t1,_j[gp]
  ldt t2,[t1]
  stw t2,[t0]
; e = k;
  lda t0,_e[gp]
  lda t1,_k[gp]
  ldt t2,[t1]
  stw t2,[t0]
; f = a;
  lda t0,_f[gp]
  lda t1,_a[gp]
  ldo t2,[t1]
  sto t2,[t0]
; f = b;
  lda t0,_f[gp]
  lda t1,_b[gp]
  ldo t2,[t1]
  sto t2,[t0]
; f = c;
  lda t0,_f[gp]
  lda t1,_c[gp]
  ldw t2,[t1]
  sto t2,[t0]
; f = d;
  lda t0,_f[gp]
  lda t1,_d[gp]
  ldw t2,[t1]
  sto t2,[t0]
; f = e;
  lda t0,_f[gp]
  lda t1,_e[gp]
  ldw t2,[t1]
  sto t2,[t0]
; f = g;
  lda t0,_f[gp]
  lda t1,_g[gp]
  ldo t2,[t1]
  sto t2,[t0]
; f = h;
  lda t0,_f[gp]
  lda t1,_h[gp]
  ldo t2,[t1]
  sto t2,[t0]
; f = i;
  lda t0,_f[gp]
  lda t1,_i[gp]
  ldo t2,[t1]
  sto t2,[t0]
; f = j;
  lda t0,_f[gp]
  lda t1,_j[gp]
  ldt t2,[t1]
  sto t2,[t0]
; f = k;
  lda t0,_f[gp]
  lda t1,_k[gp]
  ldt t2,[t1]
  sto t2,[t0]
; g = a;
  lda t0,_g[gp]
  lda t1,_a[gp]
  ldo t2,[t1]
  sto t2,[t0]
; g = b;
  lda t0,_g[gp]
  lda t1,_b[gp]
  ldo t2,[t1]
  sto t2,[t0]
; g = c;
  lda t0,_g[gp]
  lda t1,_c[gp]
  ldw t2,[t1]
  sto t2,[t0]
; g = d;
  lda t0,_g[gp]
  lda t1,_d[gp]
  ldw t2,[t1]
  sto t2,[t0]
; g = e;
  lda t0,_g[gp]
  lda t1,_e[gp]
  ldw t2,[t1]
  sto t2,[t0]
; g = f;
  lda t0,_g[gp]
  lda t1,_f[gp]
  ldo t2,[t1]
  sto t2,[t0]
; g = h;
  lda t0,_g[gp]
  lda t1,_h[gp]
  ldo t2,[t1]
  sto t2,[t0]
; g = i;
  lda t0,_g[gp]
  lda t1,_i[gp]
  ldo t2,[t1]
  sto t2,[t0]
; g = j;
  lda t0,_g[gp]
  lda t1,_j[gp]
  ldt t2,[t1]
  sto t2,[t0]
; g = k;
  lda t0,_g[gp]
  lda t1,_k[gp]
  ldt t2,[t1]
  sto t2,[t0]
; h = a;
  lda t0,_h[gp]
  lda t1,_a[gp]
  ldo t2,[t1]
  sto t2,[t0]
; h = b;
  lda t0,_h[gp]
  lda t1,_b[gp]
  ldo t2,[t1]
  sto t2,[t0]
; h = c;
  lda t0,_h[gp]
  lda t1,_c[gp]
  ldw t2,[t1]
  sto t2,[t0]
; h = d;
  lda t0,_h[gp]
  lda t1,_d[gp]
  ldw t2,[t1]
  sto t2,[t0]
; h = e;
  lda t0,_h[gp]
  lda t1,_e[gp]
  ldw t2,[t1]
  sto t2,[t0]
; h = f;
  lda t0,_h[gp]
  lda t1,_f[gp]
  ldo t2,[t1]
  sto t2,[t0]
; h = g;
  lda t0,_h[gp]
  lda t1,_g[gp]
  ldo t2,[t1]
  sto t2,[t0]
; h = i;
  lda t0,_h[gp]
  lda t1,_i[gp]
  ldo t2,[t1]
  sto t2,[t0]
; h = j;
  lda t0,_h[gp]
  lda t1,_j[gp]
  ldt t2,[t1]
  sto t2,[t0]
; h = k;
  lda t0,_h[gp]
  lda t1,_k[gp]
  ldt t2,[t1]
  sto t2,[t0]
; i = a;
  lda t0,_i[gp]
  lda t1,_a[gp]
  ldo t2,[t1]
  sto t2,[t0]
; i = b;
  lda t0,_i[gp]
  lda t1,_b[gp]
  ldo t2,[t1]
  sto t2,[t0]
; i = c;
  lda t0,_i[gp]
  lda t1,_c[gp]
  ldw t2,[t1]
  sto t2,[t0]
; i = d;
  lda t0,_i[gp]
  lda t1,_d[gp]
  ldw t2,[t1]
  sto t2,[t0]
; i = e;
  lda t0,_i[gp]
  lda t1,_e[gp]
  ldw t2,[t1]
  sto t2,[t0]
; i = f;
  lda t0,_i[gp]
  lda t1,_f[gp]
  ldo t2,[t1]
  sto t2,[t0]
; i = g;
  lda t0,_i[gp]
  lda t1,_g[gp]
  ldo t2,[t1]
  sto t2,[t0]
; i = h;
  lda t0,_i[gp]
  lda t1,_h[gp]
  ldo t2,[t1]
  sto t2,[t0]
; i = j;
  lda t0,_i[gp]
  lda t1,_j[gp]
  ldt t2,[t1]
  sto t2,[t0]
; i = k;
  lda t0,_i[gp]
  lda t1,_k[gp]
  ldt t2,[t1]
  sto t2,[t0]
; j = a;
  lda t0,_j[gp]
  lda t1,_a[gp]
  ldo t2,[t1]
  stt t2,[t0]
; j = b;
  lda t0,_j[gp]
  lda t1,_b[gp]
  ldo t2,[t1]
  stt t2,[t0]
; j = c;
  lda t0,_j[gp]
  lda t1,_c[gp]
  ldw t2,[t1]
  stt t2,[t0]
; j = d;
  lda t0,_j[gp]
  lda t1,_d[gp]
  ldw t2,[t1]
  stt t2,[t0]
; j = e;
  lda t0,_j[gp]
  lda t1,_e[gp]
  ldw t2,[t1]
  stt t2,[t0]
; j = f;
  lda t0,_j[gp]
  lda t1,_f[gp]
  ldo t2,[t1]
  stt t2,[t0]
; j = g;
  lda t0,_j[gp]
  lda t1,_g[gp]
  ldo t2,[t1]
  stt t2,[t0]
; j = h;
  lda t0,_j[gp]
  lda t1,_h[gp]
  ldo t2,[t1]
  stt t2,[t0]
; j = i;
  lda t0,_j[gp]
  lda t1,_i[gp]
  ldo t2,[t1]
  stt t2,[t0]
; j = k;
  lda t0,_j[gp]
  lda t1,_k[gp]
  ldt t2,[t1]
  stt t2,[t0]
; k = a;
  lda t0,_k[gp]
  lda t1,_a[gp]
  ldo t2,[t1]
  stt t2,[t0]
; k = b;
  lda t0,_k[gp]
  lda t1,_b[gp]
  ldo t2,[t1]
  stt t2,[t0]
; k = c;
  lda t0,_k[gp]
  lda t1,_c[gp]
  ldw t2,[t1]
  stt t2,[t0]
; k = d;
  lda t0,_k[gp]
  lda t1,_d[gp]
  ldw t2,[t1]
  stt t2,[t0]
; k = e;
  lda t0,_k[gp]
  lda t1,_e[gp]
  ldw t2,[t1]
  stt t2,[t0]
; k = f;
  lda t0,_k[gp]
  lda t1,_f[gp]
  ldo t2,[t1]
  stt t2,[t0]
; k = g;
  lda t0,_k[gp]
  lda t1,_g[gp]
  ldo t2,[t1]
  stt t2,[t0]
; k = h;
  lda t0,_k[gp]
  lda t1,_h[gp]
  ldo t2,[t1]
  stt t2,[t0]
; k = j;
  lda t0,_k[gp]
  lda t1,_j[gp]
  ldt t2,[t1]
  stt t2,[t0]
; k = i;
  lda t0,_k[gp]
  lda t1,_i[gp]
  ldo t2,[t1]
  stt t2,[t0]
; return 0;
  mov a0,r0
.00010:
  rts 
	.type	_main00128,@function
	.size	_main00128,$-_main00128


;--}

	.global	_main00128
	.extern	_start_data
	.extern	_start_bss
	.global	_a
	.global	_b
	.global	_c
	.global	_d
	.global	_e
	.global	_f
	.global	_g
	.global	_h
	.global	_i
	.global	_j
	.global	_k
	.extern	_start_rodata
