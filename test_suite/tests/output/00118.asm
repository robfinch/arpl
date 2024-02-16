   
	.text

;{++ _main00118

	.sdreg	29


	.bss
	.align	6
	.type	_main00118.00000,@object
	.size	_main00118.00000,8
_main00118.00000: ; __aggregate_tag
	.byte	0,0,0,0,0,0,0,0



_main00118:
; struct { int x; } s = { 0 };
  lda t0,<__aggregate_tag_0
  ldo t0,[t0]
; return s.x;
  ldo t0,0[t0]
  ldo a0,0[t0]
.00011:
  rts 
	.type	_main00118,@function
	.size	_main00118,$-_main00118


;--}

	.rodata
	.align	6

	.align	8



	.global	_main00118
	.extern	_start_data
	.extern	_start_bss
	.extern	_start_rodata
