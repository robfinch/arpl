  
	.text

;{++ _main00026

	.sdreg	29
_main00026:
; p = "hello";
  lda t0,_main00026.00001[gp]
; return p[0] - 104;
  ldw t0,0[t0]
  sub t0,t0,104
  sub a0,t0,104
.00011:
  rts 
	.type	_main00026,@function
	.size	_main00026,$-_main00026


;--}

	.rodata
	.align	6

	.align	8



	.align	6
	.type	_main00026.00001,@object
	.size	_main00026.00001,12
_main00026.00001: ; hello

	.2byte	104,101,108,108,111,0
	.global	_main00026
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
