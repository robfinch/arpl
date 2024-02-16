   
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
  sub sp,sp,32
  sto fp,[sp]
  mov fp,sp
  sub sp,sp,40
  sto s0,[sp]
  lda s0,-8[fp]
; struct { int x; } s = { 0 };
  lda t0,<__aggregate_tag_0
  ldo s0,[t0]
; return s.x;
  ldo a0,0[s0]
.00011:
  ldo s0,[sp]
  mov sp,fp
  ldo fp,[sp]
  rtd 32,0
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
